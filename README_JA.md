# スマートテキスタイル電子回路に基づくウェアラブル姿勢センシング・生体フィードバックシステム
# Fabric-Electronics-Based Wearable Posture Sensing & Wireless Biofeedback System

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](LICENSE)
[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266%20Xtensa%2032bit-red.svg?style=flat-square)](https://www.espressif.com/)
[![Sensor: MPU6050](https://img.shields.io/badge/IMU-InvenSense%20MPU6050-green.svg?style=flat-square)](https://invensense.tdk.com/)
[![Medium: E--Textile Conductive Yarn](https://img.shields.io/badge/E--Textile-Conductive%20Yarn%20Matrix-purple.svg?style=flat-square)](docs/)
[![Protocol: I2C & TCP/IP](https://img.shields.io/badge/Protocol-I2C%20400kHz%20%7C%20TCP%20Socket-orange.svg?style=flat-square)](src/)

[**中文文档**](README.md) | [**English**](README_EN.md) | [**日本語**](README_JA.md)

</div>

---

## 1. プロジェクト概要 (Project Overview)

**スマートテキスタイル電子回路に基づくウェアラブル姿勢センシング・生体フィードバックシステム**は、デジタルヘルスケア、脊柱姿勢モニタリング、および次世代ヒューマンマシンインターフェース（HMI）に向けた高集積フレキシブルIoTソリューションです。

従来の剛性基板を用いたウェアラブルデバイスは、身体適合性の低さ、装着時の異物感、関節屈曲に伴う配線疲労断線などの課題を抱えていました。本研究開発では、**高導電性ステンレス鋼・銀メッキ複合微細導電糸（Conductive Yarn）**を採用し、精密刺繍技法および二層絶縁配線構造により、100%天然コットン布地基盤へ直接組み込むことに成功しました。これにより、装着感ゼロかつ極めて高い耐屈曲性を備えた柔軟なテキスタイル電子マイクロバスを実現しています。

エッジ処理中核には低消費電力 **Espressif ESP8266 (ESP-12F) 32ビット Xtensa マイクロプロセッサ**を配置し、**InvenSense MPU-6050 6軸慣性計測ユニット（3軸 MEMS 加速度計 ＋ 3軸 MEMS ジャイロスコープ）**と高速 I2C 接続しています。エッジ側で実行される離散相補フィルタリングと運動力学エネルギー推定モデルにより、猫背（前屈）や側弯（横傾斜）といった不良姿勢をリアルタイムに検知。非線形呼吸パルス幅変調（PWM）による局所光生体フィードバックおよび Wi-Fi TCP/IP / WebServer による遠隔テレメトリのデュアル系統を通じて、高精度な姿勢計測と閉ループ健康介入を提供します。

---

## 2. ハードウェア実装とシステム外観 (Visual Showcase)

<div align="center">

| システム全体アーキテクチャ・データフロー | 導電糸配線統合スマートウェアと主制御ノード実物 |
| :---: | :---: |
| <img src="docs/images/system_architecture.png" width="450" alt="System Architecture"> | <img src="docs/images/demo_e_textile_garment.jpg" width="450" alt="E-Textile Garment"> |
| **MPU6050 空間姿勢慣性センサノードと縫合配線** | **円形ウェアラブル ESP8266 コアボードと端子配置** |
| <img src="docs/images/demo_sensing_node.jpg" width="450" alt="Sensing Node"> | <img src="docs/images/hardware_esp8266.jpg" width="450" alt="Hardware Node"> |

</div>

---

## 3. 運動生体力学と数理モデル導出 (Mathematical Formulations)

エッジファームウェアおよび遠隔モニタリング端末には、厳密な力学理論と数理モデルを実装しています。

### 3.1 導電糸ピエゾ抵抗歪み特性とホイートストンブリッジ回路方程式

着用者の体動に伴う布地の伸縮変形により、縫合された導電糸の繊維間接触抵抗が変化します。無歪み状態の基準抵抗を $R_0$、機械的歪みを $\varepsilon = \frac{\Delta L}{L_0}$、ゲージ率（Gauge Factor）を $G_F$ とすると、等価抵抗特性は以下のように定式化されます：

$$
R(\varepsilon) = R_0 (1 + G_F \cdot \varepsilon)
$$

微小歪み信号を同相ノイズを除去して高精度に検出するため、対称ホイートストンブリッジ回路を導入しています。差動出力電圧方程式は次の通りです：

$$
V_{\text{out}} = V_{\text{in}} \left( \frac{R_2}{R_1 + R_2} - \frac{R_x(\varepsilon)}{R_3 + R_x(\varepsilon)} \right)
$$

平衡条件 $$R_1 = R_2 = R_3 = R_0$$ を適用した一次線形近似特性は以下で与えられます：

$$
V_{\text{out}} \approx \frac{V_{\text{in}}}{4} \cdot G_F \cdot \varepsilon
$$

### 3.2 重力加速度ベクトルに基づく空間オイラー姿勢角の幾何導出

3軸 MEMS 加速度計により計測される正規化重力加速度ベクトルを $$\mathbf{a} = [a_x, a_y, a_z]^{\top}$$ と定義します。剛体運動学の投影幾何学に基づき、体幹の傾斜角（Pitch 角 $\theta$：前屈度合）およびロール角（Roll 角 $\phi$：側屈度合）は解析的に導出されます。

ピッチ角 $\theta$（矢状面傾斜 / 猫背姿勢）：

$$
\theta = \arctan \left( \frac{-a_x}{\sqrt{a_y^2 + a_z^2}} \right) \cdot \frac{180^\circ}{\pi}
$$

ロール角 $\phi$（前額面傾斜 / 側弯姿勢）：

$$
\phi = \arctan \left( \frac{a_y}{a_z} \right) \cdot \frac{180^\circ}{\pi}
$$

### 3.3 加速度計・ジャイロスコープの離散時間相補フィルタリング

加速度計の高周波振動ノイズとジャイロスコープの低周波積分ドリフトを同時に抑制するため、サンプリング周期 $\Delta t$ の離散時間相補フィルタを適用します：

$$
\theta_t = \alpha (\theta_{t-1} + \omega_{\text{gyro}} \cdot \Delta t) + (1 - \alpha) \theta_{\text{acc}}
$$

時定数 $\tau$ に依存する重み付け係数 $\alpha$ は以下を満たします：

$$
\alpha = \frac{\tau}{\tau + \Delta t}, \quad (0.95 \le \alpha \le 0.98)
$$

### 3.4 3次元ユークリッド合成加速度と動的運動エネルギー指標

静止立位・座位と激しい歩行運動を自律識別するため、全合成加速度のユークリッドノルム $|\mathbf{a}|$ を算出します：

$$
|\mathbf{a}| = \sqrt{a_x^2 + a_y^2 + a_z^2}
$$

観測時間窓 $$t \in [t_0, t_0 + T]$$ において、静的重力ベクトル $\mathbf{g}$ を減算した体幹の動的運動エネルギー積分値は以下で与えられます：

$$
E_{\text{kinetic}} = \frac{1}{2} m \int_{t_0}^{t_0 + T} \left\| \mathbf{a}(t) - \mathbf{g} \right\|^2 \, dt
$$

$|\mathbf{a}| \approx 1.0g$ の静的状態において姿勢判定感度を最大化し、過渡運動時には閾値判定ロジックにより誤警報を抑制します。

### 3.5 ウェーバー・フェヒナーの法則に基づく非線形呼吸生体フィードバック則

人間の視覚心理物理学（Weber-Fechner の法則）に適合した自然な光感覚を提供するため、PWM 出力デューティ比 $D(t)$ にべき乗補正を施した呼吸曲線を生成します：

$$
D(t) = D_{\min} + (D_{\max} - D_{\min}) \cdot \left[ \frac{1}{2} \left( 1 + \sin \left( \frac{2\pi t}{T_{\text{breath}}} - \frac{\pi}{2} \right) \right) \right]^\gamma
$$

ここで呼吸周期 $T_{\text{breath}} = 3.0\text{ s}$、補正指数 $\gamma = 2.2$ と設定。姿勢傾斜が許容閾値 $|\theta| > \theta_{\text{threshold}}$ を超過した場合、即座に高周波ストロボ警報モード（$f = 10\text{ Hz}$）へと遷移します。

---

## 4. ハードウェア・ソフトウェア仕様 (System Specifications)

| サブシステム項目 | 採用技術・コンポーネント | 仕様諸元および実装標準 |
| :--- | :--- | :--- |
| **主制御プロセッサ** | Espressif ESP8266 (ESP-12F) | Tensilica 32-bit LX106 @ 80/160MHz, 4MB Flash |
| **慣性センサ (IMU)** | InvenSense MPU-6050 | 3軸加速度計 ($\pm 2g$) ＋ 3軸ジャイロ ($\pm 250^\circ/s$), 16-bit ADC |
| **内部通信バス** | ハードウェア I2C Fast-Mode | 通信周波数 $f_{\text{SCL}} = 400\text{ kHz}$、ポーリング周期 $10\text{ ms}$ |
| **テキスタイル導電配線** | 柔軟複合導電繊維糸 | 線径 $0.2\text{ mm}$、線抵抗 $< 1.5\,\Omega/\text{cm}$、柔軟絶縁層構造 |
| **無線通信スタック** | 802.11 b/g/n Wi-Fi | SoftAP ＋ Station デュアル動作、内蔵 WebServer / TCP ソケット |
| **生体光フィードバック** | GPIO PWM 呼吸インジケータ | 10ビット分解能 ($0 \sim 1023$)、平常時呼吸態 / 逸脱時ストロボ警報 |
| **遠隔テレメトリUI** | 組み込み Web ダッシュボード | JSON 形式リアルタイムストリーミング ($5\text{ Hz}$)、レスポンシブ UI |

---

## 5. ディレクトリ構造 (Repository Layout)

```text
Fabric-Electronics-Based-Wearable-Posture-Sensing-System/
├── docs/
│   └── images/
│       ├── system_architecture.png       # システムアーキテクチャ・データフロー全体図
│       ├── demo_e_textile_garment.jpg    # スマートウェア・円形制御ボード実写 (1920x1280)
│       ├── demo_sensing_node.jpg         # MPU6050 慣性センサ接写写真 (1920x1280)
│       ├── hardware_esp8266.jpg          # 円形ウェアラブル ESP8266 ボード写真
│       └── hardware_mpu6050.jpg          # MPU6050 センサモジュール接写写真
├── src/
│   └── firmware/
│       ├── wearable_posture_node.ino     # 量産対応組込みファームウェア (I2C+Filter+Web+TCP)
│       └── examples/
│           ├── sketch_mar26a_tcp_imu.ino # 基礎 TCP 通信および MPU6050 動作検証
│           ├── sketch_mar19a2_pwm.ino    # 非線形呼吸 PWM 調光ルーチン
│           └── sketch_mar12a1_blink.ino  # 初期導通確認用 GPIO テストコード
├── LICENSE                               # 公式 MIT ライセンス条文
├── README.md                             # 中国語技術解説ドキュメント
├── README_EN.md                          # 英語エンジニアリング仕様書
└── README_JA.md                          # 日本語技術仕様書・学術ポートフォリオ
```

---

## 6. ビルド・展開およびクイックスタート (Quick Start)

### 6.1 ピンアサイン定義 (Pin Mapping)

| ESP8266 端子 | 導電糸テキスタイル配線 | MPU6050 端子 | 接続機能説明 |
| :---: | :---: | :---: | :---: |
| **3V3** | 導電糸 VCC バス | VCC | 3.3V 安定化電源供給 |
| **GND** | 導電糸 GND バス | GND | システム共通基準グランド |
| **GPIO4 (D2)** | 導電糸 SDA バス | SDA | I2C 双方向シリアルデータ信号線 |
| **GPIO5 (D1)** | 导電糸 SCL バス | SCL | I2C シリアルクロック信号線 ($400\text{ kHz}$) |
| **GPIO2 (D4)** | 基板上 / 外付け LED | - | 非線形呼吸・姿勢逸脱光警報 |

### 6.2 ファームウェアのコンパイルと書き込み

1. **Arduino IDE** を起動し、ESP8266 ボードマネージャ URL を追加します：
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
2. 依存ライブラリを導入：`MPU6050` (Electronic Cats 製)、`I2Cdev`、`ESP8266WiFi`。
3. `src/firmware/wearable_posture_node.ino` を開きます。
4. `ツール` $\rightarrow$ `ボード` から `Generic ESP8266 Module`（または `NodeMCU 1.0`）を選択し、ボーレートを `115200` に設定して **マイコンボードに書き込む** を実行します。

### 6.3 遠隔テレメトリの動作確認

1. 給電後、ウェアラブルノードは自動的に Wi-Fi AP `Posture_Garment_Node` を起動します。
2. PC またはスマートフォンから同 Wi-Fi へ接続（パスワード：`12345678`）。
3. ブラウザから `http://192.168.4.1` へアクセスします。
4. リアルタイムに傾斜角、側弯角、合成加速度、および姿勢健全度ステータスが $5\text{ Hz}$ 周期で描画されます。

---

## 7. ライセンス (License)

本リポジトリは **MIT License** のもとで公開されています。詳細は [LICENSE](LICENSE) をご参照ください。
