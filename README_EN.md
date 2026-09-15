# Fabric-Electronics-Based Wearable Posture Sensing & Wireless Biofeedback System
# 基于织物电子与智能服饰的穿戴式人体姿态感知与生物反馈系统

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

## 1. Academic Heritage & Project Background

This project originated from the freshman engineering capstone research project—**"Freshman Project Course: Wearable Computing System Design" (Group 03)**—at the **School of Automation Engineering, University of Electronic Science and Technology of China (UESTC)**.

Among sedentary students and desk-bound office professionals, the prevalence of cervical spondylosis, idiopathic scoliosis, and chronic posture-induced back strain is rising sharply. Conventional commercial posture monitoring gadgets rely predominantly on rigid plastic shells, stiff elastic straps, and discrete copper wiring harnesses. These architectures exhibit severe shortcomings: **poor dermal compliance, high foreign-body sensation and pressure sores, as well as rapid mechanical fatigue and conductor fractures under continuous torso movement**.

To solve these fundamental dilemmas, this research integrates cutting-edge **Electronic Textiles (E-Textiles / Smart Fabrics)** with **Edge Wearable Computing**:
- Eliminating rigid discrete harnesses by adopting **micro-diameter silver-coated / stainless-steel hybrid conductive yarn**, seamlessly interweaving low-impedance electrical buses directly into standard cotton fabric via precision embroidery;
- Engineering low-profile **circular wearable PCB nodes**, bordered with concentric annular stitch pads to facilitate needle-stitched mechanical fastening and effective mechanical strain relief;
- Mounting an **InvenSense MPU-6050 6-axis Inertial Measurement Unit (IMU)** along key biomechanical spinal segments, with on-node kinematic edge estimation executed by an **Espressif ESP8266 32-bit Wi-Fi micro-controller**;
- Implementing a complete closed-loop wearable system comprising **"Torso Motion Capture — Flexible Textile Conduction — Edge Complementary Fusion — Non-linear Visual Biofeedback — Multi-terminal Remote Telemetry"**.

---

## 2. Hardware & System Showcase

### 2.1 Physical Wearable Artifacts & Dual-Node Operation

<div align="center">

| E-Textile Garment & Circular Main Node Physical Stitching | Dual-Node Glowing State & Conductive Thread Routing Close-up |
| :---: | :---: |
| <img src="docs/images/demo_e_textile_garment.jpg" width="480" alt="E-Textile Garment"> | <img src="docs/images/demo_wearable_dual_node_active.png" width="480" alt="Active Wearable Nodes"> |
| **Garment Integration**: Circular ESP8266 node and MPU-6050 module embroidered into cotton fabric via silver conductive threads | **Active Dual Nodes**: Li-ion power management node interconnected with main controller node, glowing LEDs rendering real-time biofeedback |

| Wearable Sensing Node Close-Up | Breadboard Pre-textile Prototyping |
| :---: | :---: |
| <img src="docs/images/demo_sensing_node.jpg" width="480" alt="Sensing Node Close-up"> | <img src="docs/images/breadboard_proto_esp8266_mpu6050.png" width="480" alt="Breadboard Prototype"> |
| **Node Close-up**: Onboard Micro-USB charging/debug port, status LED, and miniature tactile buttons | **Pre-embroidery Validation**: ESP8266 + MPU6050 + OLED display breadboard verification for 6-axis kinematic timing |

</div>

### 2.2 System Architecture & End-to-End Data Flow

The platform forms an end-to-end data pipeline spanning physical textile sensing to cloud/browser telemetry:

<div align="center">
<img src="docs/images/system_architecture.png" width="900" alt="End-to-End System Architecture">
</div>

1. **Flexible Textile & Sensing Layer**: 3D biomechanical torso motion flexes the fabric substrate and modulates the internal capacitive MEMS structures of the MPU-6050, digitizing 16-bit tri-axial linear acceleration and angular velocity signals;
2. **Edge Computation & Filtering Layer**: The ESP8266 controller executes hardware I2C at 400kHz, runs discrete time-domain complementary attitude fusion, and computes Euclidean acceleration norms to assess posture deviations;
3. **Local Visual Biofeedback Layer**: The integrated hardware PWM engine drives the status LED with logarithmic Weber-Fechner respiratory rhythm during correct posture, switching instantaneously to a 10 Hz stroboscopic strobe upon prolonged slump detection;
4. **Wireless Network & Telemetry Layer**: The controller deploys concurrent SoftAP + Station Wi-Fi stacks, serving a lightweight embedded HTTP portal and streaming continuous kinematic packets to host stations via TCP Sockets at 50 Hz.

---

## 3. Hardware Circuit Design & Interconnect Topology

<div align="center">

| ESP-12F Pinout & MPU-6050 Textile Interconnect Schematic | Silver Conductive Yarn Embroidery Process |
| :---: | :---: |
| <img src="docs/images/hardware_interconnect_schematic.png" width="480" alt="Interconnect Schematic"> | <img src="docs/images/e_textile_circuit_layer.png" width="480" alt="Conductive Yarn Embroidery"> |
| **Textile Wiring Topology**: 3V3, GND, GPIO4 (SDA), and GPIO5 (SCL) routed through embroidered conductive traces | **E-Textile Craftsmanship**: High-flexibility silver-plated filaments sewn into textile matrix to form low-impedance flexible buses |

</div>

### 3.1 Power Management Subsystem

The garment is powered independently by a compact rechargeable lithium-polymer pouch cell (**Model 603035**, nominal voltage **3.7V**, capacity **800mAh**, dimensions $37 \times 30 \times 6.0\text{ mm}$), supported by comprehensive power protection:
- **Linear Li-ion CC/CV Charge Management IC (TP4054)**: SOT23-5 package, featuring autonomous constant-current / constant-voltage charging algorithms. The internal float voltage threshold is calibrated to $4.2\text{V} \pm 1\%$, supporting up to $1000\text{mA}$ charging current, thermal foldback regulation, and ultra-low sleep drain ($2\mu\text{A}$) when disconnected from input power;
- **Ultra-Low Dropout RF Low-Noise LDO Regulator (SGM2019)**: Input voltage range $2.5\text{V} \sim 5.5\text{V}$, regulating a clean $3.3\text{V}$ system bus. Operates with an ultra-low dropout voltage of $270\text{mV}$ under full $300\text{mA}$ load, exceptional power supply rejection ratio ($74\text{dB}$ PSRR @ 1kHz), and low output noise ($30\mu\text{V}_{\text{RMS}}$), guaranteeing stable power for high-sensitivity MEMS acquisition and Wi-Fi RF bursts.

<div align="center">

| ESP8266 Minimal System Schematic | MPU-6050 Level-Shifter & Filtering Circuit |
| :---: | :---: |
| <img src="docs/images/arch_esp8266_chip.png" width="480" alt="ESP8266 Schematic"> | <img src="docs/images/arch_mpu6050_chip.png" width="480" alt="MPU6050 Schematic"> |
| **ESP8266 Circuit**: Tensilica L106 core, reset pull-ups, GPIO0 boot mode strap, and UART programming lines | **MPU6050 Circuit**: L2N7002 bidirectional MOSFET level translation, SPX3819 regulator, and multi-stage decoupling |

</div>

### 3.2 Main Controller Subsystem (ESP-12F)

The core computation node incorporates the **Espressif ESP8266 (ESP-12F module)**, based on the 32-bit Tensilica Xtensa L106 single-core processor running at $80\text{MHz}$ (configurable up to $160\text{MHz}$):
- **Boot Configuration Strapping**: Pins `REST` and `CH_PD` are pulled high via $1\text{k}\Omega$ resistors; `GPIO15` is pulled down to GND; `GPIO0` is switch-selectable between GND (UART download mode) and HIGH (normal SPI Flash booting);
- **RF Wireless Front-End**: Monolithic integrated balun, impedance-matching network, low-noise amplifier (LNA), and inverted-F PCB trace antenna conforming to IEEE 802.11 b/g/n with up to $+19.5\text{dBm}$ output power in 802.11b mode.

### 3.3 Inertial Measurement Subsystem (InvenSense MPU-6050)

- **MEMS Transducer**: Incorporates an integrated tri-axial accelerometer and tri-axial gyroscope, both sampled by internal 16-bit analog-to-digital converters (ADCs) buffering data continuously into an internal FIFO pool;
- **Bidirectional I2C Level Shifting**: Dual N-channel enhancement MOSFETs (`L2N7002LT1G`) with $4.7\text{k}\Omega$ pull-up resistors provide logic voltage isolation and crisp signal edges for high-speed $400\text{kHz}$ I2C transmission;
- **Transient Decoupling & Stability Network**: Low-ESR ceramic bypass capacitors ($0.1\mu\text{F}$ and $2.2\mu\text{F}$) positioned immediately adjacent to `VDD`, `VLOGIC`, and charge-pump `CPOUT` suppress voltage ripple caused by mechanical impacts.

---

## 4. Mathematical Formulations & Biomechanical Kinematic Modeling

The system implements rigorous analytical biomechanical and kinematic equations across edge firmware and host telemetric stations:

<div align="center">
<img src="docs/images/algorithm_6dof_motion_trajectory.png" width="750" alt="6-DOF Motion Trajectory Reconstruction">
<p><em>Figure: 3D kinematic trajectory reconstruction and spatial orientation tracking derived from 6-DOF inertial sensor fusion</em></p>
</div>

### 4.1 Conductive Yarn Piezoresistive Strain Model & Wheatstone Bridge Equation

When conductive yarns embroidered into the garment stretch under torso flexing, geometric elongation and micro-contact alterations modify yarn resistance. For initial unstretched resistance $R_0$, mechanical strain $\varepsilon = \frac{\Delta L}{L_0}$, and gauge factor $G_F$:

$$
R(\varepsilon) = R_0 (1 + G_F \cdot \varepsilon)
$$

For strain signal conditioning, a symmetrical Wheatstone bridge is employed. The differential output voltage satisfies:

$$
V_{\text{out}} = V_{\text{in}} \left( \frac{R_2}{R_1 + R_2} - \frac{R_x(\varepsilon)}{R_3 + R_x(\varepsilon)} \right)
$$

Under the baseline condition $R_1 = R_2 = R_3 = R_0$, the first-order linearized output voltage with respect to mechanical strain reduces to:

$$
V_{\text{out}} \approx \frac{V_{\text{in}}}{4} \cdot G_F \cdot \varepsilon
$$

### 4.2 Tri-axial Gravitational Vector Decomposition & Spatial Euler Posture Angles

Let the normalized gravitational acceleration vector acquired by the tri-axial accelerometer be $\mathbf{a} = [a_x, a_y, a_z]^{\top}$. In rigid-body kinematics, spatial Pitch angle $\theta$ (anterior-posterior tilt) and Roll angle $\phi$ (lateral scoliosis tilt) are given by:

Pitch Angle $\theta$ (cervical/thoracic hunchback slumping):

$$
\theta = \arctan \left( \frac{-a_x}{\sqrt{a_y^2 + a_z^2}} \right) \cdot \frac{180^\circ}{\pi}
$$

Roll Angle $\phi$ (lateral spinal tilting / postural asymmetry):

$$
\phi = \arctan \left( \frac{a_y}{a_z} \right) \cdot \frac{180^\circ}{\pi}
$$

### 4.3 Discrete Time-Domain Complementary Attitude Fusion Filter

While accelerometers exhibit zero low-frequency drift, they suffer from dynamic motion artifacts and muscle tremor. Conversely, gyroscopes provide excellent dynamic response but suffer from unbounded integration drift. The firmware implements a discrete-time first-order complementary filter:

For sampling period $\Delta t$, measured angular velocity $\omega_{\text{gyro}}$, instantaneous trigonometric accelerometer angle $\theta_{\text{acc}}$, and filter time constant $\tau$:

$$
\theta_t = \alpha (\theta_{t-1} + \omega_{\text{gyro}} \cdot \Delta t) + (1 - \alpha) \theta_{\text{acc}}
$$

where the smoothing gain factor $\alpha$ satisfies:

$$
\alpha = \frac{\tau}{\tau + \Delta t}, \quad (0.95 \le \alpha \le 0.98)
$$

Executed on the microcontroller at $100\text{Hz}$, this filter consumes minimal computational overhead (only 2 floating-point multiplications and 2 additions) while effectively eliminating gyro drift and high-frequency tremor.

### 4.4 Euclidean Acceleration Norm & Human Kinetic Disturbance Energy

To distinguish static postural slumping from active walking or jogging, the Euclidean norm of total acceleration $\|\mathbf{a}\|$ is monitored:

$$
\|\mathbf{a}\| = \sqrt{a_x^2 + a_y^2 + a_z^2}
$$

Within a sliding time window $t \in [t_0, t_0 + T]$, removing the static gravity vector $\mathbf{g}$ yields the torso kinetic disturbance energy:

$$
E_{\text{kinetic}} = \frac{1}{2} m \int_{t_0}^{t_0 + T} \left\| \mathbf{a}(t) - \mathbf{g} \right\|^2 \, dt
$$

**Posture Decision Logic**:
- When $|\|\mathbf{a}\| - 1.0g| \le \delta$ (quasi-static threshold with $\delta = 0.15g$), the wearer is seated or standing steadily. The high-sensitivity posture classification logic engages: if $|\theta| > 30^\circ$ or $|\phi| > 35^\circ$, an alert status (`ALERT: POSTURE SLUMP`) triggers;
- When dynamic acceleration deviates significantly from $1.0g$, the system infers active ambulation and temporarily suppresses posture alarms, suppressing false positives.

### 4.5 Non-linear Weber-Fechner Law Visual Respiratory Biofeedback

To provide natural, non-intrusive sensory guidance, the PWM duty cycle $D(t)$ driving the feedback LED is corrected via the psychophysical Weber-Fechner power law:

$$
D(t) = D_{\min} + (D_{\max} - D_{\min}) \cdot \left[ \frac{1}{2} \left( 1 + \sin \left( \frac{2\pi t}{T_{\text{breath}}} - \frac{\pi}{2} \right) \right) \right]^\gamma
$$

where:
- Respiratory cycle period $T_{\text{breath}} = 3.0\text{ s}$;
- Perceptual non-linear gamma exponent $\gamma = 2.2$;
- Duty cycle limits $D_{\min} = 0, D_{\max} = 1023$ (10-bit hardware PWM resolution).

Upon detecting persistent abnormal posture exceeding a dwell window, the soothing respiratory rhythm transitions instantly into a stroboscopic warning pulse ($50\%$ duty cycle, $f = 10\text{Hz}$).

---

## 5. Software & Wireless Telemetry Systems

The platform provides dual real-time monitoring streams: a zero-install embedded mobile Web portal and a high-frequency serial/socket telemetry channel.

<div align="center">

| Embedded Web Control Dashboard | COM3 Serial High-Precision Tri-Axial Acceleration Telemetry |
| :---: | :---: |
| <img src="docs/images/ui_web_portal_init.png" width="480" alt="Web Portal Initial"> | <img src="docs/images/telemetry_serial_output.png" width="480" alt="Serial Telemetry Output"> |
| **Mobile Web GUI**: Direct browser connection to ESP8266 AP (192.168.147.81) for real-time mode toggling | **Serial Stream**: Continuous COM3 telemetry at 115200 baud streaming calibrated 3-axis acceleration components |

</div>

- **Lightweight Mobile Web Portal**: The firmware embeds an HTTP server via `ESP8266WebServer`. Clients connecting to the `Posture_Garment_Node` Wi-Fi network can view live pitch, roll, and posture status directly on any modern browser;
- **TCP Telemetry Stream**: The node opens a TCP socket connection via `WiFiClient` to a host computer, streaming kinematic packets at 50 Hz:
  ```text
  PKT:P=<pitch>,R=<roll>,A=<accel_magnitude>\n
  ```

---

## 6. System Technical Specifications Comparison

| Subsystem Dimension | Hardware / Technology Selection | Detailed Specifications & Performance Metrics |
| :--- | :--- | :--- |
| **Main Processor (MCU)** | Espressif ESP8266 (ESP-12F) | Tensilica 32-bit LX106 core @ 80MHz / 160MHz, 4MB SPI Flash |
| **Inertial Measurement Unit** | InvenSense MPU-6050 | 3-axis accelerometer ($\pm 2g$, 16384 LSB/g) + 3-axis gyro ($\pm 250^\circ/\text{s}$), 16-bit ADCs |
| **Bus Interface & Rate** | Hardware I2C Fast-Mode | Clock frequency $f_{\text{SCL}} = 400\text{ kHz}$, sensor polling period $10\text{ ms}$ (100Hz) |
| **Conductive Textile Bus** | Silver-coated hybrid conductive yarn | Diameter $\approx 0.2\text{ mm}$, linear resistance $< 1.5\,\Omega/\text{cm}$, washable & fatigue-resistant |
| **Node Physical Design** | Circular multi-pad wearable PCB | Circular FR-4 PCB with perimeter annular stitch pads for needle-thread strain relief |
| **Power Management** | TP4054 CC/CV + SGM2019 LDO | 3.7V 800mAh LiPo cell, 4.2V CC/CV charging, 3.3V RF-clean LDO output (74dB PSRR) |
| **Wireless Networking** | 802.11 b/g/n Wi-Fi | SoftAP + Station dual-mode, embedded HTTP Web server and TCP telemetry client |
| **Local Biofeedback** | 10-bit non-linear hardware PWM | Resolution $0 \sim 1023$, soothing respiratory feedback + 10Hz stroboscopic posture alarm |
| **Attitude Fusion Accuracy** | Discrete 1st-order complementary filter | Angular resolution $< 0.1^\circ$, static accuracy $\pm 1.0^\circ$, dynamic response lag $< 15\text{ ms}$ |

---

## 7. Repository Layout & File Manifest

```text
Fabric-Electronics-Based-Wearable-Posture-Sensing-System/
├── docs/
│   └── images/
│       ├── system_architecture.png              # End-to-end wearable system architecture and dataflow diagram
│       ├── demo_e_textile_garment.jpg           # High-resolution photo of garment embroidered node (1920x1280)
│       ├── demo_wearable_dual_node_active.png   # Active dual-node prototype with illuminated LEDs and stitch routing
│       ├── demo_sensing_node.jpg                # Close-up photograph of sensing node with Micro-USB interface (1920x1280)
│       ├── breadboard_proto_esp8266_mpu6050.png # Pre-textile breadboard validation circuit prototype
│       ├── hardware_interconnect_schematic.png  # Detailed pin-to-pin textile wiring topology diagram
│       ├── arch_esp8266_chip.png                # ESP-12F minimal circuit schematic and boot mode wiring
│       ├── arch_mpu6050_chip.png                # MPU-6050 level shifter and decoupling schematic
│       ├── e_textile_circuit_layer.png          # Silver conductive yarn embroidery process demonstration
│       ├── algorithm_6dof_motion_trajectory.png # 6-DOF 3D motion trajectory and posture tracking visualization
│       ├── ui_web_portal_init.png               # Embedded ESP8266 Web control portal initial screen
│       ├── ui_web_posture_telemetry.png         # Web portal posture telemetry and mode feedback screen
│       └── telemetry_serial_output.png          # Live COM3 serial telemetry monitor output
├── src/
│   └── firmware/
│       ├── wearable_posture_node.ino            # Production firmware (I2C + Attitude Fusion + Web + TCP)
│       └── examples/
│           ├── sketch_mar26a_tcp_imu.ino        # Basic TCP client telemetry & MPU6050 initialization routine
│           ├── sketch_mar19a2_pwm.ino           # Basic non-linear respiratory PWM modulation routine
│           └── sketch_mar12a1_blink.ino         # Hardware connectivity & GPIO blink test routine
├── LICENSE                                      # MIT Official Open-Source License
├── README.md                                    # Chinese Engineering & Mathematical Documentation
├── README_EN.md                                 # English Comprehensive Engineering Specification
└── README_JA.md                                 # Japanese Technical Specification & Portfolio
```

---

## 8. Quick Start Guide

### 8.1 Pin Mapping & Textile Stitching Connections

| ESP8266 Pin | Textile Bus Trace | MPU-6050 Pin | Functional Definition |
| :---: | :---: | :---: | :---: |
| **3V3** | Conductive Yarn VCC Bus | **VCC** | 3.3V Clean Regulated Power Supply |
| **GND** | Conductive Yarn GND Bus | **GND** | Signal Ground & Power Return Path |
| **GPIO4 (D2)** | Conductive Yarn SDA Bus | **SDA** | I2C Bidirectional Serial Data ($400\text{kHz}$) |
| **GPIO5 (D1)** | Conductive Yarn SCL Bus | **SCL** | I2C Serial Clock Synchronization Bus |
| **GPIO2 (D4)** | Onboard / E-Textile LED | - | Biofeedback Optical Feedback Indicator (Active LOW) |

### 8.2 Toolchain Setup & Firmware Flashing

1. Install the latest **Arduino IDE** (version 2.x recommended);
2. Navigate to `File` -> `Preferences`, and add the official ESP8266 board manager URL:
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Open `Tools` -> `Board Manager`, search for `esp8266` and install version 3.0.0 or later;
4. Open `Tools` -> `Manage Libraries`, and install the following libraries:
   - `MPU6050` by Electronic Cats
   - `I2Cdev` by Jeff Rowberg
   - `ESP8266WiFi` and `ESP8266WebServer` (included with ESP8266 core)
5. Open the main firmware sketch [wearable_posture_node.ino](src/firmware/wearable_posture_node.ino);
6. In the `Tools` menu, configure:
   - **Board**: `Generic ESP8266 Module` (or `NodeMCU 1.0`)
   - **CPU Frequency**: `80 MHz` (or `160 MHz`)
   - **Flash Size**: `4MB (FS:2MB OTA:~1019KB)`
   - **Upload Speed**: `115200`
7. Pull `GPIO0` low to GND, pulse `RESET` to enter UART bootloader mode, and click **Upload**.

### 8.3 Operation & Wireless Verification

1. Disconnect `GPIO0` from GND and press `RESET` to boot into operational mode;
2. Open the Serial Monitor at `115200` baud to observe boot logs and self-test results:
   ```text
   [INIT] E-Textile Wearable Posture Sensing Node Booting...
   [IMU] Initializing InvenSense MPU6050...
   [IMU] MPU6050 connection SUCCESSFUL
   [WIFI] SoftAP IP: 192.168.4.1
   [HTTP] Web telemetry server started on port 80
   ```
3. **Connect to Wi-Fi**: On a smartphone or PC, connect to the Wi-Fi network `Posture_Garment_Node` using password `12345678`;
4. **Access Dashboard**: Open any browser and navigate to `http://192.168.4.1` to view live pitch, roll, acceleration, and posture evaluation metrics;
5. **Test Biofeedback**: In an upright, ergonomic posture, the LED breathes with a soothing rhythm. Slumping forward past $30^\circ$ causes the LED to immediately blink at $10\text{Hz}$, prompting posture correction.

---

## 9. Academic Credits & Acknowledgments

- **Development Team**: 2023 Undergraduate Cohort (Group 03), School of Automation Engineering, University of Electronic Science and Technology of China (UESTC);
  - Student IDs: 2023060904023, 2023060904028, 2023060904025;
- **Advising Faculty**: Teaching Team of "Freshman Project Course: Wearable Computing System Design", School of Automation Engineering, UESTC;
- **Open-Source Acknowledgments**: Grateful acknowledgment to Electronic Cats and Jeff Rowberg for the open-source I2Cdevlib and MPU6050 kinematic libraries.

---

## 10. Open-Source License

This project's firmware, hardware schematics, and technical documentation are distributed under the **MIT License**. For details, please refer to the [LICENSE](LICENSE) file.
