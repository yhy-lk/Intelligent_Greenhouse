// #![no_std]

use embassy_stm32::can::filter::Mask32;
use embassy_stm32::can::{Frame, StandardId};

pub const NODE_ID: u16 = 0x01;

// ==========================================
// 1. CAN 功能码定义 (Function Codes)
// ==========================================
#[repr(u8)]
#[derive(Copy, Clone, Eq, PartialEq)]
pub enum FuncCode {
    Alert = 0x0,
    TimeSync = 0x1,
    Write = 0x2,
    Report = 0x3,
}

impl FuncCode {
    pub fn from_u16(val: u16) -> Option<Self> {
        match val {
            0x0 => Some(FuncCode::Alert),
            0x1 => Some(FuncCode::TimeSync),
            0x2 => Some(FuncCode::Write),
            0x3 => Some(FuncCode::Report),
            _ => None,
        }
    }
}

// ==========================================
// 2. 协议解析与打包逻辑
// ==========================================

/// 提取接收帧的 Node ID 和 功能码
pub fn parse_id(id: StandardId) -> (u16, Option<FuncCode>) {
    let raw = id.as_raw();
    let node_id = raw & 0x7F; // 提取低 7 位
    let func_code = FuncCode::from_u16((raw >> 7) & 0x0F); // 提取次低 4 位
    (node_id, func_code)
}

/// 解析 CAN 数据载荷 (提取 Index 和 Value)
pub fn parse_rx_payload(frame: &Frame) -> Option<(u8, u32)> {
    let data = frame.data();
    if data.len() == 8 {
        let index = data[0];
        
        // 提取 Byte 4~7 并进行小端序转换
        let mut val_bytes = [0u8; 4];
        val_bytes.copy_from_slice(&data[4..8]);
        let value = u32::from_le_bytes(val_bytes);
        
        return Some((index, value));
    }
    None
}

/// 封装 CAN 发送帧
pub fn build_tx_frame(func: FuncCode, index: u8, value: u32) -> Frame {
    let id_val = ((func as u16) << 7) | NODE_ID;
    let id = StandardId::new(id_val).unwrap();
    
    let mut data = [0u8; 8];
    data[0] = index;
    // Byte 1~3 隐式初始化为 0x00 (保留位)
    
    // 值转为小端序并填入 Byte 4~7
    data[4..8].copy_from_slice(&value.to_le_bytes());
    
    // 构建 Frame (API与给定版本对齐)
    Frame::new_data(id, &data).unwrap()
}

// ==========================================
// 3. 硬件过滤器配置
// ==========================================

/// 生成硬件过滤器配置
/// 目标: 仅允许目标地址为 0x00 (Broadcast) 和 0x01 (本节点) 的帧通过硬件层，丢弃其他。
pub fn get_can_filters() -> [Mask32; 2] {
    // 11位标准帧结构: [4位 功能码] + [7位 节点ID]
    // 掩码要求: 必须匹配节点 ID (低 7 位), 忽略功能码 (高 4 位)
    let mask = StandardId::new(0x07F).unwrap();
    
    let filter_broadcast = Mask32::frames_with_std_id(
        StandardId::new(0x000).unwrap(), 
        mask
    );
    
    let filter_node = Mask32::frames_with_std_id(
        StandardId::new(NODE_ID).unwrap(),
        mask
    );
    
    [filter_broadcast, filter_node]
}