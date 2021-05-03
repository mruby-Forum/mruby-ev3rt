# EV3RT::TouchSensor - タッチセンサクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3のタッチセンサの状態を取得するクラスです。

## メソッド一覧

### 

|メソッド|概要|シミュレータ対応|備考|
|:--|:--|:-:|:--|
|new(port)|指定したポートのタッチセンサオブジェクトを生成します。|○||
|pressed?|タッチセンサの押下状態を取得します。|○||

## メソッド概要

---

### EV3RT::TouchSensor.new(port) => EV3RT::TouchSensor

指定したポートのタッチセンサオブジェクトを生成します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|port|EV3のセンサポート番号|EV3RT::PORT_1 〜 EV3RT::PORT_4 の範囲で指定します。|

#### 戻り値

生成したタッチセンサオブジェクトを返します。

---

### EV3RT::TouchSensor#pressed? => true|false

タッチセンサの押下状態を取得します。  
ETロボコン大会のシミュレータ環境ではキーボードの `Enterキー` を押下することでタッチセンサが反応します。

#### 引数

なし

#### 戻り値

- `true`: タッチセンサが押下されている
- `false`: タッチセンサが押下させていない

---

## 使用例

```ruby
# タッチセンサオブジェクト生成
ts = EV3RT::TouchSensor.new(EV3RT::PORT_1)  # センサポート1番をタッチセンサとして使用する

# モータオブジェクト生成
m_r = EV3RT::Motor.new(EV3RT::PORT_B, EV3RT::LARGE_MOTOR) # 右モータ：モータポートBを使用
m_l = EV3RT::Motor.new(EV3RT::PORT_C, EV3RT::LARGE_MOTOR) # 左モータ：モータポートCを使用

loop {
  # タッチセンサが押下されるのを待つ
  until ts.pressed?
    EV3RT::Task.delay(100) # 100ミリ秒待つ
  end

  # 1秒間前進する
  m_r.power = 20
  m_l.power = 20
  EV3RT::Task.delay(1000) # 1秒待つ
  m_r.stop
  m_l.stop
}
```
