# EV3RT::ColorSensor - カラーセンサクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3のカラーセンサを使用して色計測を行うクラスです。

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
|new(port)|指定したポートのカラーセンサオブジェクトを生成します。|○||
|brightness|カラーセンサで反射光の強さを取得します。|○||
|rgb|カラーセンサで計測したRGB値を取得します。|○||
|rgb_part(n)|カラーセンサで計測したRGB値のうち、指定した色成分の値を取得します。|○||
|color|カラーセンサで色を識別します。|×||

## メソッド概要

---

### EV3RT::ColorSensor.new(port) => EV3RT::ColorSensor

指定したポートのカラーセンサオブジェクトを生成します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|port|EV3のセンサポート番号|EV3RT::PORT_1 〜 EV3RT::PORT_4 の範囲で指定します。|

#### 戻り値

生成したカラーセンサオブジェクトを返します。

---

### EV3RT::ColorSensor#brightness => Integer

カラーセンサで反射光の強さを取得します。

#### 引数

なし

#### 戻り値

取得した反射光の強さ（0〜100）を返します。

---

### EV3RT::ColorSensor#rgb => Array

カラーセンサで計測したRGB値を取得します。

#### 引数

なし

#### 戻り値

カラーセンサから取得したRGB値をIntegerの配列 `[R, G, B]` で返します。

---

### EV3RT::ColorSensor#rgb_part(n) => Integer

カラーセンサで計測したRGB値を取得します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|n|取得する色成分を `EV3RT::ColorSensor::R`, `EV3RT::ColorSensor::G`, `EV3RT::ColorSensor::B` のいずれかで指定します。||

#### 戻り値

カラーセンサから取得したRGB値の指定色成分をIntegerで返します。

---

## 使用例

```ruby
# カラーセンサオブジェクト生成
cs = EV3RT::ColorSensor.new(EV3RT::PORT_2)  # センサポート2番をカラーセンサとして使用する

loop {
  # RGB値を取得する
  rgb = cs.rgb

  # RGB色成分値をログに出力する
  log("R:#{rgb[EV3RT::ColorSensor::R]}\n")  # 赤色成分
  log("G:#{rgb[EV3RT::ColorSensor::G]}\n")  # 緑色成分
  log("B:#{rgb[EV3RT::ColorSensor::B]}\n")  # 青色成分

  EV3RT::Task.delay(1000) # 1秒待つ
}
```
