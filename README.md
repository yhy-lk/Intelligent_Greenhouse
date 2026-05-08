# 智能温室系统 -- Intelligent Greenhouse

基于 **ESP32-S3 + STM32F407VET6** 双 MCU 架构的智能温室控制系统，通过 CAN 总线通信，集成触摸屏 GUI、语音助手、DeepSeek 大模型推理与自动环境调控功能。

---

## 目录

- [系统概述](#系统概述)
- [系统架构](#系统架构)
- [功能特性](#功能特性)
- [硬件平台](#硬件平台)
- [软件架构](#软件架构)
- [CAN 总线协议](#can-总线协议)
- [引脚分配](#引脚分配)
- [项目结构](#项目结构)
- [构建与烧录](#构建与烧录)
- [配置说明](#配置说明)
- [许可证](#许可证)

---

## 系统概述

本系统是一个完整的嵌入式智能温室解决方案，包含以下核心能力：

- **环境感知** -- 温度、空气湿度、土壤湿度、光照强度等多维度传感器数据采集
- **精准控制** -- 通风风扇 PID 闭环调速、水泵/加湿器继电器控制、遮阳舵机调节、WS2812 补光灯 PWM 调光
- **触摸屏交互** -- 240x320 TFT 彩色触摸屏，基于 LVGL 9.3 的 8 屏 GUI 界面（仪表盘、手动/自动控制、数据趋势）
- **语音助手** -- INMP441 麦克风录音 -> 百度语音识别 -> DeepSeek 大模型推理 -> 百度语音合成 -> MAX98357A 扬声器播放
- **大模型集成** -- DeepSeek Chat API 实时注入传感器数据，充当"温室大脑"提供智能分析与建议
- **三种控制模式** -- 手动模式、自动模式（迟滞控制）、AI 领航模式（语音交互）

---

## 系统架构

```
+---------------------------+        CAN Bus (1 Mbps)        +---------------------------+
|     ESP32-S3 交互层       | <=============================> |    STM32F407 控制层        |
|     (Interaction Layer)   |     11-bit ID, 8-byte payload   |    (Control Layer)         |
+---------------------------+                                 +---------------------------+
|                           |                                 |                           |
|  [LVGL 触摸屏 GUI]       |     Write/Set (0x2)             |  [SHT30] 温度/湿度        |
|  [INMP441 麦克风]        |  -------------------------->    |  [BH1750] 光照强度        |
|  [MAX98357A 扬声器]      |     Report  (0x3)               |  [ADC] 土壤湿度           |
|  [WS2812 状态灯]         |  <---------------------------   |  [QEI] 风扇转速编码器     |
|  [WiFi 模块]             |                                 |                           |
|                           |     Alert   (0x0)               |  [PID 通风风扇]           |
|  [DeepSeek API] LLM      |  <---------------------------   |  [水泵/加湿器 继电器]     |
|  [百度 STT/TTS] 语音     |                                 |  [MG995 遮阳舵机]         |
|                           |                                 |  [WS2812 补光灯]          |
+---------------------------+                                 |  [SSD1306 OLED 状态屏]    |
                                                              +---------------------------+

数据流向:
  传感器 -> STM32 GLOBAL_STATE -> CAN TX -> ESP32 sensor_state -> LVGL UI / DeepSeek API
  触摸操作 -> ESP32 CAN TX -> CAN Bus -> STM32 CAN RX -> GLOBAL_STATE -> 执行器
  语音输入 -> 百度 STT -> DeepSeek LLM -> 百度 TTS -> 扬声器播放
```

---

## 功能特性

### 三种控制模式

| 模式 | 说明 |
|------|------|
| **手动模式** | 通过触摸屏直接控制每个执行器的开关/参数（水泵开关、风扇转速滑块、补光灯亮度/颜色滑块等） |
| **自动模式** | STM32 迟滞控制调度器自动调节：温度过高开风扇、湿度过低开加湿器、土壤过干开水泵、光照不足开补光灯 |
| **AI 领航模式** | 语音交互驱动：说出问题 -> 百度 STT 识别 -> DeepSeek 大模型（注入实时传感器数据）-> 百度 TTS 语音回答 |

### PID 闭环风扇控制

- 离散位置式 PID 控制器，带抗积分饱和
- 正交编码器反馈（TIM3 QEI），20kHz PWM 输出（超静音）
- Kp/Ki/Kd 参数可通过 CAN 总线远程调整
- 默认参数：Kp=2.0, Ki=0.5, Kd=0.0

### 数字孪生状态管理

- 双端互斥锁保护的全局状态单例
- ESP32 端支持最多 16 个节点，5 分钟超时离线检测
- LVGL 快照机制防止 UI 数据撕裂

### GUI 界面（8 屏）

| 屏幕 | 功能 |
|------|------|
| Home | 主导航界面，日期时间显示 |
| Overview | 传感器仪表盘（温度弧形、湿度弧形、土壤柱形、光照数值） |
| Control | 模式选择入口（手动/自动/AI 领航） |
| Manual Mode | 执行器直接控制（遮阳开关、水泵开关、加湿器开关、风扇转速、补光灯亮度/颜色） |
| Auto Mode | 目标范围设置（温度、湿度、土壤湿度、光照强度的双滑块范围选择） |
| AI Pilot Mode | 语音助手界面（进入激活录音，退出停止） |
| Trend | 数据趋势展示 |
| Settings | 系统设置 |

---

## 硬件平台

### ESP32-S3 交互层

| 组件 | 型号/规格 | 说明 |
|------|-----------|------|
| 主控芯片 | ESP32-S3 (8MB PSRAM, 16MB Flash) | 4D Systems Gen4 R8N16 模组 |
| 显示屏 | ILI9341 TFT 240x320 | 8-bit 8080 并行总线，RGB565 |
| 触摸屏 | XPT2046 电阻式触摸 | SPI 接口，1MHz |
| 麦克风 | INMP441 I2S MEMS | 16kHz 采样，32-bit |
| 功放 | MAX98357A I2S | 驱动扬声器播放 TTS 语音 |
| 状态灯 | WS2812B x 12 | 呼吸/加载/告警灯效 |
| CAN 收发器 | TWAI | GPIO48(TX)/GPIO47(RX), 1Mbps |

### STM32F407 控制层

| 组件 | 型号/规格 | 说明 |
|------|-----------|------|
| 主控芯片 | STM32F407VET6 | ARM Cortex-M4F, 168MHz, FPU |
| 温湿度传感器 | SHT30 | I2C2, 100kHz |
| 光照传感器 | BH1750 | I2C2, 共享总线 |
| 土壤湿度传感器 | 模拟输出 | ADC1, PC1, 带 VREFINT 校准 |
| OLED 显示屏 | SSD1306 128x64 | I2C1, 400kHz |
| 通风风扇 | DC 电机 + H 桥 | PWM(TIM1, 20kHz) + 编码器(TIM3 QEI) + PID 闭环 |
| 水泵 | 继电器控制 | GPIO PD13 |
| 加湿器 | 继电器控制 | GPIO PE4 |
| 遮阳舵机 | MG995 | PWM(TIM5, 50Hz), 0-300 度 |
| 补光灯 | WS2812B x 25 | PWM-DMA(TIM4), PD12 |
| CAN 收发器 | CAN1 | PB8(RX)/PB9(TX), 1Mbps |

---

## 软件架构

### ESP32 交互层 -- AUTOSAR 分层架构

```
App 层          CAN App, LVGL App (FreeRTOS 任务编排)
  |
Service 层      CAN Service, SensorState, DeepSeekAPI, BaiduApi,
                VoiceAssistant, WifiService, LedService, GUI Manager
  |
ECUAL 层        Display HAL, INMP441 Driver, MAX98357A Driver
  |
MCAL 层         I2S HAL, WiFi HAL
  |
BSP 层          OS Service (FreeRTOS 抽象)
  |
Config          global_config, app_config, deepseek_config, secrets
```

**FreeRTOS 任务：**

| 任务 | 核心 | 优先级 | 周期 | 职责 |
|------|------|--------|------|------|
| `can_rx_task` | 默认 | CRITICAL(4) | 10ms | CAN 总线轮询接收 |
| `ui_update_task` | 默认 | NORMAL(2) | 5ms | LVGL 渲染 + 传感器数据刷新 |
| `ui_event_task` | 默认 | LOW(1) | 20ms | UI 事件处理 |
| `VoiceTask` | Core 1 | NORMAL(2) | 可变 | 语音助手状态机 |
| `loopTask` | 默认 | 1 | 60s | 内存监控 |

### STM32 控制层 -- Embassy 异步架构

```
firmware/       主二进制，域任务（CAN RX/TX, 传感器, 执行器, 调度器, UI）
  |
crates/app/     应用逻辑（BH1750, LED, SHT30, UI, WS2812 任务封装）
  |
crates/bsw/     基础软件（SHT30/BH1750 驱动, CAN 协议, PID 控制器, 电机控制, WS2812）
  |
crates/service/  服务层（颜色定义, LED 服务）
```

**异步任务：** 12+ 并发 Embassy 任务，包括 CAN 收发、传感器轮询、执行器控制、自动调度、OLED UI 更新、硬件自检等。

### 语音助手状态机

```
WAITING --[音量 > 500]--> RECORDING --[静音 1.5s]--> PROCESSING_STT
    ^                                                        |
    |                                                   [百度 STT]
    |                                                        v
    +---[播放完成]--- PLAYBACK <--[百度 TTS]-- PROCESSING_TTS <--[DeepSeek]-- PROCESSING_LLM
```

---

## CAN 总线协议

### 帧格式

采用标准 CAN 2.0A（11-bit ID），8 字节定长数据帧：

```
CAN ID (11-bit):  [功能码(4-bit)] [节点ID(7-bit)]

数据帧 (8 bytes):
  Byte 0:     参数索引 (Parameter Index)
  Byte 1-3:   保留 (Reserved, 0x00)
  Byte 4-7:   参数值 (uint32, little-endian)
```

### 功能码

| 功能码 | 值 | 说明 | 优先级 |
|--------|------|------|--------|
| ALERT | 0x0 | 告警帧 | 最高 |
| TIME_SYNC | 0x1 | 时间同步 | 高 |
| WRITE_SET | 0x2 | 主机下发控制命令 | 中 |
| REPORT | 0(3) | 从机上报传感器数据 | 低 |

### 参数字典（节选）

| 参数 | 索引 | 缩放因子 | 单位 |
|------|------|----------|------|
| 控制模式 | 0x03 | - | 0=手动, 1=自动 |
| 补光灯开关 | 0x10 | - | bool |
| 水泵开关 | 0x11 | - | bool |
| 加湿器开关 | 0x12 | - | bool |
| 通风风扇开关 | 0x13 | - | bool |
| 遮阳电机 | 0x14 | - | bool |
| 温度 | 0x30 | x100 | °C |
| 空气湿度 | 0x31 | x100 | %RH |
| 土壤湿度 | 0x32 | x100 | % |
| 光照强度 | 0x33 | x1 | Lux |
| 风扇转速 | 0x35 | x1 | RPM |
| 补光灯颜色 RGB | 0x50 | x1 | uint32 |
| 补光灯 PWM 占空比 | 0x51 | x1 | % |
| PID Kp | 0x60 | x100 | - |
| PID Ki | 0x61 | x100 | - |
| PID Kd | 0x62 | x100 | - |

完整参数定义见：
- ESP32 端：[can_protocol.h](Src/Esp32_Interaction/src/Service/CanService/CAN/can_protocol.h)
- STM32 端：[protocol.rs](Src/Stm32_Control/crates/bsw/src/protocol.rs)

---

## 引脚分配

### ESP32-S3 引脚

<details>
<summary>点击展开完整引脚表</summary>

| 外设 | 信号 | GPIO |
|------|------|------|
| CAN (TWAI) | TX | 48 |
| CAN (TWAI) | RX | 47 |
| 显示 (8080) | D0-D7 | 4, 5, 6, 7, 8, 9, 10, 11 |
| 显示 (8080) | WR | 12 |
| 显示 (8080) | RS (DC) | 13 |
| 显示 (8080) | CS | 14 |
| 显示 (8080) | RST | 15 |
| 显示 (8080) | BL (PWM) | 16 |
| 触摸 (SPI) | SCLK | 17 |
| 触摸 (SPI) | MOSI | 38 |
| 触摸 (SPI) | MISO | 18 |
| 触摸 (SPI) | CS | 21 |
| 触摸 (SPI) | INT | 39 |
| I2S 音频 | BCLK (共享) | 40 |
| I2S 音频 | LRCK (共享) | 41 |
| I2S 音频 | DOUT (功放) | 42 |
| I2S 音频 | DIN (麦克风) | 1 |
| WS2812 | Data | 2 |

</details>

### STM32F407VET6 引脚

<details>
<summary>点击展开完整引脚表</summary>

| 外设 | 信号 | 引脚 |
|------|------|------|
| I2C2 (传感器) | SCL | PB10 |
| I2C2 (传感器) | SDA | PB11 |
| I2C1 (OLED) | SCL | PB6 |
| I2C1 (OLED) | SDA | PB7 |
| ADC1 (土壤湿度) | 模拟输入 | PC1 |
| CAN1 | RX | PB8 |
| CAN1 | TX | PB9 |
| 通风风扇 PWM | TIM1 CH1 | PE9 |
| 通风风扇方向 | IN1 | PE10 |
| 通风风扇方向 | IN2 | PE11 |
| 风扇编码器 A | TIM3 CH1 | PC6 |
| 风扇编码器 B | TIM3 CH2 | PC7 |
| 水泵继电器 | GPIO | PD13 |
| 加湿器继电器 | GPIO | PE4 |
| 遮阳舵机 | TIM5 CH2 | PA1 |
| WS2812 补光灯 | TIM4 CH1 | PD12 |
| 板载 LED | GPIO | PC0 |

</details>

---

## 项目结构

```
Intelligent_Greenhouse/
├── README.md
├── .gitignore
├── Greenhouse.code-workspace          # VS Code 多根工作区
│
├── Src/
│   ├── Esp32_Interaction/             # ESP32-S3 交互层 (C/C++, PlatformIO)
│   │   ├── platformio.ini             # PlatformIO 配置
│   │   ├── build_middleware.py         # 构建脚本（跳过 LVGL 汇编文件）
│   │   └── src/
│   │       ├── main.cpp               # 入口点
│   │       ├── Config/                # 配置头文件
│   │       │   ├── global_config.h    # 引脚、CAN、显示、I2S 配置
│   │       │   ├── app_config.h       # FreeRTOS 任务参数
│   │       │   ├── deepseek_runtime_config.h  # DeepSeek API 配置
│   │       │   ├── secrets_temp.h     # 凭证模板
│   │       │   └── lv_conf.h          # LVGL 配置
│   │       ├── App/                   # 应用层（CAN App, LVGL App）
│   │       ├── Service/               # 服务层
│   │       │   ├── CanService/        # CAN 协议 + 网络服务
│   │       │   ├── SensorState/       # 数字孪生状态管理
│   │       │   ├── DeepSeekAPI/       # DeepSeek 大模型接口
│   │       │   ├── BaiduApi/          # 百度 STT/TTS 接口
│   │       │   ├── VoiceAssistant/    # 语音助手状态机
│   │       │   ├── WifiService/       # WiFi 多凭证管理
│   │       │   ├── NetworkService/    # HTTP 客户端
│   │       │   ├── LedService/        # WS2812 灯效服务
│   │       │   ├── GUI/               # LVGL GUI (GUI Guider 生成 + 自定义)
│   │       │   └── Utils/             # 工具函数
│   │       ├── ECUAL/                 # ECU 抽象层（显示/麦克风/功放驱动）
│   │       ├── MCAL/                  # MCU 抽象层（I2S HAL, WiFi HAL）
│   │       └── BSP/                   # 板级支持包
│   │
│   └── Stm32_Control/                 # STM32F407 控制层 (Rust, Embassy)
│       ├── Cargo.toml                 # Cargo 工作区配置
│       ├── .cargo/config.toml         # 构建目标与 probe-rs 配置
│       ├── firmware/                  # 主二进制
│       │   └── src/bin/
│       │       ├── main.rs            # 入口点，外设初始化，任务启动
│       │       ├── shared.rs          # 全局状态 GLOBAL_STATE
│       │       └── domains/           # 域任务
│       │           ├── can_rx.rs      # CAN 接收任务
│       │           ├── can_tx.rs      # CAN 发送任务
│       │           ├── sensor.rs      # 传感器读取任务
│       │           ├── actuator.rs    # 执行器控制任务
│       │           ├── dispatcher.rs  # 自动调度器（迟滞控制）
│       │           ├── ui.rs          # OLED 显示任务
│       │           ├── hw_test.rs     # 硬件自检
│       │           └── config.rs      # 系统时钟配置
│       └── crates/
│           ├── bsw/                   # 基础软件（驱动 + 协议）
│           │   ├── sht30.rs           # 温湿度传感器驱动
│           │   ├── bh1750.rs          # 光照传感器驱动
│           │   ├── can_proto.rs       # CAN 协议实现
│           │   ├── pid.rs             # PID 控制器
│           │   ├── motor_ctrl.rs      # 电机控制
│           │   └── protocol.rs        # 参数字典与状态定义
│           ├── app/                   # 应用逻辑封装
│           └── service/               # 服务层（颜色、LED）
│
└── Document/                          # 文档目录 (gitignored)
```

---

## 构建与烧录

### 前置条件

| 工具 | 用途 | 安装方式 |
|------|------|----------|
| [PlatformIO](https://platformio.org/) | ESP32 构建与烧录 | VS Code 扩展 或 CLI |
| [Rust](https://rustup.rs/) | STM32 编译 | `rustup` |
| [probe-rs](https://probe.rs/) | STM32 烧录与调试 | `cargo install probe-rs --features cli` |

### ESP32-S3 构建与烧录

```bash
cd Src/Esp32_Interaction

# 构建
pio run

# 烧录（默认 COM23，可在 platformio.ini 中修改）
pio run --target upload

# 串口监视器
pio device monitor
```

### STM32F407 构建与烧录

```bash
cd Src/Stm32_Control

# 构建
cargo build --bin main --release

# 烧录并运行（通过 SWD 调试器连接）
cargo run --bin main --release

# 使用 probe-rs 调试（配合 VS Code launch.json）
probe-rs run --chip STM32F407VETx --protocol swd
```

---

## 配置说明

### 凭证配置

复制模板文件并填入实际凭证：

```bash
cp Src/Esp32_Interaction/src/Config/secrets_temp.h \
   Src/Esp32_Interaction/src/Config/secrets.h
```

在 `secrets.h` 中配置：

| 配置项 | 说明 |
|--------|------|
| `WIFI_CREDENTIALS[]` | WiFi SSID/密码数组（支持多网络自动切换） |
| `DEEPSEEK_API_KEY` | DeepSeek API 密钥 |
| `DEEPSEEK_API_URL` | DeepSeek API 地址（默认 `api.deepseek.com`） |
| `BAIDU_API_KEY` | 百度云 API Key |
| `BAIDU_SECRET_KEY` | 百度云 Secret Key |

> `secrets.h` 已被 `.gitignore` 忽略，不会提交到版本控制。

---

## 许可证

STM32 固件代码采用 MIT OR Apache-2.0 双许可。其余代码未指定许可。
