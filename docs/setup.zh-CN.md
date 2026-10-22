# Wearable Posture Sensing — Setup and operation

[简体中文](setup.zh-CN.md) · [English](setup.en.md) · [日本語](setup.ja.md) · [Overview](../README.md)

<a id="environment"></a>

## 环境与草图

已编译：ESP8266 core **3.1.2**、Electronic Cats MPU6050 **1.4.5**（包含 I2Cdev）、核心 Wire/Wi-Fi/WebServer。将主 `.ino`、`posture_logic.h` 和 `network_config.example.h` 复制到独立 `wearable_posture_node` 文件夹。三个历史 `.ino` 分别置于与文件同名的草图目录；TCP 例程同时带上 `tcp_config.example.h`。不要合并多个 setup/loop。

```powershell
arduino-cli compile --fqbn esp8266:esp8266:generic ./wearable_posture_node
```

<a id="wiring"></a>

## 引脚与实物条件

GPIO4=SDA、GPIO5=SCL，MPU6050 地址 0x68；GPIO2 为低有效 LED，所有程序串口 115200。供电、共地、电平、AD0、实际 Flash/烧录方式和 GPIO2 启动电平需按实板确认。

<a id="operation"></a>

## 配置与运行

如需自定义配置，在本机将 `network_config.example.h` 另存为 `network_config.private.h`；TCP 例程将 `tcp_config.example.h` 另存为 `tcp_config.private.h`。不要覆盖已有私有配置。私有文件已忽略，公开模板不含个人网络配置。

核实硬件后再编译烧录。串口检查 IMU 状态和 AP 地址，浏览 `/` 或 `/api/data`。IMU 失效会显示不可用并重试，不把零值当成正常。启动接收器后将目标 IP 配成接收主机的实际地址，允许实验网入站 TCP 8080；默认 `.4.2` 不代表已经获得该地址。旧 TCP 例程只发送问候，综合版才发送姿态。

```powershell
python tools/telemetry_receiver.py --bind 0.0.0.0 --port 8080
python -m unittest discover -s tests -p "test_*.py"
```

<a id="validation"></a>

## 验证与后续工作

本轮完成四个独立草图编译、反馈逻辑 7 项、内嵌页面模拟 7 项和 Python 解析器 3 项检查。浏览器使用模拟 JSON，编译使用公开配置；没有实体烧录或链路测试。

仍需测量角度误差、动态干扰、安装方向、真实 PWM 周期、断连恢复、功耗和长时佩戴。±1°、<15 ms、耐洗、量产和 50 Hz 流式传输均无证据；校准和实体计时先于融合研究。
