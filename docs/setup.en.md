# Wearable Posture Sensing — Setup and operation

[简体中文](setup.zh-CN.md) · **English** · [日本語](setup.ja.md) · [Overview](../README_EN.md)

<a id="environment"></a>

## Tools and sketch naming

Compiled combination: ESP8266 Arduino core **3.1.2**, Electronic Cats **MPU6050 1.4.5** including `I2Cdev.h`, and core-provided `Wire`/`ESP8266WiFi`/`ESP8266WebServer`. Other similarly named MPU6050 libraries were not tested; do not add a second same-name driver unnecessarily.

Copy [wearable_posture_node.ino](../src/firmware/wearable_posture_node.ino) into a standalone directory named `wearable_posture_node`, matching Arduino sketch naming. Do not combine examples containing separate `setup/loop` functions. With the platform and library installed:

```powershell
arduino-cli compile --fqbn esp8266:esp8266:generic ./wearable_posture_node
```

A copy outside the repository compiled successfully, with dependency macro-redefinition warnings. This proves compilation for that combination, not actual board flash size, wiring, or runtime behavior.

<a id="wiring"></a>

## Pins and physical requirements

| Source pin/interface | Wiring basis |
| --- | --- |
| GPIO4 / GPIO5 | MPU6050 SDA / SCL, common ground; verify module supply and logic levels |
| GPIO2 | Active-low LED/PWM, not a buzzer or vibration motor |
| MPU6050 address | Compiled library defaults to 0x68; check AD0/module address |
| Serial | Integrated firmware 115200; historical TCP example ends at 38400 |

GPIO numbers are not universal board D-labels. Select the board, flash, power, and boot/upload method from the actual ESP-12F carrier rather than assuming the former 4 MB and power-IC specification.

<a id="operation"></a>

## Network and operation

1. Configure integrated-firmware AP/STA and telemetry targets, compile, and flash only after verifying hardware. No flashing was performed in this review.
2. At 115200 baud, inspect IMU self-test and AP address. Firmware continues after a failed check, so displayed numbers alone do not establish valid sensing.
3. Connect to the configured AP and browse `/` at the printed address; `/api/data` exposes the three JSON fields.
4. TCP requires an external listener at the target address on port 8080. `192.168.4.2` is a configuration, not a guaranteed DHCP assignment. Check it; receiver software is absent.
5. Compare known pitch and roll changes with both LED and browser thresholds. No web LED-switch endpoint exists; older UI screenshots are not instructions for the current firmware.

<a id="validation"></a>

## Verification boundaries and next steps

Completed: review of all four sketches, byte comparison with course originals, integrated-firmware compilation, and browser page checks with simulated JSON. The browser check uses embedded HTML and mock data, not live device telemetry.

Not performed: sensor/textile/power measurements, network reception/disconnection tests, actual brightness or blink frequency, false-positive rate, angle error, long-term wear, or safety validation. Former ±1°, <15 ms, washability, production readiness, and 50 Hz streaming claims lack evidence.

First align LED/browser rules and handle IMU faults, TCP blocking, and PWM state. Then calibrate and measure timing. Fusion, robust networking, and garment reliability are separate future work.
