# Wearable Posture Sensing — 技术说明

**简体中文** · [English](technical-notes.en.md) · [日本語](technical-notes.ja.md) · [项目首页](../README.md)

<a id="versions"></a>

## 课程实验与综合固件的区别

| 文件 | 实际逻辑 | 缺口 |
| --- | --- | --- |
| [blink](../src/firmware/examples/sketch_mar12a1_blink.ino) | GPIO2 高低电平各保持 2 s 的意图 | `pinMod` 拼写错误，不能原样编译 |
| [PWM](../src/firmware/examples/sketch_mar19a2_pwm.ino) | 0–255 步长 5、每步延时 30 ms 的线性升降 | 无显式 `pinMode`/PWM范围配置；没有非线性校正 |
| [TCP/IMU](../src/firmware/examples/sketch_mar26a_tcp_imu.ino) | 等待 STA 联网、初始化 MPU6050、尝试文本读写 | 没有 `client.connect()`、没有读取运动量；主机变量为回环地址且未使用；串口先115200再38400 |
| [综合固件](../src/firmware/wearable_posture_node.ino) | 采集、倾角、PWM、HTTP、TCP客户端 | 后来归档的实现；与当年演示固件的对应关系未证实 |

三个示例与本地课程目录 `新生项目课` 的对应 `.ino` 逐字节相同。综合固件首次出现在仓库 `a353b67`（2026-09-15）；课程原始草图中没有它，不能把新增时间或注释中的“production”当作历史完成证明。

<a id="sensing"></a>

## 采集与数学实现

```math
\theta=\mathrm{atan2}(-a_x,\sqrt{a_y^2+a_z^2})\frac{180}{\pi},\quad
\phi=\mathrm{atan2}(a_y,a_z)\frac{180}{\pi}
```

```math
\lVert\mathbf a\rVert=\sqrt{a_x^2+a_y^2+a_z^2},\qquad a_i=\frac{\mathrm{raw}_i}{16384}
```

`updateKinematics()` 经 `getMotion6()` 取六个 `int16_t`，加速度除以16384。已编译的 MPU6050 1.4.5 库默认初始化为 ±2g、陀螺仪 ±250°/s，源码没有显式校准偏置。`atan2` 输出角度，合加速度保留重力分量，不是动能积分。

当前没有使用 `raw_gx/raw_gy/raw_gz`、积分时间或融合状态，没有 DMP/FIFO 采样、航向角、速度/位置重建，也没有动态加速度抑制告警。准静态假设、安装方向、偏置与运动加速度都会影响倾角；不能把传感器分辨率、保留的小数位数或网页显示精度当作系统角度准确率。

`Wire.begin(4,5)` 与 `Wire.setClock(400000)` 是代码配置；ESP8266 core 的 `Wire` 使用软件 TWI 实现，不能称独立硬件 I2C 控制器，也没有信号实测保证。IMU 自检失败只打印警告，未实现注释所说的重新初始化重试或有效数据门控。

<a id="feedback"></a>

## 判据、PWM与时间

| 路径 | 实际判断或定时 |
| --- | --- |
| 固件 LED | 仅 `abs(pitch) > 30`；不检查 roll |
| 浏览器徽标 | `abs(pitch) > 30` 或 `abs(roll) > 35` |
| PWM 更新 | 检查间隔25 ms；正常步进±5、端点约5/1020，输出 `1023-pwm_val` |
| 告警闪烁 | 每次更新在0和1023间切换；理想连续条件约20 Hz，不是注释写的10 Hz |
| TCP发送 | 检查间隔100 ms，约10次/s的名义上限；连接与HTTP处理可延迟 |
| 网页轮询/主循环 | 浏览器200 ms定时器；固件每轮另有 `delay(10)`，不是固定100 Hz采样器 |

正常 LED 是线性三角波，按代码端点估算周期约10.15 s，不是3 s正弦/Gamma2.2/韦伯定律控制。模式切换没有重置与完整限幅，旧 PWM 状态可能造成端点越界；没有滞回、持续时间判定或运动状态门控。侧倾可导致网页告警而 LED 不告警。这些是需修复的行为，未在本次文档工作中改动代码。

<a id="network"></a>

## 网络、页面与协议

`setup()` 配置 AP+STA 模式并启动HTTP80，只注册 GET `/` 和 GET `/api/data`。API字段是 `pitch`、`roll`、`accel`；固件保留两位小数，浏览器显示一位角度/两位合加速度。没有远程开关灯、阈值设置、云服务或手机App控制路由。

`streamTelemetry()` 使用阻塞式 `WiFiClient.connect` 重连至配置的 `192.168.4.2:8080`，成功后发送下列换行结束文本；接收端不在仓库中，没有应用层确认、时间戳、序列号或接收反馈。

```text
PKT:P=<pitch>,R=<roll>,A=<acceleration_in_g>

```

AP默认网关通常为 `192.168.4.1`，应以串口 `softAPIP()` 输出为准；旧图片中的 `192.168.147.81` 不能当作这个 AP 的固定地址。综合固件串口115200仅打印初始化信息，没有每帧串口三轴数据输出。反复连接失败可能拖慢全部单线程功能；没有可靠实时性测量。网络凭据需要按自己的网络配置，文档不重复发布 STA 口令。

<a id="hardware"></a>

## 硬件资料与待确认事项

![历史节点近景](images/demo_sensing_node.jpg)

![服饰节点工作画面](images/demo_wearable_dual_node_active.png)

照片支持 ESP-12F 圆形节点、MPU6050 模块和缝合安装形态；不独立证明导电线是银还是不锈钢、线径/电阻率、可靠性、耐洗性或量产成熟度。导电线没有进入ADC/电桥测量路径，不是已经完成的应变传感器。

仓库没有可编辑原理图、PCB工程、BOM或电源测试报告。器件图片和课程电路图可作参考，但不据此断言本机一定采用TP4054/SGM2019、800mAh电池、指定充电电流、完整保护或续航。GPIO2是源码指定的低有效LED输出；外部LED与电源应按实板核对。旧6DOF轨迹示意、含融合的系统图不作为当前算法证据。

保留课程团队背景，不根据未对应版本的材料认定个人设计了全部软硬件。确切分工、当年固件与实测记录仍待补充。
