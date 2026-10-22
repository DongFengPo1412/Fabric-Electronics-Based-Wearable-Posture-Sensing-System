# Wearable Posture Sensing — Setup and operation

[简体中文](setup.zh-CN.md) · [English](setup.en.md) · [日本語](setup.ja.md) · [Overview](../README_EN.md)

<a id="environment"></a>

## Tools and sketches

Compiled with ESP8266 core **3.1.2**, Electronic Cats MPU6050 **1.4.5** (including I2Cdev), and core Wire/Wi-Fi/WebServer. Copy the main `.ino`, `posture_logic.h`, and `network_config.example.h` into a separate `wearable_posture_node` directory. Place each historical `.ino` in its own matching-name sketch folder; include `tcp_config.example.h` for TCP. Do not combine multiple setup/loop definitions.

```powershell
arduino-cli compile --fqbn esp8266:esp8266:generic ./wearable_posture_node
```

<a id="wiring"></a>

## Pins and physical requirements

GPIO4=SDA, GPIO5=SCL; MPU6050 address 0x68; GPIO2 is active-low LED; all sketches log at 115200 baud. Verify power, common ground, logic levels, AD0, actual flash/upload settings, and GPIO2 boot levels against the carrier.

<a id="operation"></a>

## Configuration and operation

To customize locally, save `network_config.example.h` as `network_config.private.h`; use `tcp_config.private.h` from its example for the TCP sketch. Preserve existing private files. Both private headers are ignored; public templates contain no personal network configuration.

Verify hardware before flashing. Inspect IMU status and AP address over serial, then browse `/` or `/api/data`. IMU faults show unavailable data and retry, rather than healthy zeros. Start the receiver and configure the real host address; allow TCP 8080 on the experiment network. Default `.4.2` is not an assigned-address guarantee. The old TCP sketch sends greetings; posture packets require the integrated sketch.

```powershell
python tools/telemetry_receiver.py --bind 0.0.0.0 --port 8080
python -m unittest discover -s tests -p "test_*.py"
```

<a id="validation"></a>

## Verification and next steps

This maintenance compiled four separate sketches and passed seven feedback, seven embedded-page mock, and three Python-parser cases. Browser JSON was simulated and builds used public configuration. No flashing or physical network test was performed.

Angle error, dynamic interference, mounting, actual PWM timing, reconnection, power, and prolonged wear remain unmeasured. Earlier ±1°, <15 ms, washability, production, and 50 Hz streaming claims lack evidence. Calibration and physical timing measurements precede fusion research.
