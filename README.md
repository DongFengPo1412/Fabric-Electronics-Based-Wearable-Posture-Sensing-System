# 基于织物电子与智能服饰的穿戴式人体姿态感知与生物反馈系统
# Fabric-Electronics-Based Wearable Posture Sensing & Wireless Biofeedback System

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](LICENSE)
[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266%20Xtensa%2032bit-red.svg?style=flat-square)](https://www.espressif.com/)
[![Sensor: MPU6050](https://img.shields.io/badge/IMU-InvenSense%20MPU6050-green.svg?style=flat-square)](https://invensense.tdk.com/)
[![Medium: E--Textile Silver Yarn](https://img.shields.io/badge/E--Textile-Silver%20Conductive%20Yarn-purple.svg?style=flat-square)](docs/)
[![Protocol: I2C 400kHz & TCP/IP](https://img.shields.io/badge/Protocol-I2C%20400kHz%20%7C%20TCP%20Socket%20%7C%20HTTP-orange.svg?style=flat-square)](src/)
[![Course: UESTC Freshman Project](https://img.shields.io/badge/UESTC-Freshman%20Project%20Course-blueviolet.svg?style=flat-square)](https://www.uestc.edu.cn/)

[**中文文档**](README.md) | [**English**](README_EN.md) | [**日本語**](README_JA.md)

</div>

---

## 1. 项目背景与学术渊源 (Academic Heritage & Background)

本项目源自**电子科技大学（UESTC）自动化工程学院**大一下学期核心实践课程——**《新生项目课：可穿戴计算系统设计》第 3 组**课程设计与科研攻关成果。

长期伏案学习与办公人群中，颈椎病、脊柱侧弯与不良体态发生率逐年攀升。现有市售体态监测设备大多基于传统刚性穿戴外壳与刚性线缆绑带，存在**皮肤贴合度差、异物感与压迫感明显、导线在人体运动反复屈伸中极易疲劳折断**等致命痛点。

针对上述挑战，本课题探索了前沿**织物电子（E-Textile / Electronic Textiles）**与**柔性可穿戴计算（Wearable Computing）**交叉技术：
- 摒弃传统硬质线束，创新性采用**微米级高导电银纤维/复合金属细导电纱线（Silver Conductive Yarn）**，将电气回路与日常全棉服饰纺织纤维深度交织绣合；
- 研发低剖面**微型圆形穿戴式 PCB 硬件节点**，沿外圆周密布环形穿线电极焊盘，实现电气信号与服饰纤维的缝合针刺固连与高可靠机械应变释放；
- 在脊柱关键动力学节段部署 **InvenSense MPU-6050 六轴惯性测量单元（IMU）**，由 **Espressif ESP8266 32 位低功耗无线微处理器**进行端侧边缘姿态解算，实现集“**人体运动捕捉—柔性织物传导—端侧滤波解算—非线性光学感知反馈—无线远程多端遥测**”于一体的智能体态闭环监护服饰系统。

---

## 2. 硬件实物与工程架构展示 (Hardware & System Showcase)

### 2.1 织物服饰实物与多节点工作状态 (Physical Wearable Artifacts)

<div align="center">

| 织物电子智能服饰与主控节点缝合实物 | 柔性双节点工作状态与针缝导电走线特写 |
| :---: | :---: |
| <img src="docs/images/demo_e_textile_garment.jpg" width="480" alt="E-Textile Garment"> | <img src="docs/images/demo_wearable_dual_node_active.png" width="480" alt="Active Wearable Nodes"> |
| **实物服饰缝绣**：圆形 ESP8266 主控与 MPU6050 传感器通过银纤维导电线刺绣嵌入全棉服饰 | **双节点发光工作状态**：锂电充放电管理节点与主控节点柔性互连，LED 实时发出生物感知光反馈 |

| 穿戴式主控节点近景特写 | 面包板前期电路验证原型 |
| :---: | :---: |
| <img src="docs/images/demo_sensing_node.jpg" width="480" alt="Sensing Node Close-up"> | <img src="docs/images/breadboard_proto_esp8266_mpu6050.png" width="480" alt="Breadboard Prototype"> |
| **主控节点近景**：板载 Micro-USB 充电/固件调试接口、状态 LED 及微型复位按键 | **前期方案验证**：ESP8266 + MPU6050 + OLED 显示屏在面包板上进行 6 轴运动学时序标定 |

</div>

### 2.2 系统全景拓扑与端云数据流 (System Architecture & Data Flow)

系统整体构建了自底层物理织物至顶层多终端可视化遥测的端到端数据闭环：

<div align="center">
<img src="docs/images/system_architecture.png" width="900" alt="End-to-End System Architecture">
</div>

1. **柔性物理感知层 (Textile & Sensing Layer)**：人体脊柱三维运动位移通过织物基底应变与 MPU-6050 硅基 MEMS 微结构转换为电容电荷变化，输出原始 16 位三轴加速度与角速度数字信号；
2. **边缘滤波解算层 (Edge Computation & Filtering Layer)**：ESP8266 运行硬件 I2C 400kHz 驱动、时域互补姿态融合算法与欧氏合加速度运动能量模型，解算出实时倾仰角（Pitch）、翻滚角（Roll）与体态异常判据；
3. **就地光学生物反馈层 (Local Biofeedback Layer)**：由微处理器内置硬件 PWM 发生器驱动状态指示灯，以符合人眼视觉特性的对数幂律呼吸节奏指示健康体态，并在伏案含胸或侧弯超限时瞬间切换至 10Hz 高频闪烁警报；
4. **无线网络与多端遥测层 (Wireless Telemetry Layer)**：ESP8266 启动 SoftAP + Station 双重网络堆栈，本地搭建实时 HTTP Web 交互门户，并通过 Socket 客户端向远程监控主机以 50Hz 频率实时串流姿态数据流。

---

## 3. 硬件电路原理与互连规范 (Hardware & Circuit Design)

<div align="center">

| ESP-12F 主控引脚与 MPU6050 织物互连拓扑 | 银纤维导电纱线走线与刺绣工艺展示 |
| :---: | :---: |
| <img src="docs/images/hardware_interconnect_schematic.png" width="480" alt="Interconnect Schematic"> | <img src="docs/images/e_textile_circuit_layer.png" width="480" alt="Conductive Yarn Embroidery"> |
| **系统织物互连**：ESP-12F 的 3V3、GND、GPIO4 (SDA)、GPIO5 (SCL) 通过导电纱线与传感器精准连通 | **织物刺绣层工艺**：高柔韧镀银长丝导电缝纫线在纺织层上精密布线，构建微阻抗柔性总线 |

</div>

### 3.1 供电与电源管理子系统 (Power Management Subsystem)

系统采用微型可充锂聚合物电池（软包电池型号 **603035**，标称电压 **3.7V**，容量 **800mAh**，尺寸仅 $37 \times 30 \times 6.0\text{ mm}$）独立供电，集成完善的充放电安全保护机制：
- **锂电恒流/恒压充电管理芯片 (TP4054)**：SOT23-5 封装，内置恒定电流/恒定电压（CC/CV）充电算法。内部充电截止门限电压精准调校在 $4.2\text{V} \pm 1\%$，充电电流上限可配置至 $1000\text{mA}$，热过载自动回退保护，具备电源输入端拔出时自动进入超低泄漏功耗休眠（静态泄漏电流仅 $2\mu\text{A}$）；
- **超低压降 RF 低噪声线性稳压芯片 (SGM2019)**：输入电压范围 $2.5\text{V} \sim 5.5\text{V}$，输出稳定 $3.3\text{V}$ 系统总线供电。在 $300\text{mA}$ 满载工况下压降（Dropout Voltage）低至 $270\text{mV}$，电源抑制比（PSRR）高达 $74\text{dB}$（@ 1kHz），输出纹波噪声低至 $30\mu\text{V}_{\text{RMS}}$，为高灵敏度 MEMS 传感器采集与 Wi-Fi 射频功放提供高纯净度稳压电源。

<div align="center">

| ESP8266 最小系统外围原理图 | MPU6050 电平转换与滤波原理图 |
| :---: | :---: |
| <img src="docs/images/arch_esp8266_chip.png" width="480" alt="ESP8266 Schematic"> | <img src="docs/images/arch_mpu6050_chip.png" width="480" alt="MPU6050 Schematic"> |
| **ESP8266 架构**：Tensilica L106 核心、复位电路上拉、GPIO0 烧录模式切换及 TTL 串口通信 | **MPU6050 架构**：L2N7002 双向 MOS 电平转换、SPX3819 稳压器与多级高频去耦滤波网络 |

</div>

### 3.2 边缘主控计算子系统 (ESP8266 Main Controller)

主控选用乐鑫 **ESP8266 (ESP-12F 模块)**，板载 Tensilica Xtensa 32 位 L106 单核处理器，运行主频达 $80\text{MHz}$（可锁相环倍频至 $160\text{MHz}$），具备以下核心电路特性：
- **自举启动逻辑配置**：`REST` 与 `CH_PD`（芯片使能）通过 $1\text{k}\Omega$ 电阻上拉至 3.3V；`GPIO15` 经下拉电阻接地，`GPIO0` 经按键选择接地（烧录模式）或上拉高电平（Flash 正常运行自举模式）；
- **高集成射频链路**：集成巴伦（Balun）、阻抗匹配网络、低噪声放大器（LNA）及板载 PCB 倒 F 天线，满足 IEEE 802.11 b/g/n 协议标准，具备最高 $+19.5\text{dBm}$（802.11b）的发射功率。

### 3.3 惯性测量与信号调理子系统 (MPU-6050 IMU Subsystem)

- **MEMS 核心检测机构**：内置独立三轴加速度计与三轴角速度陀螺仪，均采用片上 16 位高精 ADC，通过片内 FIFO 缓冲池连续缓存运动学采样数据；
- **双向 I2C 接口电平转换设计**：模块板载双路 N 沟道 MOSFET（`L2N7002LT1G`）及 $4.7\text{k}\Omega$ 上拉电阻，隔离外部 $5\text{V}$ 兼容输入并向 MPU-6050 核心提供纯净 $3.3\text{V}$ I2C 逻辑电平，确保高速 $400\text{kHz}$ 信号传输波形上升沿陡峭无毛刺；
- **高频去耦与防抖网络**：针对运动状态下可能产生的瞬态抖动，在 `VDD`、`VLOGIC` 与 `CPOUT` 引脚就近配置 $0.1\mu\text{F}$（104）及 $2.2\mu\text{F}$ 低 ESR 陶瓷旁路电容。

---

## 4. 核心数学模型与生物力学算法推导 (Mathematical Formulations)

本项目在微控制器端与遥测上位机实现了严格的物理力学建模，涵盖柔性微应变电学响应、空间姿态角估计、三维运动能量量化以及人眼感知非线性反馈律。

<div align="center">
<img src="docs/images/algorithm_6dof_motion_trajectory.png" width="750" alt="6-DOF Motion Trajectory Reconstruction">
<p><em>图：基于 6 自由度惯性传感器融合算法的人体运动姿态 3D 轨迹空间重建与动态姿态追踪</em></p>
</div>

### 4.1 导电纱线压阻应变模型与电桥信号调理方程

柔性导电纱线缝合于织物表面随躯干拉伸发生几何形变与纤维微观接触电阻变化。设未拉伸状态初始电阻为 $R_0$，应变量为 $\varepsilon = \frac{\Delta L}{L_0}$，纱线灵敏度系数（Gauge Factor）为 $G_F$，则等效阻值数学模型满足：

$$
R(\varepsilon) = R_0 (1 + G_F \cdot \varepsilon)
$$

在微小应变采集电路中，采用对称惠斯通电桥进行非线性差分放大调理，输出差分电压方程表示为：

$$
V_{\text{out}} = V_{\text{in}} \left( \frac{R_2}{R_1 + R_2} - \frac{R_x(\varepsilon)}{R_3 + R_x(\varepsilon)} \right)
$$

当满足全桥平衡基准 $R_1 = R_2 = R_3 = R_0$ 时，输出电压关于机械微应变的一阶线性化响应为：

$$
V_{\text{out}} \approx \frac{V_{\text{in}}}{4} \cdot G_F \cdot \varepsilon
$$

### 4.2 基于静态重力矢量的空间欧拉姿态角解算模型

设三轴 MEMS 加速度计测得的归一化重力加速度分量为向量 $\mathbf{a} = [a_x, a_y, a_z]^{\top}$。根据刚体运动学投影原理，定义躯干空间倾仰角（Pitch Angle）为 $\theta$，侧向翻滚角（Roll Angle）为 $\phi$。

倾仰角 $\theta$（反映人体俯仰/含胸驼背程度）：

$$
\theta = \arctan \left( \frac{-a_x}{\sqrt{a_y^2 + a_z^2}} \right) \cdot \frac{180^\circ}{\pi}
$$

翻滚角 $\phi$（反映人体侧向倾斜/脊柱侧弯不对称度）：

$$
\phi = \arctan \left( \frac{a_y}{a_z} \right) \cdot \frac{180^\circ}{\pi}
$$

### 4.3 加速度计与陀螺仪时域离散互补姿态融合滤波

加速度计对低频静态倾角响应极准，但在人体运动走跑时极易受高频震颤振动干扰；陀螺仪角速度积分短时动态响应优异，但存在积分漂移误差发散。系统在微控制器内部部署时域一阶离散互补滤波器（Complementary Filter）：

设采样周期为 $\Delta t$，陀螺仪测得绕轴角速度分量为 $\omega_{\text{gyro}}$，由加速度计几何三角学解算的瞬时静态角为 $\theta_{\text{acc}}$，系统截止时间常数为 $\tau$：

$$
\theta_t = \alpha (\theta_{t-1} + \omega_{\text{gyro}} \cdot \Delta t) + (1 - \alpha) \theta_{\text{acc}}
$$

其中平滑衰减增益因子 $\alpha$ 满足高通与低通滤波器的时间常数映射关系：

$$
\alpha = \frac{\tau}{\tau + \Delta t}, \quad (0.95 \le \alpha \le 0.98)
$$

该滤波器在微控制器端以 $100\text{Hz}$ 周期执行，计算开销极低（仅需 2 次浮点乘法与 2 次浮点加法），有效根除了纯积分角度漂移与高频肌肉颤动噪声。

### 4.4 三维欧氏加速度范数与人体运动动能度量模型

为动态区分静态体态保持与剧烈步行动作，计算合加速度矢量的欧几里得范数 $\|\mathbf{a}\|$：

$$
\|\mathbf{a}\| = \sqrt{a_x^2 + a_y^2 + a_z^2}
$$

在动态运动时间滑动窗口 $t \in [t_0, t_0 + T]$ 内，剔除静态重力常数矢量 $\mathbf{g}$ 后的躯干动力学微元动能积分为：

$$
E_{\text{kinetic}} = \frac{1}{2} m \int_{t_0}^{t_0 + T} \left\| \mathbf{a}(t) - \mathbf{g} \right\|^2 \, dt
$$

**体态判定判决律**：
- 当满足 $|\|\mathbf{a}\| - 1.0g| \le \delta$（准静态区间，$\delta = 0.15g$）时，表明人体处于坐立等平稳状态，系统激活高灵敏度体态异常判决逻辑：若 $|\theta| > 30^\circ$ 或 $|\phi| > 35^\circ$，判定为“**体态不良（SLUMP）**”；
- 当动态合加速度显著偏离 $1.0g$ 时，判定人体处于走动或剧烈活动状态，系统自动抑制体态报警，有效避免误触发。

### 4.5 韦伯-费希纳非线性视觉呼吸生物反馈控制律

为人眼提供舒适、柔和的生物体感光反馈，状态 LED 的驱动占空比 $D(t)$ 基于人眼视觉对数感应特性（Weber-Fechner Law）进行幂律感知矫正：

$$
D(t) = D_{\min} + (D_{\max} - D_{\min}) \cdot \left[ \frac{1}{2} \left( 1 + \sin \left( \frac{2\pi t}{T_{\text{breath}}} - \frac{\pi}{2} \right) \right) \right]^\gamma
$$

其中：
- 呼吸仿真调制周期 $T_{\text{breath}} = 3.0\text{ s}$；
- 视觉非线性心理物理矫正指数取 $\gamma = 2.2$；
- 占空比极值区间 $D_{\min} = 0, D_{\max} = 1023$（10 位硬件 PWM 分辨率）。

当判定体态异常持续超过设定时间窗口时，系统立即中断呼吸模式，瞬间跃迁至占空比 $50\%$、$f = 10\text{Hz}$ 的高频频闪强提示模式，提醒穿戴者矫正坐姿。

---

## 5. 交互软件与网络遥测 (Software & Telemetry Systems)

系统提供了双重实时监测途径：基于嵌入式 Web 服务的轻量级移动端控制门户，以及基于串口与 TCP Socket 的高频遥测数据流。

<div align="center">

| 嵌入式 Web 控制交互门户界面 | 串口高精度三轴加速度实时遥测数据流 |
| :---: | :---: |
| <img src="docs/images/ui_web_portal_init.png" width="480" alt="Web Portal Initial"> | <img src="docs/images/telemetry_serial_output.png" width="480" alt="Serial Telemetry Output"> |
| **移动端 Web 控制**：手机直连 ESP8266 AP 热点（192.168.147.81），提供开灯、关灯及呼吸模式远程下发 | **底层数据流监视**：COM3 串口以 115200 波特率高速输出实时计算的三轴加速度分量 |

</div>

- **轻量化移动端 Web 门户**：固件内置基于 `ESP8266WebServer` 的轻量 HTTP 服务器，用户使用手机或平板连接 `Posture_Garment_Node` 无线热点后，即可在浏览器直接交互并查看实时姿态角；
- **高频遥测串流协议**：微控制器通过 `WiFiClient` 建立与宿主主机的 TCP Socket 链路，以固定数据包协议以 50Hz 速率连续回传：
  ```text
  PKT:P=<pitch>,R=<roll>,A=<accel_magnitude>\n
  ```

---

## 6. 系统规格技术指标对照表 (System Specifications)

| 子系统维度 | 技术指标选型 | 详细参数与性能表现 |
| :--- | :--- | :--- |
| **主控微处理器** | Espressif ESP8266 (ESP-12F) | Tensilica 32-bit LX106 核心，主频 80MHz / 160MHz，4MB SPI Flash |
| **惯性传感单元** | InvenSense MPU-6050 | 3 轴加速度计（$\pm 2g$, 16384 LSB/g）+ 3 轴陀螺仪（$\pm 250^\circ/\text{s}$），16 位内置 ADC |
| **通信总线与时钟** | Hardware I2C Fast-Mode | 运行频率 $f_{\text{SCL}} = 400\text{ kHz}$，传感器轮询采样周期 $10\text{ ms}$（100Hz） |
| **导电织物互连** | 微米级复合镀银/不锈钢导电纱线 | 纱线直径 $\approx 0.2\text{ mm}$，常态线性电阻率 $< 1.5\,\Omega/\text{cm}$，耐水洗及反复弯折 |
| **穿戴硬件结构** | 边缘缝合多孔圆形 PCB | 双层 FR-4 环形电极设计，外圈环绕导电缝合针孔，分散机械形变应力 |
| **电源与充电系统** | TP4054 + SGM2019 LDO | 3.7V 800mAh 锂聚合物软包电池，4.2V CC/CV 恒流充，3.3V 稳压输出（PSRR 74dB） |
| **无线通信网络** | 802.11 b/g/n Wi-Fi | 具备 SoftAP + Station 双模式，内置 HTTP Web 门户与底层 TCP 遥测客户端 |
| **本地生物反馈** | 10 位非线性硬件 PWM | 分辨率 $0 \sim 1023$，支持仿生呼吸光模式与 10Hz 频闪异常坐姿声光告警 |
| **端侧解算精度** | 离散一阶互补滤波 | 倾斜角度分辨率 $< 0.1^\circ$，静态精度 $\pm 1.0^\circ$，动态跟踪响应延迟 $< 15\text{ ms}$ |

---

## 7. 仓库代码与文件目录索引 (Repository Layout)

```text
Fabric-Electronics-Based-Wearable-Posture-Sensing-System/
├── docs/
│   └── images/
│       ├── system_architecture.png              # 穿戴式姿态感知系统端到端拓扑架构图
│       ├── demo_e_textile_garment.jpg           # 织物服饰实物走线与主控节点高清实拍图 (1920x1280)
│       ├── demo_wearable_dual_node_active.png   # 双节点工作状态、点亮 LED 与导电纱线走线特写
│       ├── demo_sensing_node.jpg                # 穿戴主控节点近景及 Micro-USB 接口实拍 (1920x1280)
│       ├── breadboard_proto_esp8266_mpu6050.png # 面包板前期电路验证原型图
│       ├── hardware_interconnect_schematic.png  # ESP8266 与 MPU6050 引脚互连走线拓扑图
│       ├── arch_esp8266_chip.png                # ESP-12F 芯片外围与烧录模式电路原理图
│       ├── arch_mpu6050_chip.png                # MPU-6050 电平转换与滤波去耦电路原理图
│       ├── e_textile_circuit_layer.png          # 银纤维导电纱线纺织与刺绣工艺展示图
│       ├── algorithm_6dof_motion_trajectory.png # 6-DOF 惯性人体运动姿态 3D 轨迹重构图
│       ├── ui_web_portal_init.png               # ESP8266 本地 Web 控制交互门户初始界面
│       ├── ui_web_posture_telemetry.png         # Web 姿态状态反馈控制响应页面
│       └── telemetry_serial_output.png          # COM3 串口 3 轴加速度实时遥测数据输出
├── src/
│   └── firmware/
│       ├── wearable_posture_node.ino            # 综合量产级嵌入式固件 (I2C+姿态互补滤波+Web+TCP)
│       └── examples/
│           ├── sketch_mar26a_tcp_imu.ino        # 基础 TCP 客户端通信与 MPU6050 初始化例程
│           ├── sketch_mar19a2_pwm.ino           # 基础非线性呼吸 PWM 调制实验例程
│           └── sketch_mar12a1_blink.ino         # 硬件引脚连通性与 GPIO 点灯测试例程
├── LICENSE                                      # MIT 官方开源许可协议
├── README.md                                    # 中文综合技术工程文档与数学模型推导
├── README_EN.md                                 # English Comprehensive Engineering Specification
└── README_JA.md                                 # 日本語技術仕様書・学術ポートフォリオ
```

---

## 8. 快速部署与使用指南 (Quick Start Guide)

### 8.1 硬件引脚接线与织物缝合分配 (Pinout Mapping)

| ESP8266 (ESP-12F) 引脚 | 织物刺绣分配 | MPU-6050 引脚 | 电气功能与信号流向 |
| :---: | :---: | :---: | :---: |
| **3V3** | 导电纱线 VCC 总线 | **VCC** | 3.3V 纯净系统稳压供电 |
| **GND** | 导电纱线 GND 总线 | **GND** | 信号参考地与电源回路 |
| **GPIO4 (D2)** | 导电纱线 SDA 数据线 | **SDA** | I2C 双向串行数据总线 ($400\text{kHz}$) |
| **GPIO5 (D1)** | 导电纱线 SCL 时钟线 | **SCL** | I2C 同步串行时钟总线 |
| **GPIO2 (D4)** | 板载/外引 LED 阴极 | - | 状态指示与非线性光学生物反馈输出 (Active LOW) |

### 8.2 工具链安装与固件编译烧录

1. 安装最新版 **Arduino IDE**（推荐 2.x 以上版本）；
2. 打开 `文件` -> `首选项`，在“附加开发板管理器网址”中填入 ESP8266 官方支持包：
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. 打开 `工具` -> `开发板管理器`，搜索并安装 `esp8266`（版本 3.0.0 以上）；
4. 打开 `工具` -> `管理库`，安装依赖库：
   - `MPU6050` by Electronic Cats
   - `I2Cdev` by Jeff Rowberg
   - `ESP8266WiFi` 与 `ESP8266WebServer`（ESP8266 核心库已自带）
5. 打开主固件工程文件 [wearable_posture_node.ino](src/firmware/wearable_posture_node.ino)；
6. 在 `工具` 菜单中选择：
   - **开发板 (Board)**: `Generic ESP8266 Module` (或 `NodeMCU 1.0`)
   - **CPU Frequency**: `80 MHz` (可按需设为 `160 MHz`)
   - **Flash Size**: `4MB (FS:2MB OTA:~1019KB)`
   - **Upload Speed**: `115200`
7. 将 ESP8266 的 `GPIO0` 接地，按下 `RESET` 按键进入下载自举模式，点击 **Upload** 烧录固件。

### 8.3 系统运行与无线交互测试

1. 烧录完成后断开 `GPIO0` 与地连接，按下 `RESET` 启动系统；
2. 打开串口监视器（波特率 `115200`），将打印初始化信息与 I2C 自检结果：
   ```text
   [INIT] E-Textile Wearable Posture Sensing Node Booting...
   [IMU] Initializing InvenSense MPU6050...
   [IMU] MPU6050 connection SUCCESSFUL
   [WIFI] SoftAP IP: 192.168.4.1
   [HTTP] Web telemetry server started on port 80
   ```
3. **连接无线热点**：使用智能手机或电脑 Wi-Fi 搜索热点 `Posture_Garment_Node`，输入预设密码 `12345678`；
4. **访问仪表盘**：在浏览器地址栏输入默认网关 `http://192.168.4.1`，即可看到实时更新的倾角（Pitch）、翻滚角（Roll）、合加速度及体态正常/异常指示；
5. **体验生物反馈**：保持正常挺胸直立坐姿，板载 LED 呈现舒缓的平稳呼吸节奏；当身体向前倾斜驼背超过 $30^\circ$ 阈值时，LED 立即切换为 $10\text{Hz}$ 急促闪烁警报，提示端正坐姿。

---

## 9. 团队与学术致谢 (Credits & Acknowledgments)

- **开发团队**：电子科技大学自动化工程学院 2023 级本科生第 3 组
  - 核心成员：学号 2023060904023、2023060904028、2023060904025
- **指导教师**：电子科技大学自动化工程学院《新生项目课：可穿戴计算系统设计》教学团队
- **开源致谢**：感谢 Electronic Cats 与 Jeff Rowberg 提供的开源 I2Cdevlib / MPU6050 运动学函数库支持。

---

## 10. 开源许可证 (License)

本项目代码及技术文档均在 **MIT License** 许可下开源发布。完整协议条款请参阅项目根目录下的 [LICENSE](LICENSE) 文件。
