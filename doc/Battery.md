# EV3RT::Buttery - バッテリクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3本体に搭載されているバッテリの状態を取得するクラスです。  
ETロボコン大会のシミュレータ環境では常に固定値が返されます。

## メソッド一覧

|メソッド|概要|シミュレータ対応|備考|
|:--|:--|:-:|:--|
|batteryVoltage<br/>battery_voltage|バッテリ電圧を取得します。|○||
|batteryCurrent<br/>battery_current|バッテリ電流を取得します。|○||

## メソッド概要

---

### EV3RT::Battery.batteryVoltage => Integer
### EV3RT::Battery.battery_voltage => Integer

バッテリ電圧\[mV\]を取得します。  
ETロボコン大会のシミュレータ環境では常に固定値が返されます。

#### 引数

なし

#### 戻り値

ミリボルト単位のバッテリ電圧値を返します。

---

### EV3RT::Battery.batteryCurrent => Integer
### EV3RT::Battery.battery_current => Integer

バッテリ電流\[mA\]を取得します。  
ETロボコン大会のシミュレータ環境では常に固定値が返されます。

#### 引数

なし

#### 戻り値

ミリアンペア単位のバッテリ電流値を返します。

---

## 使用例

```ruby
# 10秒毎にバッテリ状態をログ出力する
loop {
  log("#{EV3RT::Battery.batteryVoltage / 1000.0} [V]\n")  # 電圧[V]
  log("#{EV3RT::Battery.batteryCurrent / 1000.0} [A]\n")  # 電流[A]
  EV3RT::Task.delay(10000)   # 10秒待つ
}
```
