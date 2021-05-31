# EV3RT::GyroSensor - ジャイロセンサクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3のジャイロセンサを使用して角速度計測を行うクラスです。

## 定数一覧

|定数|概要|備考|
|:--|:--|:--|
|R|RGBの赤色成分|rgb_partメソッドで使用します。|
|G|RGBの緑色成分|同上|
|B|RGBの青色成分|同上|

## メソッド一覧

### 

|メソッド|概要|シミュレータ対応|備考|
|:--|:--|:-:|:--|
|new(port)|指定したポートのジャイロセンサオブジェクトを生成します。|○||
|angler_velocity|ジャイロセンサで角速度を取得します。|○||
|reset|ジャイロセンサの角位置をリセットします。|○||

## メソッド概要

---

### EV3RT::GyroSensor.new(port) => EV3RT::GyroSensor

指定したポートのジャイロセンサオブジェクトを生成します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|port|EV3のセンサポート番号|EV3RT::PORT_1 〜 EV3RT::PORT_4 の範囲で指定します。|

#### 戻り値

生成したジャイロセンサオブジェクトを返します。

---

### EV3RT::GyroSensor#angler_velocity => Integer

ジャイロセンサで角速度 \[度/秒\] を取得します。

#### 引数

なし

#### 戻り値

取得した角速度 \[度/秒\] を整数値で返します。

---

### EV3RT::GyroSensor#reset => self

ジャイロセンサの角位置をリセットします。

#### 引数

なし

#### 戻り値

`self` を返します。

---

## 使用例

```ruby
# ジャイロセンサオブジェクト生成
gs = EV3RT::GyroSensor.new(EV3RT::PORT_4)  # センサポート4番をジャイロセンサとして使用する

loop {
  # 角速度をログに出力する
  log("Rangler_velocity:#{gs.angler_velocity}\n")

  EV3RT::Task.delay(1000) # 1秒待つ
}
```
