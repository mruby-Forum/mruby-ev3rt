# EV3RT::Task - タスククラス

<table width="100%"><tr><td align="right"><a href="README.md">戻る</a></td></tr></table>

[TOPPERS/EV3RT](https://dev.toppers.jp/trac_user/ev3pf/wiki/WhatsEV3RT) のタスクを制御するクラスです。

## メソッド一覧

### 

|メソッド|概要|シミュレータ対応|備考|
|:--|:--|:-:|:--|
|new|タスクオブジェクトを生成します。|○||
|sleep(tmo)|自タスクを起床待ち状態にします。|○||
|delay(dlytim)|自タスクを遅延させます。|○||
|wakeup(tskid)|指定したタスクを起床させます。|○||
|active(tskid)|指定したタスクを起動します。|○||
|exit|自タスクを終了させます。|○||
|start_cyclic(cycid)|周期ハンドラを動作開始します。|○||
|stop_cyclic)cycid)|周期ハンドラを停止します。|○||

## メソッド概要

---

### EV3RT::Task.new => EV3RT::Task

タスクオブジェクトを生成します。

#### 引数

なし

#### 戻り値

生成したタスクオブジェクトを返します。

---

### EV3RT::Task#sleep(tmo) => self

`TOPPERS/EV3RT` の `tslp_tsk` または `slp_tsk` を呼び出して、自タスクを起床待ち（スリープ）状態にします。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|tmo|タイムアウト時間 \[ミリ秒\]|引数を省略した場合はタイムアウトなし（無限起床待ち）となります。|

#### 戻り値

`self` を返します。

---

### EV3RT::Task#delay(dlytim) => self

`TOPPERS/EV3RT` の `dly_tsk` を呼び出して、指定した時間、自タスクを遅延させます。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|dlytim|遅延時間 \[ミリ秒\]||

#### 戻り値

`self` を返します。

---

### EV3RT::Task#wakeup(tskid) => self

`TOPPERS/EV3RT` の `wup_tsk` を呼び出して、指定したタスクを起床させます。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|tskid|起床させるタスクのID番号||

#### 戻り値

`self` を返します。

---

### EV3RT::Task#active(tskid) => self

`TOPPERS/EV3RT` の `act_tsk` を呼び出して、指定したタスクを起動します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|tskid|起動するタスクのID番号||

#### 戻り値

`self` を返します。

---

### EV3RT::Task#exit => self

`TOPPERS/EV3RT` の `ext_tsk` を呼び出して、自タスクを終了します。

#### 引数

なし

#### 戻り値

`self` を返します。

---

### EV3RT::Task#start_cyclic(cycid) => self

`TOPPERS/EV3RT` の `sta_cyc` を呼び出して、周期ハンドラを動作開始します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|cycid|動作開始する周期ハンドラのID番号||

#### 戻り値

`self` を返します。

---

### EV3RT::Task#stop_cyclic(cycid) => self

`TOPPERS/EV3RT` の `stp_cyc` を呼び出して、周期ハンドラを停止します。

#### 引数

|引数|概要|備考|
|:--|:--|:--|
|cycid|停止する周期ハンドラのID番号||

#### 戻り値

`self` を返します。

---

## 使用例

```ruby
n = 1

loop {
  # RGB色成分値をログに出力する
  log("n:#{n}\n")
  n += 1

  EV3RT::Task.delay(1000) # 1秒待つ
}
```
