# 基于织物电子与智能可穿戴的体态姿态感知与无线生物反馈系统
# Fabric-Electronics-Based Wearable Posture Sensing & Wireless Biofeedback System

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](LICENSE)
[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266%20Xtensa%2032bit-red.svg?style=flat-square)](https://www.espressif.com/)
[![Sensor: MPU6050](https://img.shields.io/badge/IMU-InvenSense%20MPU6050-green.svg?style=flat-square)](https://invensense.tdk.com/)
[![Medium: E--Textile Conductive Yarn](https://img.shields.io/badge/E--Textile-Conductive%20Yarn%20Matrix-purple.svg?style=flat-square)](docs/)
[![Protocol: I2C & TCP/IP](https://img.shields.io/badge/Protocol-I2C%20400kHz%20%7C%20TCP%20Socket-orange.svg?style=flat-square)](src/)

[**中文文档**](README.md) | [**English**](README_EN.md) | [**日本語**](README_JA.md)

</div>

---

## 1. 项目概述 (Project Overview)

**基于织物电子与智能可穿戴的体态姿态感知与无线生物反馈系统** 是一套面向智能健康监测、脊柱姿态矫正与数字化人机交互的高集成度柔性可穿戴物联网系统。

传统刚性可穿戴设备常存在皮肤贴合差、异物感强、导线易疲劳断裂等缺陷。本项目创新性地采用**高导电不锈钢/镀银复合纤维微细导电纱线（Conductive Yarn）**，通过精密刺绣与双层绝缘走线工艺，将高精度惯性测量传感单元与核心计算控制节点直接嵌入全棉贴身衣物基底，构建出具备零束缚感、高耐弯折性能的柔性织物电子微互连总线。

系统以低功耗 **Espressif ESP8266 (ESP-12F) 32 位 Xtensa 微处理器**为边缘计算枢纽，结合 **InvenSense MPU-6050 六轴运动跟踪传感器（3 轴 MEMS 加速度计 + 3 轴 MEMS 陀螺仪）**，实时采集人体躯干的空间运动动力学特征与三维倾角数据。通过在端侧运行的互补滤波姿态解算算法与人体运动动能估算模型，系统可精确识别脊柱前倾、伏案含胸、侧向倾斜等不良体态，并通过板载非线性呼吸脉宽调制（PWM）光学生物反馈以及实时 Wi-Fi TCP/IP / WebServer 远程遥测双重通道，实现毫米级运动姿态感测与闭环健康干预。

---

## 2. 硬件实物与工程架构展示 (Visual Showcase)

<div align="center">

| 系统全景与端云数据流拓扑架构 | 织物电子智能服饰与圆形主控节点实物 |
| :---: | :---: |
| <img src="docs/images/system_architecture.png" width="450" alt="System Architecture"> | <img src="docs/images/demo_e_textile_garment.jpg" width="450" alt="E-Textile Garment"> |
| **MPU6050 空间姿态惯性感知节点与缝合走线** | **圆形可穿戴 ESP8266 主控与导电纱线触点布局** |
| <img src="docs/images/demo_sensing_node.jpg" width="450" alt="Sensing Node"> | <img src="docs/images/hardware_esp8266.jpg" width="450" alt="Hardware Node"> |

</div>

---

## 3. 核心数学模型与运动学生物力学推导 (Mathematical Formulations)

本项目在微控制器端与遥测上位机实现了严格的物理力学建模，涵盖柔性微应变电学响应、空间姿态角估计、三维运动能量量化以及人眼感知非线性反馈律。

### 3.1 导电纱线压阻应变模型与电桥信号调理方程

柔性导电纱线缝合于织物表面随躯干拉伸发生几何形变与纤维微观接触电阻变化。设未拉伸状态初始电阻为 $R_0$，应变量为 $\varepsilon = \frac{\Delta L}{L_0}$，纱线灵敏度系数（Gauge Factor）为 $G_F$，则等效阻值数学模型满足：

$$
R(\varepsilon) = R_0 (1 + G_F \cdot \varepsilon)
$$

在微小应变采集电路中，采用对称惠斯通电桥进行非线性差分放大调理，输出差分电压方程表示为：

$$
V_{\text{out}} = V_{\text{in}} \left( \frac{R_2}{R_1 + R_2} - \frac{R_x(\varepsilon)}{R_3 + R_x(\varepsilon)} \right)
$$

当满足全桥平衡基准 $$R_1 = R_2 = R_3 = R_0$$ 时，输出电压关于机械微应变的一阶线性化响应为：

$$
V_{\text{out}} \approx \frac{V_{\text{in}}}{4} \cdot G_F \cdot \varepsilon
$$

### 3.2 基于静态重力矢量的空间欧拉姿态角解算模型

设三轴 MEMS 加速度计测得的归一化重力加速度分量为向量 $$\mathbf{a} = [a_x, a_y, a_z]^{\top}$$。根据刚体运动学投影原理，定义躯干空间倾仰角（Pitch Angle）为 $\theta$，侧向翻滚角（Roll Angle）为 $\phi$。

倾仰角 $\theta$（俯仰/驼背程度）：

$$
\theta = \arctan \left( \frac{-a_x}{\sqrt{a_y^2 + a_z^2}} \right) \cdot \frac{180^\circ}{\pi}
$$

翻滚角 $\phi$（侧向倾斜/脊柱侧弯）：

$$
\phi = \arctan \left( \frac{a_y}{a_z} \right) \cdot \frac{180^\circ}{\pi}
$$

### 3.3 加速度计与陀螺仪时域离散互补姿态融合滤波

为消除加速度计的高频机械震颤噪声与陀螺仪的时域低频漂移积分发散，系统在微控制器内部部署一阶离散互补滤波器。设角速度测量值为 $\omega_{\text{gyro}}$，加速度计瞬时计算角为 $\theta_{\text{acc}}$，采样周期为 $\Delta t$，截止时间常数为 $\tau$：

$$
\theta_t = \alpha (\theta_{t-1} + \omega_{\text{gyro}} \cdot \Delta t) + (1 - \alpha) \theta_{\text{acc}}
$$

其中平滑衰减增益因子 $\alpha$ 满足：

$$
\alpha = \frac{\tau}{\tau + \Delta t}, \quad (0.95 \le \alpha \le 0.98)
$$

### 3.4 三维欧氏加速度范数与人体运动动能度量模型

为动态区分静态体态保持与剧烈步行动作，计算合加速度矢量的欧几里得范数 $|\mathbf{a}|$：

$$
|\mathbf{a}| = \sqrt{a_x^2 + a_y^2 + a_z^2}
$$

在动态运动窗口 $$t \in [t_0, t_0 + T]$$ 内，剔除静态重力向量 $\mathbf{g}$ 后的躯干动力学微元动能积分为：

$$
E_{\text{kinetic}} = \frac{1}{2} m \int_{t_0}^{t_0 + T} \left\| \mathbf{a}(t) - \mathbf{g} \right\|^2 \, dt
$$

当 $|\mathbf{a}| \approx 1.0g$ 时判定为静态或准静态姿态，系统激活高灵敏度体态异常判决逻辑；当动态合加速度显著偏离 $1.0g$ 时，抑制误报触发。

### 3.5 韦伯-费希纳非线性视觉呼吸生物反馈控制律

为人眼提供自然、平缓的生物体感光反馈，LED 占空比 $D(t)$ 基于人眼视觉对数感应特性（Weber-Fechner Law）进行幂律矫正，呼吸调制函数表示为：

$$
D(t) = D_{\min} + (D_{\max} - D_{\min}) \cdot \left[ \frac{1}{2} \left( 1 + \sin \left( \frac{2\pi t}{T_{\text{breath}}} - \frac{\pi}{2} \right) \right) \right]^\gamma
$$

其中呼吸周期 $T_{\text{breath}} = 3.0\text{ s}$，非线性心理物理指数取 $\gamma = 2.2$。当姿态倾角超出临界阈值 $|\theta| > \theta_{\text{threshold}}$ 时，系统瞬间切换至高频频闪报警模式（$f = 10\text{ Hz}$）。

---

## 4. 软硬件系统技术指标 (System Specifications)

| 模块类别 | 硬件/技术选型 | 参数指标与实现标准 |
| :--- | :--- | :--- |
| **主控芯片 (MCU)** | Espressif ESP8266 (ESP-12F) | Tensilica 32-bit LX106 @ 80MHz/160MHz, 4MB Flash |
| **惯性传感器 (IMU)** | InvenSense MPU-6050 | 3 轴加速度计 ($\pm 2g$) + 3 轴陀螺仪 ($\pm 250^\circ/s$), 16 位 ADC |
| **总线与协议** | Hardware I2C Fast-Mode | 通信时钟 $f_{\text{SCL}} = 400\text{ kHz}$，实时轮询周期 $10\text{ ms}$ |
| **织物导电走线** | 柔性微米级复合导电线 | 直径 $0.2\text{ mm}$，电阻率 $< 1.5\,\Omega/\text{cm}$，耐弯折性能优秀 |
| **无线通信拓扑** | 802.11 b/g/n Wi-Fi | 具备 SoftAP + Station 双模式，内置 HTTP WebServer & TCP 客户端 |
| **本地生物反馈** | GPIO PWM 呼吸灯 | 10 位分辨率 ($0 \sim 1023$)，支持静息呼吸态与超限高频警报态 |
| **远程监控终端** | Web 响应式仪表板 / TCP Socket | 支持毫秒级姿态遥测流推送，无缝接入 PC / 智能手机浏览器 |

---

## 5. 目录结构说明 (Repository Layout)

```text
Fabric-Electronics-Based-Wearable-Posture-Sensing-System/
├── docs/
│   └── images/
│       ├── system_architecture.png       # 系统总体拓扑架构与数据流向设计图
│       ├── demo_e_textile_garment.jpg    # 服饰织物走线与主控节点实物图 (1920x1280)
│       ├── demo_sensing_node.jpg         # MPU6050 空间惯性感应节点特写图 (1920x1280)
│       ├── hardware_esp8266.jpg          # 圆形穿戴式 ESP8266 硬件节点实拍图
│       └── hardware_mpu6050.jpg          # MPU6050 芯片硬件模块实拍图
├── src/
│   └── firmware/
│       ├── wearable_posture_node.ino     # 综合量产级嵌入式固件 (I2C+姿态滤波+Web+TCP)
│       └── examples/
│           ├── sketch_mar26a_tcp_imu.ino # 基础 TCP 通信与 MPU6050 初始化例程
│           ├── sketch_mar19a2_pwm.ino    # 基础非线性呼吸 PWM 调制例程
│           └── sketch_mar12a1_blink.ino  # 硬件连通性与 GPIO 点灯测试例程
├── LICENSE                               # 官方 MIT 开源许可协议
├── README.md                             # 中文工程技术展示与数学模型说明
├── README_EN.md                          # English Engineering Specification
└── README_JA.md                          # 日本語技術仕様書・学術ポートフォリオ
```

---

## 6. 固件部署与快速启动指南 (Quick Start)

### 6.1 硬件连接定义 (Pin Mapping)

| ESP8266 引脚 | 织物走线分配 | MPU6050 传感器引脚 | 功能定义说明 |
| :---: | :---: | :---: | :---: |
| **3V3** | 导电纱线 VCC 总线 | VCC | 3.3V 稳压系统供电 |
| **GND** | 导电纱线 GND 总线 | GND | 信号与电源公共地 |
| **GPIO4 (D2)** | 导电纱线 SDA 信号线 | SDA | I2C 双向串行数据总线 |
| **GPIO5 (D1)** | 导电纱线 SCL 时钟线 | SCL | I2C 串行时钟线 ($400\text{ kHz}$) |
| **GPIO2 (D4)** | 板载/外引 LED 阳极 | - | 状态指示与非线性光学生物反馈 |

### 6.2 编译与固件烧录

1. 在 **Arduino IDE** 中添加 ESP8266 开发板支持包：
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
2. 安装依赖库：`MPU6050` by Electronic Cats、`I2Cdev`、`ESP8266WiFi`。
3. 打开主固件工程 `src/firmware/wearable_posture_node.ino`。
4. 开发板选择 `Generic ESP8266 Module`（或 `NodeMCU 1.0`），波特率设为 `115200`，点击 **Upload** 烧录。

### 6.3 交互与远程监控

1. 节点通电后自动创建名为 `Posture_Garment_Node` 的 Wi-Fi 热点。
2. 手机或电脑连接该 Wi-Fi（密码：`12345678`）。
3. 打开任意现代浏览器，访问默认 IP：`http://192.168.4.1`。
4. 控制台将实时渲染躯干倾仰角、侧翻角、合加速度与动态警报状态，刷新频率为 $5\text{ Hz}$。

---

## 7. 开源许可证 (License)

本项目基于 **MIT License** 开放源代码。详见项目根目录下的 [LICENSE](LICENSE) 文件。
