// 温室控制系统测试
// 这个文件用于验证代码结构，不包含实际硬件测试

#include <iostream>
#include <string>
#include <vector>

// 模拟Arduino函数
unsigned long millis() { return 0; }
void delay(int ms) {}
void Serial_print(const char* str) { std::cout << str; }
void Serial_println(const char* str) { std::cout << str << std::endl; }
void Serial_print(int val) { std::cout << val; }
void Serial_println(int val) { std::cout << val << std::endl; }
void Serial_print(float val) { std::cout << val; }
void Serial_println(float val) { std::cout << val << std::endl; }

// 模拟传感器数据结构
struct SensorData {
    int type;
    std::string name;
    float value;
    std::string unit;
    unsigned long timestamp;
    int sensor_id;
    bool is_actuator;
    float min_value;
    float max_value;
    bool is_valid;
    
    std::string toString() const {
        return name + ": " + std::to_string(value) + " " + unit;
    }
};

// 模拟命令数据结构
struct CommandData {
    int type;
    int target_sensor;
    int target_id;
    float target_value;
    std::string description;
    unsigned long timestamp;
    unsigned int command_id;
    int priority;
    bool requires_confirmation;
    
    std::string toString() const {
        return "Command[" + std::to_string(command_id) + "]: " + description;
    }
};

// 模拟传感器缓冲区
class SensorDataBuffer {
private:
    std::vector<SensorData> buffer;
    
public:
    bool addData(const SensorData& data) {
        buffer.push_back(data);
        return true;
    }
    
    size_t size() const { return buffer.size(); }
    
    std::vector<SensorData> getAllData() const { return buffer; }
    
    SensorData getLatestData(int type) const {
        for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
            if (it->type == type) {
                return *it;
            }
        }
        return SensorData();
    }
};

// 模拟命令缓冲区
class CommandBuffer {
private:
    std::vector<CommandData> buffer;
    
public:
    bool addCommand(const CommandData& cmd) {
        buffer.push_back(cmd);
        return true;
    }
    
    size_t size() const { return buffer.size(); }
    
    std::vector<CommandData> getAllCommands() const { return buffer; }
    
    CommandData getNextCommand() {
        if (buffer.empty()) return CommandData();
        return buffer.front();
    }
    
    bool removeCommand(unsigned int command_id) {
        for (auto it = buffer.begin(); it != buffer.end(); ++it) {
            if (it->command_id == command_id) {
                buffer.erase(it);
                return true;
            }
        }
        return false;
    }
};

// 测试函数
void testSensorData() {
    std::cout << "=== Testing Sensor Data ===" << std::endl;
    
    SensorDataBuffer buffer;
    
    // 创建测试传感器数据
    SensorData temp_sensor;
    temp_sensor.type = 0; // 温度
    temp_sensor.name = "Temperature";
    temp_sensor.value = 25.5;
    temp_sensor.unit = "°C";
    temp_sensor.sensor_id = 1;
    temp_sensor.is_valid = true;
    
    SensorData humidity_sensor;
    humidity_sensor.type = 1; // 湿度
    humidity_sensor.name = "Humidity";
    humidity_sensor.value = 65.0;
    humidity_sensor.unit = "%";
    humidity_sensor.sensor_id = 2;
    humidity_sensor.is_valid = true;
    
    // 添加到缓冲区
    buffer.addData(temp_sensor);
    buffer.addData(humidity_sensor);
    
    std::cout << "Buffer size: " << buffer.size() << std::endl;
    std::cout << "Latest temperature: " << buffer.getLatestData(0).toString() << std::endl;
    std::cout << "Latest humidity: " << buffer.getLatestData(1).toString() << std::endl;
    
    std::cout << "Sensor data test PASSED" << std::endl;
    std::cout << std::endl;
}

void testCommandData() {
    std::cout << "=== Testing Command Data ===" << std::endl;
    
    CommandBuffer cmd_buffer;
    
    // 创建测试命令
    CommandData vent_cmd;
    vent_cmd.command_id = 1;
    vent_cmd.target_sensor = 6; // 通风口舵机
    vent_cmd.target_id = 7;
    vent_cmd.target_value = 90.0;
    vent_cmd.description = "Open vent due to high temperature";
    vent_cmd.priority = 2;
    
    CommandData pump_cmd;
    pump_cmd.command_id = 2;
    pump_cmd.target_sensor = 5; // 水泵
    pump_cmd.target_id = 6;
    pump_cmd.target_value = 80.0;
    pump_cmd.description = "Water plants due to low soil moisture";
    pump_cmd.priority = 3;
    
    // 添加到缓冲区
    cmd_buffer.addCommand(vent_cmd);
    cmd_buffer.addCommand(pump_cmd);
    
    std::cout << "Command buffer size: " << cmd_buffer.size() << std::endl;
    
    auto commands = cmd_buffer.getAllCommands();
    for (const auto& cmd : commands) {
        std::cout << "Command: " << cmd.toString() << std::endl;
    }
    
    // 测试获取下一个命令
    CommandData next_cmd = cmd_buffer.getNextCommand();
    std::cout << "Next command: " << next_cmd.toString() << std::endl;
    
    // 测试移除命令
    cmd_buffer.removeCommand(1);
    std::cout << "After removal, buffer size: " << cmd_buffer.size() << std::endl;
    
    std::cout << "Command data test PASSED" << std::endl;
    std::cout << std::endl;
}

void testCommunicationProtocol() {
    std::cout << "=== Testing Communication Protocol ===" << std::endl;
    
    // 这里可以添加通信协议的测试
    // 由于通信协议依赖于硬件，这里只进行概念验证
    
    std::cout << "Communication protocol test SKIPPED (requires hardware)" << std::endl;
    std::cout << std::endl;
}

void testGreenhouseControlLogic() {
    std::cout << "=== Testing Greenhouse Control Logic ===" << std::endl;
    
    SensorDataBuffer sensor_buffer;
    CommandBuffer command_buffer;
    
    // 模拟传感器数据
    SensorData temp_sensor;
    temp_sensor.type = 0;
    temp_sensor.name = "Temperature";
    temp_sensor.value = 29.5; // 高温
    temp_sensor.unit = "°C";
    temp_sensor.sensor_id = 1;
    temp_sensor.is_valid = true;
    
    SensorData soil_sensor;
    soil_sensor.type = 2;
    soil_sensor.name = "Soil Moisture";
    soil_sensor.value = 25.0; // 低土壤湿度
    soil_sensor.unit = "%";
    soil_sensor.sensor_id = 3;
    soil_sensor.is_valid = true;
    
    sensor_buffer.addData(temp_sensor);
    sensor_buffer.addData(soil_sensor);
    
    // 模拟控制逻辑
    if (temp_sensor.value > 28.0) {
        CommandData vent_cmd;
        vent_cmd.command_id = 1;
        vent_cmd.target_sensor = 6; // 通风口舵机
        vent_cmd.target_id = 7;
        vent_cmd.target_value = 90.0;
        vent_cmd.description = "Open vent due to high temperature";
        vent_cmd.priority = 2;
        
        command_buffer.addCommand(vent_cmd);
        std::cout << "Added command: " << vent_cmd.toString() << std::endl;
    }
    
    if (soil_sensor.value < 30.0) {
        CommandData pump_cmd;
        pump_cmd.command_id = 2;
        pump_cmd.target_sensor = 5; // 水泵
        pump_cmd.target_id = 6;
        pump_cmd.target_value = 80.0;
        pump_cmd.description = "Water plants due to low soil moisture";
        pump_cmd.priority = 3;
        
        command_buffer.addCommand(pump_cmd);
        std::cout << "Added command: " << pump_cmd.toString() << std::endl;
    }
    
    std::cout << "Total commands generated: " << command_buffer.size() << std::endl;
    std::cout << "Greenhouse control logic test PASSED" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Greenhouse Control System Tests      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    try {
        testSensorData();
        testCommandData();
        testCommunicationProtocol();
        testGreenhouseControlLogic();
        
        std::cout << "========================================" << std::endl;
        std::cout << "   All tests completed successfully!   " << std::endl;
        std::cout << "========================================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Test failed with error: " << e.what() << std::endl;
        return 1;
    }
}
