# Wearable Posture Sensing — Technical notes

[简体中文](technical-notes.zh-CN.md) · [English](technical-notes.en.md) · [日本語](technical-notes.ja.md) · [Overview](../README.md)

<a id="versions"></a>

## 原始例程与集成固件

综合固件首次见于 2026-09-15 的 `a353b67`，不代表当年实机固件。2026-10-23 前三个例程与本地课程文件逐字节一致；本轮主动修复后不再相同。Blink 修正 `pinMod`；PWM 明确输出及 0–255 范围；TCP/IMU 增加实际连接、有限 Wi-Fi 等待、超时和有效目标地址，统一串口 115200。TCP 例程仍只发送问候并初始化 IMU，不采集姿态。综合固件是另一独立草图，不能将其功能追溯归入原始课程演示。

<a id="sensing"></a>

## 采集与公式

`updateKinematics()` 校验从 `ACCEL_XOUT_H` 读取的 14 字节长度，提取加速度及陀螺仪量；只有加速度参与以下计算，比例 16384 LSB/g 对应库初始化的 ±2g。没有偏置标定、融合、DMP/FIFO、速度或轨迹重建。合加速度包含重力，不是能量积分；动态加速度及安装角影响倾角。

GPIO4/5 软件 TWI 请求 400 kHz，时钟拉伸上限 20000 µs，读取超时参数 20 ms；不是硬实时保证。读失败标记无效并每 2 s 重试初始化；有限数值且合加速度 >0.05g 才有效。没有把全零读取视为正常姿态。

```math
\theta=\mathrm{atan2}(-a_x,\sqrt{a_y^2+a_z^2})\frac{180}{\pi},\quad
\phi=\mathrm{atan2}(a_y,a_z)\frac{180}{\pi}
```

```math
\lVert\mathbf a\rVert=\sqrt{a_x^2+a_y^2+a_z^2},\qquad a_i=\frac{\mathrm{raw}_i}{16384}
```

<a id="feedback"></a>

## 阈值、PWM 与时序

`posture_logic.h` 为 LED 与 API 共用 `abs(pitch)>30 || abs(roll)>35`；等于阈值不报警。无效数据熄灯。模式切换复位 PWM，强度限定 0–1023，GPIO2 输出为 `1023-intensity`。正常状态每 25 ms 步进 ±5，为线性三角波；报警每 50 ms 翻转，理想完整周期 100 ms（10 Hz）。这些是代码目标，网络与主循环会影响实际周期，没有正弦/Gamma 控制、迟滞或持续时间判据。

<a id="network"></a>

## 网络与报文

HTTP 端口 80 仅有 GET `/` 和 `/api/data`。JSON 含 `pitch/roll/accel`、`valid`、`abnormal`、两个阈值及 `age_ms`；无效数值为 `null`。网页采用后端报警结果，样本超过 1000 ms、无效或断连时清除正常显示；请求 1500 ms 中止，结束后 200 ms 再轮询，无重叠请求。没有远程灯控/阈值修改路由。

TCP 仅发送有效样本，尝试间隔至少 100 ms，重连间隔 2 s；数字 IPv4 避免 DNS，连接超时参数 50 ms，发送空间不足时跳过。仍是单线程同步客户端，不能据此宣称端到端 50 ms 时延。附带 `tools/telemetry_receiver.py`（Python 标准库），严格解析有限数值和行长度；兼容历史问候并回复，但数值流没有应用 ACK、时间戳或序号。

AP 地址以串口 `softAPIP()` 为准，接收器默认目标 `192.168.4.2:8080` 不是保证分配的地址。网络参数使用示例头文件或本机忽略的私有头文件；历史提交中的旧配置不因此被清除。

```text
PKT:P=<pitch>,R=<roll>,A=<acceleration_in_g>
```

<a id="hardware"></a>

## 硬件资料与待确认事项

![历史节点近景](images/demo_sensing_node.jpg)

![服饰节点工作画面](images/demo_wearable_dual_node_active.png)

照片支持 ESP-12F 圆形节点、MPU6050 模块和缝合安装形态；不独立证明导电线是银还是不锈钢、线径/电阻率、可靠性、耐洗性或量产成熟度。导电线没有进入ADC/电桥测量路径，不是已经完成的应变传感器。

仓库没有可编辑原理图、PCB工程、BOM或电源测试报告。器件图片和课程电路图可作参考，但不据此断言本机一定采用TP4054/SGM2019、800mAh电池、指定充电电流、完整保护或续航。GPIO2是源码指定的低有效LED输出；外部LED与电源应按实板核对。旧6DOF轨迹示意、含融合的系统图不作为当前算法证据。

保留课程团队背景，不根据未对应版本的材料认定个人设计了全部软硬件。确切分工、当年固件与实测记录仍待补充。
