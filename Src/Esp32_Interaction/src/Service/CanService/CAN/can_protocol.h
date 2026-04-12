/**
 * @file can_protocol.h
 * @brief 智能温室 CAN 总线协议层定义与工具库
 * * 本模块实现了温室自动化系统的 CAN 总线通信协议栈。
 * 负责 CAN 报文的打包/解包、物理浮点值与 CAN 原始整型数据的相互转换，
 * 以及特定数据格式的校验。
 * * 协议特性:
 * - 11 位标准 CAN ID (4 位功能码 + 7 位节点 ID)
 * - 8 字节定长数据帧，内部 4 字节对齐
 * - 小端字节序 (完美契合 ESP32-S3 / ARM 架构)
 * - 基于参数索引的寻址方式，内置缩放因子 (Scaling Factor) 转换
 * * @note 纯 C 实现，无任何动态内存分配。类型命名采用 Rust 规范 (PascalCase)。
 */

#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 协议位域常量定义
// ============================================================================

/** CAN ID 位分配 */
#define CAN_ID_FUNC_BITS       4
#define CAN_ID_NODE_BITS       7
#define CAN_ID_FUNC_SHIFT      CAN_ID_NODE_BITS
#define CAN_ID_MASK_FUNC       ((1 << CAN_ID_FUNC_BITS) - 1)
#define CAN_ID_MASK_NODE       ((1 << CAN_ID_NODE_BITS) - 1)

/** 系统极值定义 */
#define MAX_NODE_ID            ((1 << CAN_ID_NODE_BITS) - 1)  // 最大节点 ID: 127
#define MAX_FUNC_CODE          ((1 << CAN_ID_FUNC_BITS) - 1)  // 最大功能码: 15
#define CAN_DATA_SIZE          8                              // 标准帧载荷大小

// ============================================================================
// 枚举类型定义 (Rust 风格类型名)
// ============================================================================

/**
 * @brief CAN 报文功能码 (4-bit)
 */
typedef enum {
    CAN_FUNC_ALERT       = 0x0,  /**< 告警消息 (优先级最高，仲裁占优) */
    CAN_FUNC_TIME_SYNC   = 0x1,  /**< 时间同步广播 */
    CAN_FUNC_WRITE_SET   = 0x2,  /**< 主机下发控制/设定目标值 */
    CAN_FUNC_REPORT      = 0x3,  /**< 从机定时上报/应答当前状态 */
    
    CAN_FUNC_COUNT       = 4     /**< 功能码总数 */
} CanFuncCode;

/**
 * @brief 温室控制系统参数索引字典 (Data Dictionary)
 */
typedef enum {
    // ---- 系统级参数 (0x00 - 0x0F) ----
    PARAM_IDX_SYSTEM_TIMESTAMP  = 0x00,  /**< Unix 时间戳 (秒, u32) */
    PARAM_IDX_HEARTBEAT         = 0x01,  /**< 系统运行时间 (秒, u32) */
    PARAM_IDX_ERROR_CODE        = 0x02,  /**< 错误状态位掩码 (u32) */
    PARAM_IDX_CONTROL_MODE      = 0x03,  /**< 0=手动模式, 1=自动模式 (u32) */
    
    // ---- 开关执行器参数 (0x10 - 0x2F) | 0=OFF, 1=ON ----
    PARAM_IDX_LIGHT_MAIN_POWER  = 0x10,  /**< 主照明电源 */
    PARAM_IDX_WATER_PUMP        = 0x11,  /**< 灌溉水泵 */
    PARAM_IDX_HUMIDIFIER        = 0x12,  /**< 加湿器 */
    PARAM_IDX_VENTILATION_FAN   = 0x13,  /**< 通风风扇 */
    PARAM_IDX_SUNSHADE_MOTOR    = 0x14,  /**< 遮阳幕布电机 */
    PARAM_IDX_HEATER            = 0x15,  /**< 加热器 */
    PARAM_IDX_WINDOW_ACTUATOR   = 0x16,  /**< 开窗推杆电机 */
    
    // ---- 模拟量传感器读数 (0x30 - 0x4F) | 需缩放转换 ----
    PARAM_IDX_TEMPERATURE       = 0x30,  /**< 空气温度 (°C × 100) */
    PARAM_IDX_HUMIDITY_AIR      = 0x31,  /**< 空气湿度 (% × 100) */
    PARAM_IDX_HUMIDITY_SOIL     = 0x32,  /**< 土壤湿度 (% × 100) */
    PARAM_IDX_LIGHT_INTENSITY   = 0x33,  /**< 光照强度 (Lux × 1) */
    PARAM_IDX_CO2_LEVEL         = 0x34,  /**< CO2 浓度 (ppm × 1) */
    PARAM_IDX_FAN_SPEED         = 0x35,  /**< 风扇转速 (RPM × 1) */
    PARAM_IDX_SOIL_PH           = 0x36,  /**< 土壤 pH 值 (pH × 100) */
    PARAM_IDX_SOIL_EC           = 0x37,  /**< 土壤电导率 (μS/cm × 1) */
    PARAM_IDX_WATER_LEVEL       = 0x38,  /**< 水箱液位 (% × 1) */
    
    // ---- 高级控制参数 (0x50 - 0x5F) ----
    PARAM_IDX_LIGHT_COLOR_RGB   = 0x50,  /**< 照明颜色 (0x00RRGGBB) */
    PARAM_IDX_LIGHT_PWM_DUTY    = 0x51,  /**< 照明 PWM 占空比 (0-100) */
    
    PARAM_IDX_INVALID           = 0xFF   /**< 无效的参数索引边界值 */
} CanParamIndex;

// ============================================================================
// 数据结构定义 (Rust 风格类型名)
// ============================================================================

/**
 * @brief CAN 协议载荷结构体 (使用 GCC packed 属性保证严格的内存对齐)
 * * 内存布局 (8 bytes):
 * - Byte 0: 参数索引 (参考 CanParamIndex)
 * - Bytes 1-3: 保留对齐位 (必须填充为 0x00)
 * - Bytes 4-7: 32位整形数据值 (小端序)
 */
typedef struct __attribute__((packed)) {
    uint8_t  index;       /**< 参数索引 */
    uint8_t  reserved[3]; /**< 保留位 (强制置零) */
    uint32_t value;       /**< 数据值 (小端序传输) */
} CanDataFrame;

/**
 * @brief CAN 报文解析结果结构体
 */
typedef struct {
    CanFuncCode   func_code;    /**< 提取的功能码 */
    uint8_t       node_id;      /**< 提取的节点 ID */
    CanParamIndex param_index;  /**< 解析的参数索引 */
    uint32_t      raw_value;    /**< 从载荷中直接提取的原始整形数值 */
    float         scaled_value; /**< 还原出的物理浮点值 (处理过缩放因子后) */
    bool          is_scaled;    /**< 标识当前参数是否经过了缩放处理 */
    bool          is_valid;     /**< 标识该报文是否成功通过格式及语义校验 */
} CanParsedPacket;

// ============================================================================
// CAN ID 操作 API
// ============================================================================

/**
 * @brief 将功能码和节点 ID 打包成 11 位的标准 CAN ID
 * @param func 功能码 (4 bits)
 * @param node_id 节点 ID (7 bits)
 * @return uint16_t 合成的 11-bit CAN ID
 */
uint16_t can_proto_build_id(CanFuncCode func, uint8_t node_id);

/**
 * @brief 从 11 位 CAN ID 中解析功能码
 * @param id 11-bit CAN ID
 * @return CanFuncCode 提取的功能码
 */
CanFuncCode can_proto_get_func(uint16_t id);

/**
 * @brief 从 11 位 CAN ID 中解析节点 ID
 * @param id 11-bit CAN ID
 * @return uint8_t 提取的节点 ID (0-127)
 */
uint8_t can_proto_get_node(uint16_t id);

// ============================================================================
// 报文打包与解析 API (供业务层和底层收发任务调用)
// ============================================================================

/**
 * @brief 解析接收到的底层 CAN 报文
 * * 包含格式校验、保留位校验、物理值缩放还原等一整套安全处理逻辑。
 * * @param can_id 11-bit CAN ID
 * @param data 指向 8 字节 CAN 载荷数据的指针
 * @param out_result 指向结果结构体的指针 (用于输出解析结果)
 * @return true 报文合法且解析成功
 * @return false 报文非法 (格式错误、索引越界或指针为空)
 */
bool can_proto_parse_packet(uint32_t can_id, const uint8_t* data, CanParsedPacket* out_result);

/**
 * @brief 根据物理浮点值，打包生成要发送的 CAN 数据载荷
 * * 内部会自动根据参数索引查询缩放因子，将 float 转换为底层传输所需的 uint32_t。
 * * @param out_buffer 指向 8 字节数组的指针 (将被填充为符合协议的载荷数据)
 * @param index 参数索引
 * @param physical_value 业务逻辑层/UI 层传递的物理浮点值
 */
void can_proto_build_packet(uint8_t* out_buffer, CanParamIndex index, float physical_value);

// ============================================================================
// 调试与诊断辅助 API
// ============================================================================

/**
 * @brief 获取参数索引对应的可读字符串名称
 * @param index 参数索引
 * @return const char* 参数的英文名称 (例如 "Temperature")
 */
const char* can_proto_get_param_name(CanParamIndex index);

/**
 * @brief 获取功能码对应的可读字符串名称
 * @param func 功能码
 * @return const char* 功能码名称 (例如 "Write/Set")
 */
const char* can_proto_get_func_name(CanFuncCode func);

#ifdef __cplusplus
}
#endif

#endif // CAN_PROTOCOL_H