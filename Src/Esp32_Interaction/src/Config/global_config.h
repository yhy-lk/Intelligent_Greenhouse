#pragma once 

// FreeRTOS配置 - 重新启用，但使用更保守的配置
#define USE_FREERTOS 1  // 1:启用FreeRTOS, 0:禁用FreeRTOS

// 调试配置
#define DEBUG_MODE true

// CAN通信引脚配置
#define CAN_TX_PIN GPIO_NUM_47
#define CAN_RX_PIN GPIO_NUM_48
#define CAN_BAUD_RATE 250000
