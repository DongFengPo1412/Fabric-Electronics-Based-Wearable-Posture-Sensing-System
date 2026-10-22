# Wearable Posture Sensing — Setup and operation

[简体中文](setup.zh-CN.md) · [English](setup.en.md) · [日本語](setup.ja.md) · [Overview](../README_JA.md)

<a id="environment"></a>

## ツールとスケッチ

ESP8266 core **3.1.2**、Electronic Cats MPU6050 **1.4.5**（I2Cdev を含む）、コアの Wire/Wi-Fi/WebServer でビルド済みです。主 `.ino`、`posture_logic.h`、`network_config.example.h` を独立した `wearable_posture_node` フォルダーにコピーします。旧 `.ino` は同名の個別フォルダーに置き、TCP 例程には `tcp_config.example.h` を含めます。複数の setup/loop は混在させません。

```powershell
arduino-cli compile --fqbn esp8266:esp8266:generic ./wearable_posture_node
```

<a id="wiring"></a>

## 端子と実機条件

GPIO4=SDA、GPIO5=SCL、MPU6050 は 0x68、GPIO2 は負論理 LED、全スケッチのシリアルは 115200 baud です。電源、共通 GND、電圧、AD0、Flash/書き込み方式、GPIO2 の起動条件を実基板で確認してください。

<a id="operation"></a>

## 設定と実行

ローカル設定は `network_config.example.h` を `network_config.private.h` として保存します。TCP 例程はサンプルから `tcp_config.private.h` を作ります。既存の私有設定は上書きしません。両ファイルは Git の無視対象で、公開サンプルに個人のネットワーク設定は含みません。

実機条件を確認してから書き込みます。シリアルで IMU 状態と AP アドレスを確認し、`/` または `/api/data` を開きます。IMU 障害では正常なゼロ値を表示せず、利用不可として再試行します。受信機を起動し実際の IP を設定、実験網で TCP 8080 を許可します。既定の `.4.2` は割り当てを保証しません。旧 TCP 例程は挨拶のみ、姿勢送信は統合版です。

```powershell
python tools/telemetry_receiver.py --bind 0.0.0.0 --port 8080
python -m unittest discover -s tests -p "test_*.py"
```

<a id="validation"></a>

## 検証と今後の作業

今回、4 スケッチの個別ビルド、フィードバック 7 件、埋め込み画面の模擬 7 件、Python パーサー 3 件が成功しました。JSON は模擬データ、ビルドは公開設定です。書き込み・実通信試験は行っていません。

角度誤差、動的干渉、取り付け、PWM 実周期、再接続、消費電力、長期着用は未測定です。±1°、<15 ms、耐洗、量産、50 Hz 送信の根拠はありません。融合研究より先に校正と実周期を測定します。
