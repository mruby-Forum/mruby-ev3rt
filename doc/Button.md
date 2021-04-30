# EV3RT::Button - ボタンクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

EV3本体のボタンの状態を取得するクラスです。

## メソッド一覧

|メソッド|概要|備考|
|:--|:--|:--|
|new(n)|指定した番号のボタンオブジェクトを生成します。||
|pressed?|ボタンの押下状態を取得します。||

## メソッド概要

---

### EV3RT::Button.new(n) => EV3RT::Button

指定した番号のボタンオブジェクトを生成します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|port|EV3本体のボタン番号<li>`EV3RT::LEFT_BUTTON`: 左ボタン<li>`EV3RT::RIGHT_BUTTON`: 右ボタン<li>`EV3RT::UP_BUTTON`: 上ボタン<li>`EV3RT::DOWN_BUTTON`: 下ボタン<li>`EV3RT::ENTER_BUTTON`: 中央ボタン<li>`EV3RT::BACK_BUTTON`: 戻るボタン||

#### 戻り値

生成したボタンオブジェクトを返します。

---

### EV3RT::Button#pressed? => true|false

ボタンの押下状態を取得します。  
ETロボコン大会のシミュレータ環境では画面表示されるロボットの本体ボタンをマウスでクリックすることで反応します。

#### 引数

なし

#### 戻り値

- `true`: ボタンが押下されている
- `false`: ボタンが押下させていない

−−−

## 使用例

```ruby
# ボタンオブジェクト生成
b_u = EV3RT::Button.new(EV3RT::UP_BUTTON)   # 上ボタン
b_d = EV3RT::Button.new(EV3RT::DOWN_BUTTON) # 下ボタン

# モータオブジェクト生成
m_r = EV3RT::Motor.new(EV3RT::PORT_B, EV3RT::LARGE_MOTOR) # 右モータ：モータポートBを使用
m_l = EV3RT::Motor.new(EV3RT::PORT_C, EV3RT::LARGE_MOTOR) # 左モータ：モータポートCを使用

pwr = 10  # モータパワー初期値

loop {
  # pwr値を上ボタン押下で+10、下ボタン押下で-10する
  if b_u.pressed?
    pwr += 10 if pwr < 100
  elsif b_d.pressed?
    pwr -= 10 if pwr > -100
  end

  # 左右モータのパワーをセットする
  m_r.power = pwr
  m_l.power = pwr
  EV3RT::Task.delay(100) # 100ミリ秒待つ
}
```
