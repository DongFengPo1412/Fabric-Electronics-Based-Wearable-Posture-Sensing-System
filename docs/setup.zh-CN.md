# Wearable Posture Sensing — 部署与操作

**简体中文** · [English](setup.en.md) · [日本語](setup.ja.md) · [项目首页](../README.md)

<a id="environment"></a>

## 工具与草图目录

本次编译组合：ESP8266 Arduino core **3.1.2**、Electronic Cats **MPU6050 1.4.5**（自带 `I2Cdev.h`）、core自带 `Wire`/`ESP8266WiFi`/`ESP8266WebServer`。没有测试所有同名MPU6050库的兼容性，不需要额外叠加另一个同名驱动。

将 [wearable_posture_node.ino](../src/firmware/wearable_posture_node.ino) 复制到名为 `wearable_posture_node` 的独立草图目录，Arduino要求目录与主文件同名。不要合并 `examples` 下具有各自 `setup/loop` 的程序。已安装对应平台和库时可编译：

```powershell
arduino-cli compile --fqbn esp8266:esp8266:generic ./wearable_posture_node
```

本次在仓库外副本编译成功，有依赖宏重定义警告；这仅证明该工具与库组合能生成固件，不代表实板Flash容量、连线或功能已验证。

<a id="wiring"></a>

## 引脚与实板条件

| 代码引脚/接口 | 接线依据 |
| --- | --- |
| GPIO4 / GPIO5 | MPU6050 SDA / SCL，共地；核对模块供电与逻辑电平 |
| GPIO2 | 低有效LED/PWM；不是蜂鸣器或振动马达 |
| MPU6050 默认地址 | 已编译库默认0x68；核对AD0与模块地址 |
| 串口 | 综合固件115200；历史TCP示例最后为38400 |

GPIO编号不等于任意开发板丝印D编号。板型、Flash、电源、下载启动方式依据实际 ESP-12F 载板，不沿用旧文档未经核实的4MB和电源芯片设定。

<a id="operation"></a>

## 网络与使用步骤

1. 独立配置综合固件中的 AP/STA 与遥测目标，编译并在确认硬件后自行烧录；本次未烧录。
2. 查看115200串口的IMU自检和AP地址。当前失败后仍继续运行，显示数值不等于传感器有效。
3. 连接配置的AP，浏览串口显示地址的 `/` 页面；访问 `/api/data` 可检查三个JSON字段。
4. TCP需要外部接收服务器在目标地址8080端口监听。目标 `192.168.4.2` 是配置值，DHCP不保证接收机一定获得它；地址应核对，仓库没有接收软件。
5. 对照已知安装方向分别测试俯仰与侧倾，同时观察LED和网页不同判据。当前没有网页开关灯按钮接口；旧网页截图不作为现固件操作指南。

<a id="validation"></a>

## 验证边界与改进顺序

已完成：四份源程序阅读、课程原始示例逐字节比对、综合固件编译和浏览器页面逻辑的模拟数据检查。页面检查仅使用主固件内嵌HTML与模拟JSON，不是实机遥测。

未完成：传感器/织物/供电实测、网络收发与断连测试、实际亮度和闪烁频率、误报率、角度误差、长期穿戴或安全性验证。旧“±1°”“<15ms”“耐水洗”“量产级”“50Hz串流”等说法没有证据。

后续先统一LED/网页阈值并处理IMU失败、TCP阻塞和PWM状态，再做标定与采样计时；姿态融合、稳定网络协议及服饰可靠性作为单独的后续工作。
