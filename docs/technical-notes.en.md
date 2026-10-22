# Wearable Posture Sensing — Technical notes

[简体中文](technical-notes.zh-CN.md) · [English](technical-notes.en.md) · [日本語](technical-notes.ja.md) · [Overview](../README_EN.md)

<a id="versions"></a>

## Original sketches and integrated firmware

The integrated sketch first appears in `a353b67` on 2026-09-15, which does not establish historical deployment. Before this maintenance the three examples were byte-identical to the local course originals; they now deliberately differ. Blink fixes `pinMod`; PWM sets its output and 0–255 range; TCP/IMU adds a real connection, bounded Wi-Fi wait, timeouts, a usable target address, and consistent 115200-baud logging. That example still initializes the IMU and sends a greeting, without sampling posture. The integrated sketch remains separate; its features must not be attributed retrospectively to the course demonstration.

<a id="sensing"></a>

## Acquisition and equations

`updateKinematics()` checks a 14-byte read from `ACCEL_XOUT_H`, extracting accelerometer and gyro values. Only acceleration enters the equations below; 16384 LSB/g matches the library initialization at ±2g. There is no offset calibration, fusion, DMP/FIFO, velocity, or trajectory reconstruction. Magnitude includes gravity, not integrated energy; dynamic acceleration and mounting affect tilt.

GPIO4/5 software TWI requests 400 kHz, with a 20000 µs clock-stretch limit and a 20 ms read-timeout argument; these are not hard real-time guarantees. Failed reads invalidate the sample and trigger initialization retries every 2 s. Finite values and magnitude >0.05g are required; an all-zero read is not treated as a healthy posture.

```math
\theta=\mathrm{atan2}(-a_x,\sqrt{a_y^2+a_z^2})\frac{180}{\pi},\quad
\phi=\mathrm{atan2}(a_y,a_z)\frac{180}{\pi}
```

```math
\lVert\mathbf a\rVert=\sqrt{a_x^2+a_y^2+a_z^2},\qquad a_i=\frac{\mathrm{raw}_i}{16384}
```

<a id="feedback"></a>

## Thresholds, PWM, and timing

`posture_logic.h` shares `abs(pitch)>30 || abs(roll)>35` between LED and API; equality does not alert. Invalid data turns the LED off. Mode changes reset PWM, intensity is clamped to 0–1023, and GPIO2 receives `1023-intensity`. Normal steps are ±5 every 25 ms, forming a linear triangular ramp. Alert toggles every 50 ms, ideally a 100 ms complete cycle (10 Hz). These are source targets affected by network and loop work, without sine/Gamma control, hysteresis, or a persistence criterion.

<a id="network"></a>

## Networking and packets

HTTP port 80 exposes only GET `/` and `/api/data`. JSON includes `pitch/roll/accel`, `valid`, `abnormal`, both thresholds, and `age_ms`; invalid measurements are `null`. The page uses the backend alert decision and clears healthy status on invalid data, age >1000 ms, or connection failure. Requests abort after 1500 ms and restart 200 ms after completion, without overlap. No remote LED/threshold control route exists.

TCP sends valid samples only, at least 100 ms apart, with 2 s reconnect backoff. Numeric IPv4 avoids DNS; connection timeout is configured at 50 ms, and insufficient write space skips a sample. This remains a synchronous single-threaded client, not an end-to-end 50 ms guarantee. `tools/telemetry_receiver.py` uses the Python standard library and validates finite numbers and line length. It can acknowledge the legacy greeting; numeric telemetry has no application ACK, timestamp, or sequence number.

Use the printed `softAPIP()` address. Default receiver target `192.168.4.2:8080` is not a guaranteed lease. Example or locally ignored private headers hold network configuration; this does not remove old configuration from Git history.

```text
PKT:P=<pitch>,R=<roll>,A=<acceleration_in_g>
```

<a id="hardware"></a>

## Hardware evidence and open questions

![Historical sensing-node close-up](images/demo_sensing_node.jpg)

![Garment nodes in a historical demonstration](images/demo_wearable_dual_node_active.png)

Photos support the ESP-12F circular node, MPU6050 module, and stitched mounting. They do not establish silver versus stainless yarn, diameter/resistivity, washability, reliability, or production readiness. Conductive thread is not connected to an ADC/bridge acquisition path and is not an implemented strain sensor.

Editable schematics, PCB projects, a BOM, and power measurements are absent. Component images/course circuits are references, not proof of TP4054/SGM2019, an 800 mAh battery, a particular charge current, complete protection, or runtime in this unit. GPIO2 is the source-configured active-low LED output; external LED/power wiring needs board verification. The old 6DOF trajectory and fusion diagrams do not document the current algorithm.

Course-team context is retained without attributing all hardware/software design to one person from unmatched material. Exact ownership, deployed firmware, and measurement records remain open.
