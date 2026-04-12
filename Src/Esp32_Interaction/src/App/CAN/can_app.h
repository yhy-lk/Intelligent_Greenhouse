/**
 * @file can_app.h
 * @brief CAN 应用程序层 - FreeRTOS 任务封装
 * * 负责创建并管理 CAN 总线的接收与处理任务。
 */

#ifndef CAN_APP_H
#define CAN_APP_H

#include <stdint.h>

namespace App {
namespace CAN {

/**
 * @brief 初始化 CAN 应用
 * * 1. 初始化底层 CAN 网络服务
 * 2. 创建并启动 FreeRTOS CAN 轮询任务
 * * @param tx_pin CAN TX 引脚号 (例如 47)
 * @param rx_pin CAN RX 引脚号 (例如 48)
 * @param baud_rate 波特率 (例如 250000)
 * @return true 初始化并创建任务成功
 * @return false 初始化失败
 */
bool init();

} // namespace CAN
} // namespace App

#endif // CAN_APP_H