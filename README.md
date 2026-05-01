# 🌿 EcoAgent-Greenhouse: 基于大模型的多智能体温室自主决策系统

EcoAgent-Greenhouse 并不是一个传统的“触发式”物联网（IoT）温室项目。它是一个由 AI 大模型驱动的**全生命周期作物生长管理中枢**。

本项目摒弃了传统的“温度>30度开启风扇”的僵化硬编码逻辑，引入了 **多智能体协作 (Multi-Agent Collaboration)** 与 **长链推理 (Chain of Thought)** 机制。系统能够像一个经验丰富的农场主一样，通过“视觉感知、环境推演、成本计算、自主行动、事后反思”的完整闭环，实现温室生态的自主平衡与最优化运作。

---

## ✨ 核心亮点 (Core Features)

* **👁️ 多模态视觉感知 (Multimodal Perception):** 不再仅仅依赖温湿度传感器，系统接入了视觉大模型。能够通过监控图像实时判断作物的生长阶段、叶片色泽，甚至提前识别病虫害征兆。
* **🧠 深度逻辑推理 (Deep Reasoning & Planning):** 基于长上下文模型，综合【实时传感器数据】+【未来 48 小时天气预报】+【土壤盐碱度历史】+【实时分时电价】，进行多维度、多方案的动态沙盘模拟推演。
* **🛠️ 自主工具调用 (Autonomous Tool Use):** AI Agent 能够自主生成指令并调用底层 IoT 硬件 API（如：无级调节遮阳板角度、智能分配多光谱补光灯策略、精准滴灌），而非简单的人工遥控。
* **🔄 自我反思机制 (Reflection & Self-Correction):** 每次执行物理操作后，系统会启动反思机制，评估环境指标的恢复曲线。如果偏离预期，Agent 会自动修正下一次的决策权重。

---

## 🏗️ 系统架构设计 (Architecture)

本系统由三个核心 Agent 协同运转：

1.  **感知与视觉 Agent (Perception Agent)**
    * 负责高频拉取各节点传感器数据流。
    * 调用多模态模型分析温室摄像头捕捉到的作物图像，输出结构化的“作物健康状态报告”。
2.  **策略规划 Agent (Strategy Planning Agent) —— *核心大脑***
    * 接收感知 Agent 的报告，发现异常（如：检测到叶片因干旱卷曲）。
    * **触发长链推理 (CoT)：** 不会立刻下发浇水指令，而是并行查询气象 API 与水务/电力成本。
    * **决策输出：** 权衡“立即滴灌”、“喷雾降温”或“调整顶棚遮阳”的最优解，兼顾植物存活率与能源消耗。
3.  **执行与反思 Agent (Execution & Reflection Agent)**
    * 将自然语言决策转化为具体的设备控制代码/API 调用。
    * 进入“监视期”，对比操作前后的环境变化，输出反馈日志供大脑 Agent 学习。

---

## 🚀 快速开始 (Getting Started)

### 环境要求
* Python 3.10+
* 依赖 IoT 硬件网关支持 (MQTT / RESTful API)
* **大模型 API 密钥** (推荐使用支持超长上下文及多模态的大模型，如小米 MiMo 等)

### 安装与运行
1. **克隆仓库**
   ```bash
   git clone [https://github.com/yhy-lk/Intelligent_Greenhouse.git](https://github.com/yhy-lk/Intelligent_Greenhouse.git)
   cd EcoAgent-Greenhouse