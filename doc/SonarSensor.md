# EV3RT::SonarSensor - 超音波センサクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3の超音波センサを使用して色計測を行うクラスです。

## メソッド一覧

|メソッド|概要|備考|
|:--|:--|:--|
|new(port)|指定したポートの超音波センサオブジェクトを生成します。||
|distance|超音波センサで検知した物体までの距離を取得します。||

## メソッド概要

---

### EV3RT::SonarSensor.new(port) => EV3RT::SonarSensor

指定したポートの超音波センサオブジェクトを生成します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|port|EV3のセンサポート番号|EV3RT::PORT_1 〜 EV3RT::PORT_4 の範囲で指定します。|

#### 戻り値

生成した超音波センサオブジェクトを返します。

---

### EV3RT::SonarSensor#distance => Integer

超音波センサで検知した物体までの距離を取得します。

#### 引数

なし

#### 戻り値

超音波センサで検知した物体までの距離\[cm\]

−−−

## 使用例

```ruby
# 超音波センサオブジェクト生成
ss = EV3RT::SonarSensor.new(EV3RT::PORT_3)  # センサポート3番を超音波センサとして使用する

loop {
  # 超音波センサから取得した距離をログに出力する
  log("dist:#{ss.distance}[cm]\n")

  EV3RT::Task.delay(1000) # 1秒待つ
}
```
