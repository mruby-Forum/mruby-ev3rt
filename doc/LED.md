# EV3RT::LED - LEDクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3本体ののLEDを制御するクラスです。  
ETロボコン大会のシミュレータ環境ではシミュレータ上のロボットのLED色が変化します。

## メソッド一覧

### 

|メソッド|概要|シミュレータ対応|備考|
|:--|:--|:-:|:--|
|new|LEDオブジェクトを生成します。|○||
|on(color)|指定した色でLEDを点灯します。|○||
|off|LEDを消灯します。|○||

## メソッド概要

---

### EV3RT::LED.new => EV3RT::LED

LEDオブジェクトを生成します。

#### 引数

なし

#### 戻り値

生成したLEDオブジェクトを返します。

---

### EV3RT::LED#on(color) => self

指定した色でLEDを点灯します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|color|LED点灯色<li>`EVEV3RT3::LED_RED`: 赤に点灯します。<li>`EV3RT::LED_GREEN`: 緑に点灯します。<li>`EV3RT::LED_ORANGE`: オレンジに点灯します。<li>`EV3RT::LED_OFF`: 消灯します。|`EV3RT::LED_OFF`を指定した場合は`EV3RT::LED#off`と同じ動作になります。|

#### 戻り値

`self` を返します。

---

### EV3RT::LED#off => self

LEDを消灯します。

#### 引数

なし

#### 戻り値

`self` を返します。

---

## 使用例

```ruby
# LEDオブジェクト生成
led = EV3RT::LED.new

# LED点灯パターン
colors = [EV3RT::LED_RED, EV3RT::LED_GREEN, EV3RT::LED_ORANGE]

loop {
  # 赤 → 緑 → オレンジ の順に点灯
  colors.each {|color|
    led.on(color)
    EV3RT::Task.delay(1000) # 1秒待つ
  }
  # LED消灯
  led.off
  EV3RT::Task.delay(1000) # 1秒待つ
}
```
