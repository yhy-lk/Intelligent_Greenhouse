// #![no_std]

use bsw::can_proto::{parse_id, parse_rx_payload, FuncCode, NODE_ID};
use bsw::protocol::{ControlMode, ParamIndex};
use crate::shared::GLOBAL_STATE;
use defmt::*;
use embassy_stm32::can::{Id, CanRx}; 

/// CAN 接收路由任务
/// 负责监听总线下发的 Write (0x2) 指令，并将其映射为内部控制目标写入全局状态单例
#[embassy_executor::task]
pub async fn can_rx_task(mut rx: CanRx<'static>) { // 修复 1：移除外设类型泛型参数
    info!("Starting CAN RX task");

    loop {
        match rx.read().await {
            Ok(envelope) => {
                let frame = envelope.frame;
                
                // 校验：仅支持 11-bit 标准帧
                let Id::Standard(std_id) = frame.id() else { 
                    trace!("Dropped non-standard CAN frame");
                    continue; 
                };

                // 修复 2：对 std_id 进行解引用，将其从 &StandardId 转换为 StandardId
                let (target_node, func_code_opt) = parse_id(*std_id);

                // 节点地址路由：忽略发给其他从机的单播帧
                if target_node != NODE_ID && target_node != 0x00 {
                    trace!("CAN RX: dropped by node route, target={=u16}, local={=u16}", target_node, NODE_ID);
                    continue;
                }

                let Some(func_code) = func_code_opt else { 
                    warn!("Received unknown function code");
                    continue; 
                };

                // 功能码路由：处理下发指令 (Write) 和时钟同步 (TimeSync)
                if func_code != FuncCode::Write && func_code != FuncCode::TimeSync {
                    trace!("CAN RX: dropped by func route, func={=u8}", func_code as u8);
                    continue;
                }

                // 载荷解析
                if let Some((index, value_u32)) = parse_rx_payload(&frame) {
                    info!(
                        "CAN RX frame accepted: id=0x{:03X}, node={=u16}, func={=u8}, index=0x{:02X}, value={} ",
                        std_id.as_raw(),
                        target_node,
                        func_code as u8,
                        index,
                        value_u32
                    );
                    
                    // 进入局部临界区更新状态
                    {
                        let mut state = GLOBAL_STATE.lock().await;

                        match index {
                            // A. 系统级参数
                            x if x == ParamIndex::SystemTimestamp as u8 => {
                                state.target.system_timestamp = Some(value_u32);
                            }
                            x if x == ParamIndex::ControlMode as u8 => {
                                state.target.control_mode = Some(if value_u32 == 1 {
                                    ControlMode::Auto
                                } else {
                                    ControlMode::Manual
                                });
                            }

                            // B. 开关/状态量
                            x if x == ParamIndex::LightMainPower as u8 => state.target.light_main_power = Some(value_u32 != 0),
                            x if x == ParamIndex::WaterPump as u8 => state.target.water_pump = Some(value_u32 != 0),
                            x if x == ParamIndex::Humidifier as u8 => state.target.humidifier = Some(value_u32 != 0),
                            x if x == ParamIndex::VentilationFan as u8 => state.target.ventilation_fan = Some(value_u32 != 0),
                            x if x == ParamIndex::SunshadeMotor as u8 => state.target.sunshade_motor = Some(value_u32 != 0),
                            x if x == ParamIndex::Heater as u8 => state.target.heater = Some(value_u32 != 0),
                            x if x == ParamIndex::WindowActuator as u8 => state.target.window_actuator = Some(value_u32 != 0),
                            x if x == ParamIndex::AuxRelay1 as u8 => state.target.aux_relay_1 = Some(value_u32 != 0),
                            x if x == ParamIndex::AuxRelay2 as u8 => state.target.aux_relay_2 = Some(value_u32 != 0),

                            // C. 模拟量：环境值设定
                            x if x == ParamIndex::Temperature as u8 => {
                                state.target.target_temperature = Some((value_u32 as f32) / 100.0);
                            }
                            x if x == ParamIndex::HumidityAir as u8 => {
                                state.target.target_humidity_air = Some((value_u32 as f32) / 100.0);
                            }
                            x if x == ParamIndex::HumiditySoil as u8 => {
                                state.target.target_humidity_soil = Some((value_u32 as f32) / 100.0);
                            }
                            x if x == ParamIndex::LightIntensity as u8 => {
                                state.target.target_light_intensity = Some(value_u32 as f32); 
                            }
                            x if x == ParamIndex::Co2Level as u8 => {
                                state.target.target_co2_level = Some(value_u32);
                            }
                            x if x == ParamIndex::FanSpeed as u8 => {
                                state.target.target_fan_speed_rpm = Some(value_u32);
                            }
                            x if x == ParamIndex::SoilPh as u8 => {
                                state.target.target_soil_ph = Some((value_u32 as f32) / 100.0);
                            }
                            x if x == ParamIndex::SoilEc as u8 => {
                                state.target.target_soil_ec = Some(value_u32);
                            }
                            x if x == ParamIndex::WaterLevel as u8 => {
                                state.target.target_water_level = Some(value_u32);
                            }

                            // D. 复杂参数
                            x if x == ParamIndex::LightColor as u8 => {
                                state.target.light_color_rgb = Some(value_u32);
                            }
                            x if x == ParamIndex::LightPwmDuty as u8 => {
                                state.target.light_pwm_duty = Some(value_u32);
                            }
                            x if x == ParamIndex::PidP as u8 => {
                                state.target.pid_p = Some((value_u32 as f32) / 1000.0);
                            }
                            x if x == ParamIndex::PidI as u8 => {
                                state.target.pid_i = Some((value_u32 as f32) / 1000.0);
                            }
                            x if x == ParamIndex::PidD as u8 => {
                                state.target.pid_d = Some((value_u32 as f32) / 1000.0);
                            }

                            _ => warn!("Unhandled ParamIndex in Write Cmd: {=u8}", index),
                        }
                    } 
                    
                    info!("CAN RX: state updated [index={=u8}, raw={}]", index, value_u32);
                }
            }
            Err(e) => {
                error!("CAN RX Bus Error: {:?}", e);
            }
        }
    }
}