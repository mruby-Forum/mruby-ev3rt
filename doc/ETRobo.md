# EVRobo - ETRoboクラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

ETロボコン競技に関する機能を提供するクラスです。  
ETロボコンシミュレータ上で動作しますが、EV3実機では動作しません。

## 定数一覧

### コース情報

|定数|概要|備考|
|:--|:--|:--|
|CARD_NUMBER|数字カードの数字|ETロボコンシミュレータ2020向け|
|BLOCK_NUMBER|黒ブロックに書かれた数字|ETロボコンシミュレータ2020向け|
|BLOCK_POS_START|ブロック位置情報指定の開始||
|BLOCK_POS_BLACK1|黒ブロック1の位置情報||
|BLOCK_POS_BLACK2|黒ブロック2の位置情報||
|BLOCK_POS_RED1|赤ブロック1の位置情報||
|BLOCK_POS_RED2|赤ブロック2の位置情報||
|BLOCK_POS_YELLOW1|黄ブロック1の位置情報||
|BLOCK_POS_YELLOW2|黄ブロック2の位置情報||
|BLOCK_POS_BLUE1|青ブロック1の位置情報||
|BLOCK_POS_BLUE2|青ブロック2の位置情報||
|BLOCK_POS_GREEN1|緑ブロック1の位置情報||
|BLOCK_POS_GREEN2|緑ブロック2の位置情報||
|BLOCK_POS_END|ブロック位置情報指定の終了||

## メソッド一覧

### クラスメソッド

|メソッド|概要|シミュレータ対応|備考|
|:--|:--|:-:|:--|
|notify_completed||○||
|get_course_info(info)||○||

## メソッド概要

---

### ETRobo.notify_completed => self

ETロボコンシミュレータに競技が終了したことを通知します。  
従来のETロボコン大会における「リタイヤ宣言」に相当します。

#### 引数

なし

#### 戻り値

`self` を返します。

---

### ETRobo.get_course_info(info) => Integer

ETロボコン2021のアドバンストクラスの競技（ブロックビンゴ2021）で使用するコース情報を取得します。  
ETロボコンのアドバンストクラスのみで有効です。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|info|取得するコース情報のID|以下が指定できます。<li>ETRobo::BLOCK_POS_BLACK1: 黒ブロック1の位置<li>ETRobo::BLOCK_POS_BLACK2: 黒ブロック2の位置<li>ETRobo::BLOCK_POS_RED1: 赤ブロック1の位置<li>ETRobo::BLOCK_POS_RED2: 赤ブロック2の位置<li>ETRobo::BLOCK_POS_YELLOW1: 黄ブロック1の位置<li>ETRobo::BLOCK_POS_YELLOW2: 黄ブロック2の位置<li>ETRobo::BLOCK_POS_BLUE1: 青ブロック1の位置<li>ETRobo::BLOCK_POS_BLUE2: 青ブロック2の位置<li>ETRobo::BLOCK_POS_GREEN1: 緑ブロック1の位置<li>ETRobo::BLOCK_POS_GREEN2: 緑ブロック2の位置|

#### 戻り値

`info` で指定したブロック位置情報を文字で返します。

##### 交点サークルの場合  
ブロックが配置されたサークル位置として以下の文字が返されます。

```
A  B  C  D
E  F  G  H
J  K  L  M
P  Q  R  S
```

##### 橋点サークルの場合  
'0' が返されます。

##### ブロックサークル、センターマークの場合  
ブロックが配置された位置として以下の文字が返されます。

```
1  2  3
4  9  5
6  7  8
```

---

## 使用例

```ruby
n = 0

loop {
  EV3RT::Task.delay(1000) # 1秒待つ

  # 5秒たったらコース情報をログ出力する
  if n == 5
    log("BLACK1 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_BLACK1)}")
    log("BLACK2 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_BLACK2)}")
    log("RED1 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_RED1)}")
    log("RED2 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_RED2)}")
    log("YELLOW1 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_YELLOW1)}")
    log("YELLOW2 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_YELLOW2)}")
    log("BLUE1 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_BLUE1)}")
    log("BLUE2 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_BLUE2)}")
    log("GREEN1 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_GREEN1)}")
    log("GREEN2 pos: #{ETRobo.get_course_info(ETRobo::BLOCK_POS_GREEN2)}")
  end

  # 10秒たったら終了通知する
  n += 1
  if n == 10
    ETRobo.notify_completed
  end
}
```
