/**
 * @file can_network_service.h
 * @brief CAN 网络服务层 - 硬件抽象与协议桥接
 * 
 * 此模块实现 CAN 网络服务层，作为 ESP32 TWAI/CAN 驱动与业务逻辑层之间的桥梁。
 * 负责硬件初始化、消息收发、协议解析和数据同步，实现现代 C++11 工程范式。
 * 
 * 架构定位：
 * - 上层：业务逻辑层 (SensorState) 和协议逻辑层 (GreenhouseProtocol)
 * - 下层：ESP32-S3 TWAI 硬件驱动 (driver/twai.h)
 * 
 * 核心功能：
 * 1. 使用 ESP-IDF 原生 driver/twai.h 驱动实现稳健的 CAN 通信
 * 2. 依赖注入模式：通过引用注入 SensorState，实现解耦和数据源唯一
 * 3. 配置灵活：通过结构体传递 GPIO 引脚和波特率，避免硬编码
 * 4. 生命周期管理：begin() 初始化，poll() 轮询，sendControl() 发送控制指令
 * 5. 错误处理：优雅处理 TWAI 错误，支持总线恢复逻辑
 * 
 * @note 使用 C++11 标准，适用于 PlatformIO 和 Arduino 框架
 */

#ifndef CAN_NETWORK_SERVICE_H
#define CAN_NETWORK_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

// ESP32 TWAI 驱动头文件
#include "driver/twai.h"

// 业务逻辑层依赖
#include "../SensorState/sensor_state.h"
#include "../Protocol/CAN/can_protocol.h"

namespace Service {

// ============================================================================
// 配置结构体
// ============================================================================

/**
 * @brief CAN 硬件配置参数结构体
 * 
 * 此结构体包含 TWAI 驱动初始化所需的所有硬件参数，
 * 避免在类文件中硬编码 GPIO 引脚号和波特率。
 */
struct CanConfig {
    gpio_num_t tx_pin;           ///< TX 引脚号 (GPIO_NUM_X)
    gpio_num_t rx_pin;           ///< RX 引脚号 (GPIO_NUM_X)
    uint32_t baud_rate;          ///< 波特率 (bps)，例如 250000, 500000
    uint32_t queue_size;         ///< 接收队列大小（消息数量），默认 20
    twai_mode_t mode;            ///< TWAI 工作模式，默认 TWAI_MODE_NORMAL
    
    /**
     * @brief 默认构造函数，提供合理的默认值
     */
    CanConfig()
        : tx_pin(GPIO_NUM_47)    // 根据硬件设计，TX 使用 GPIO 47
        , rx_pin(GPIO_NUM_48)    // 根据硬件设计，RX 使用 GPIO 48
        , baud_rate(250000)      // 250kbps 标准 CAN 波特率
        , queue_size(20)         // 合理队列大小
        , mode(TWAI_MODE_NORMAL) // 标准模式
    {}
    
    /**
     * @brief 全参数构造函数
     */
    CanConfig(gpio_num_t tx, gpio_num_t rx, uint32_t baud, 
              uint32_t queue = 20, twai_mode_t m = TWAI_MODE_NORMAL)
        : tx_pin(tx), rx_pin(rx), baud_rate(baud), queue_size(queue), mode(m)
    {}
};

// ============================================================================
// CAN 网络服务类
// ============================================================================

/**
 * @class CanNetworkService
 * @brief CAN 网络服务类 - 硬件抽象与协议桥接实现
 * 
 * 此类实现以下核心职责：
 * 1. 硬件抽象：封装 ESP32 TWAI 驱动，提供统一的 CAN 接口
 * 2. 协议桥接：将原始 CAN 消息转换为业务层数据，反之亦然
 * 3. 数据同步：将解析后的数据更新到 SensorState 数字孪生
 * 4. 错误处理：监控 CAN 总线状态，实现优雅的错误恢复
 * 
 * 设计原则：
 * - 依赖注入：不拥有 SensorState，通过引用访问
 * - 配置灵活：所有硬件参数通过结构体传递
 * - 资源安全：RAII 风格资源管理
 * - 线程安全：适用于 FreeRTOS 任务环境
 */
class CanNetworkService {
private:
    /// @name 内部状态
    /// @{
    bool is_initialized_;                ///< TWAI 驱动是否已初始化
    bool is_started_;                    ///< TWAI 驱动是否已启动
    CanConfig config_;                   ///< CAN 硬件配置
    /// @}
    
    /// @name 依赖注入
    /// @{
    Greenhouse::SensorState& sensor_state_; ///< 传感器状态引用（依赖注入）
    /// @}
    
    /// @name 统计信息
    /// @{
    uint32_t rx_count_;                  ///< 接收消息计数
    uint32_t tx_count_;                  ///< 发送消息计数
    uint32_t parse_error_count_;         ///< 协议解析错误计数
    uint32_t bus_error_count_;           ///< 总线错误计数
    /// @}
    
    // ========================================================================
    // 私有方法
    // ========================================================================
    
    /**
     * @brief 初始化 TWAI 驱动配置
     * 
     * @param config CAN 硬件配置
     * @return twai_general_config_t 配置好的通用配置结构
     */
    twai_general_config_t prepareGeneralConfig(const CanConfig& config) const;
    
    /**
     * @brief 初始化 TWAI 时序配置
     * 
     * @param baud_rate 波特率
     * @return twai_timing_config_t 配置好的时序配置结构
     */
    twai_timing_config_t prepareTimingConfig(uint32_t baud_rate) const;
    
    /**
     * @brief 初始化 TWAI 过滤器配置
     * 
     * @return twai_filter_config_t 配置好的过滤器配置结构
     * 
     * @note 当前配置为接受所有消息，可根据需求调整
     */
    twai_filter_config_t prepareFilterConfig() const;
    
    /**
     * @brief 处理接收到的 CAN 消息
     * 
     * @param message 接收到的 CAN 消息
     * 
     * @note 此方法解析消息、更新 SensorState，并处理协议逻辑
     */
    void processReceivedMessage(const twai_message_t& message);
    
    /**
     * @brief 处理 CAN 总线错误
     * 
     * @param error_flags 错误标志位
     * 
     * @note 记录错误信息，必要时尝试总线恢复
     */
    void handleBusError(uint32_t error_flags);
    
    /**
     * @brief 尝试恢复 CAN 总线
     * 
     * @return true 恢复成功
     * @return false 恢复失败
     */
    bool attemptBusRecovery();
    
    /**
     * @brief 打印调试信息
     * 
     * @param format 格式化字符串
     * @param ... 可变参数
     */
    void debugPrint(const char* format, ...) const;

public:
    // ========================================================================
    // 构造函数与析构函数
    // ========================================================================
    
    /**
     * @brief 构造函数（依赖注入）
     * 
     * @param sensor_state 传感器状态引用（必须有效）
     * 
     * @note 使用依赖注入模式，确保数据源唯一和解耦
     */
    explicit CanNetworkService(Greenhouse::SensorState& sensor_state);
    
    /**
     * @brief 析构函数
     * 
     * @note 自动停止并卸载 TWAI 驱动
     */
    ~CanNetworkService();
    
    // ========================================================================
    // 禁止拷贝和赋值
    // ========================================================================
    CanNetworkService(const CanNetworkService&) = delete;
    CanNetworkService& operator=(const CanNetworkService&) = delete;
    
    // ========================================================================
    // 生命周期管理 API
    // ========================================================================
    
    /**
     * @brief 初始化 CAN 网络服务
     * 
     * @param config CAN 硬件配置（引脚、波特率等）
     * @return true 初始化成功
     * @return false 初始化失败
     * 
     * @note 此方法安装并启动 TWAI 驱动，必须在 poll() 前调用
     */
    bool begin(const CanConfig& config);
    
    /**
     * @brief 轮询接收 CAN 消息
     * 
     * 此方法应在主循环或 FreeRTOS 任务中周期性调用，
     * 检查接收队列中的消息并进行处理。
     * 
     * @note 调用频率建议为 10-100ms，取决于消息负载
     */
    void poll();
    
    /**
     * @brief 停止 CAN 网络服务
     * 
     * 停止 TWAI 驱动但保持配置，可重新调用 begin() 恢复。
     */
    void stop();
    
    /**
     * @brief 检查服务是否已初始化
     * 
     * @return true 服务已初始化并运行
     * @return false 服务未初始化或已停止
     */
    bool isInitialized() const { return is_initialized_ && is_started_; }
    
    // ========================================================================
    // 数据发送 API
    // ========================================================================
    
    /**
     * @brief 发送控制指令到指定节点
     * 
     * @param node_id 目标节点 ID (0-15)
     * @param param_index 参数索引（例如 INDEX_WATER_PUMP）
     * @param value 物理值（例如 1.0 表示打开，0.0 表示关闭）
     * @return true 发送成功
     * @return false 发送失败（参数无效或发送错误）
     * 
     * @note 使用 FUNC_WRITE_SET 功能码发送目标设置
     */
    bool sendControl(uint8_t node_id, ParameterIndex param_index, float value);
    
    /**
     * @brief 发送原始 CAN 消息
     * 
     * @param id 11-bit CAN ID
     * @param data 8字节数据缓冲区
     * @param data_length 数据长度（必须为 8）
     * @return true 发送成功
     * @return false 发送失败
     */
    bool sendRaw(uint16_t id, const uint8_t* data, size_t data_length);
    
    // ========================================================================
    // 状态查询 API
    // ========================================================================
    
    /**
     * @brief 获取接收消息计数
     * 
     * @return uint32_t 接收消息总数
     */
    uint32_t getRxCount() const { return rx_count_; }
    
    /**
     * @brief 获取发送消息计数
     * 
     * @return uint32_t 发送消息总数
     */
    uint32_t getTxCount() const { return tx_count_; }
    
    /**
     * @brief 获取协议解析错误计数
     * 
     * @return uint32_t 解析错误总数
     */
    uint32_t getParseErrorCount() const { return parse_error_count_; }
    
    /**
     * @brief 获取总线错误计数
     * 
     * @return uint32_t 总线错误总数
     */
    uint32_t getBusErrorCount() const { return bus_error_count_; }
    
    /**
     * @brief 获取 TWAI 驱动状态
     * 
     * @return twai_status_info_t TWAI 状态信息结构体
     * 
     * @note 包含总线状态、错误计数等详细信息
     */
    twai_status_info_t getStatus() const;
    
    /**
     * @brief 重置统计信息
     * 
     * 重置所有计数器（接收、发送、错误等）。
     */
    void resetStatistics();
    
    /**
     * @brief 打印诊断信息
     * 
     * 输出当前状态、统计信息和配置参数。
     */
    void printDiagnostics() const;
};

} // namespace Service

#endif // CAN_NETWORK_SERVICE_H