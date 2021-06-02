# EV3RT - EV3RTクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3RTの各種定数が定義されているクラスです。

## 定数一覧

### センサポート

|定数|概要|備考|
|:--|:--|:--|
|PORT_1|センサポート1||
|PORT_2|センサポート2||
|PORT_3|センサポート3||
|PORT_4|センサポート4||
|TNUM_SENSOR_PORT|センサポートの数||

---

### センサタイプ

|定数|概要|備考|
|:--|:--|:--|
|NONE_SENSOR|センサ未接続||
|ULTRASONIC_SENSOR|超音波センサ||
|GYRO_SENSOR|ジャイロセンサ||
|TOUCH_SENSOR|タッチセンサ||
|COLOR_SENSOR|ジカラーセンサ||
|TNUM_SENSOR_TYPE|センサタイプの数||

---

### モータポート

|定数|概要|備考|
|:--|:--|:--|
|PORT_A|モータポートA||
|PORT_B|モータポートB||
|PORT_C|モータポートC||
|PORT_D|モータポートD||
|TNUM_MOTOR_PORT|モータポートの数||

---

### モータタイプ

|定数|概要|備考|
|:--|:--|:--|
|NONE_MOTOR|モータ未接続||
|MEDIUM_MOTOR|サーボモータM||
|LARGE_MOTOR|サーボモータL||
|UNREGULATED_MOTOR|未調整モータ||
|TNUM_MOTOR_TYPE|モータタイプの数||

---

### LED点灯色

|定数|概要|備考|
|:--|:--|:--|
|LED_OFF|OFF(LED消灯)||
|LED_RED|赤||
|LED_GREEN|緑||
|LED_ORANGE|オレンジ色||

---

### ボタン

|定数|概要|備考|
|:--|:--|:--|
|LEFT_BUTTON|左ボタン||
|RIGHT_BUTTON|右ボタン||
|UP_BUTTON|上ボタン||
|DOWN_BUTTON|下ボタン||
|ENTER_BUTTON|ENTERボタン||
|BACK_BUTTON|BACKボタン||
|TNUM_BUTTON|ボタンの数||

---

## 使用例

```ruby
# センサオブジェクト生成
ts = EV3RT::TouchSensor.new(EV3RT::PORT_1)  # センサポート1番をタッチセンサとして使用する
cs = EV3RT::ColorSensor.new(EV3RT::PORT_2)  # センサポート2番をカラーセンサとして使用する
ss = EV3RT::SonarSensor.new(EV3RT::PORT_3)  # センサポート3番を超音波センサとして使用する
gs = EV3RT::GyroSensor.new(EV3RT::PORT_4)   # センサポート4番をジャイロセンサとして使用する

loop {
  # センサ値をログに出力する
  log("ts.pressed?:#{ts.pressed?}\n")   # タッチセンサ押下状態
  log("cs.rgb:#{cs.rgb}\n")             # RGB値
  log("ss.distance:#{ss.distance}\n")   # 緑色成分
  log("gs.angler_velocity:#{gs.angler_velocity}\n") # 角速度

  EV3RT::Task.delay(1000) # 1秒待つ
}
```
