# mrubyサンプルの解説（TOPPERS編）

このドキュメントでは、[ETロボコンシミュレータ向け EV3RT用mruby サンプルコード](https://github.com/ETrobocon/etrobo/tree/master/dist/sample_mruby)について、TOPPERSのタスクからmrubyを使う方法、mrubyからTOPPERSのタスクを制御する(TOPPERSのAPIを呼び出す)方法について解説します。

ここではTOPPERS自体の解説はしないので、必要に応じて[TOPPERS第3世代カーネル（ITRON系）統合仕様書](https://www.toppers.jp/documents.html)（EV3RTに関してはリンク先のページのどれでも良いと思いますが、最新が無難じゃないでしょうか）を参照してください。

## 概要
mrubyをTOPPERSの上で動かす場合、TOPPERSの各タスクの上でmrubyが動きます。

タスク毎に別のmrubyが動いています。（現時点では、異なるタスク間で１つのmrubyを共有することはおそらく不可能と思います。）

そのため、違うタスクで動いているmrubyとグローバル変数を共有したりすることはできず、TOPPERSの機能を利用してデータをやりとりする必要があります。

また、mrubyはC/C++より多くのRAM領域を必要とします。

実機の場合はRAMを増やす必要がありますが、ETロボコンシミュレータの場合は既に充分な領域が確保されていますので、ほとんどの場合、増やす必要はないでしょう。（RAMの増やし方についてはAthrillの資料を参照してください。）

### TOPPERSからmrubyを使う際のコツ

処理時間やRAM使用量を考慮して以下のようにしています。

* 走行開始までに各タスクのmrubyの初期化(initialize)を終わらせておく。
  
  mainタスク内ですべてのタスクの初期化を終わらせ、各タスクは起床待ち(sleep)状態にしておきます。

* 周期ハンドラを使って繰り返し実行されるものは都度オブジェクトを生成しない
  
  周期ハンドラから呼ばれる際には起動(activate)ではなく、起床(wake up)を使っています。
  
  起動(activate)では毎回 initialize から開始しますが、起床(wake up)の場合は前回sleep(TOPPERSのAPIだとslp_tsk)したところの続きから始めることができるため、周期的に呼ばれた際に前回使用したオブジェクトの使い回しができます。


詳しくは以下のコードの説明部分を参照してください。




## サンプルコードの概要

このサンプルは、10ミリ秒周期でセンサーの値を読み取りながらライントレースをします。

MAIN_TASKと
TRACER_TASKの２つのタスクがあります。
MAIN_TASKタスクでは、mrubyのMainTaskクラス、TRACERE_TASKタスクでは、mrubyのTraceerTaskクラスを実行しています。

MainTaskは初期化を行い、開始コマンドを待ちます。開始コマンドを受け取ったら、周期ハンドラを起動します。

TracerTaskは周期ハンドラから実行されます。ライン上を走行しているかチェックし、軌道修正を行います。

### タスク定義(cfg)

```app.cfg```のうち、mrubyのタスクに関係する部分は以下の3行です。

```
CRE_TSK(MAIN_TASK, { TA_ACT , 0, main_task, TMIN_APP_TPRI + 1, STACK_SIZE, NULL });
CRE_TSK(TRACER_TASK  , { TA_NULL, 0, tracer_task, TMIN_APP_TPRI, STACK_SIZE, NULL });
CRE_CYC(CYC_TRACER, { TA_NULL, { TNFY_WUPTSK, TRACER_TASK }, 10U * 1000U, 0U });
```

細かい説明は省略しますが、この定義から、

* 起動するとMAIN_TASKタスクが実行される。
* TRACER_TASKタスクは、「TRACER_TASKタスクを実行するための周期ハンドラ」が開始されると、10ミリ秒周期で実行される。

ということがわかります。
(cfgファイルの書き方(TOPPERSのタスクの定義のしかた)はC/C++と同じです。C/C++向けの資料も参考にできます)

ここで、 MAIN_TASK の優先度が ```TMIN_APP_TPRI + 1``` 、TRACER_TASK の優先度が ```TMIN_APP_TPRI``` で「MAIN_TASKよりもTRACER_TASKの方が優先度が高い」ことに注意してください。

### TOPPERSのタスクとmrubyの関係

このサンプルでは、TOPPERSのMAIN_TASKタスクはmrubyのMainTaskクラス、TOPPERSのTRACER_TASKタスクはmrubyのTracerTaskクラスを実行するためのものです。

### main_task.rb

MainTaskクラスは MAIN_TASKタスクから実行されます。つまり、最初に実行されるクラスです。

MainTaskクラスが実行されると、initializeメソッド、executeメソッドの順に実行されます。

モーターやセンサーの初期化は initializeメソッド内で行います。(コードは省略)


```ruby
  def execute
    # 初期処理だけして寝かす
    EV3RT::Task.active(EV3RT::TRACER_TASK)

    # スタート待機
    while true
      if @touchSensor.pressed?
        log("-- pressed! --")
        break
      end

      EV3RT::Task.sleep(10)
    end	# loop end

    # 周期タスクを開始する
    EV3RT::Task.start_cyclic(EV3RT::CYC_TRACER)

(以下略)
```

executeメソッドでは、以下のことを行っています。

(1) TRACER_TASKを起動(activate)にする


```EV3RT::Task.active(EV3RT::TRACER_TASK)```でTRACER_TASKを起動します。

（TRACER_TASK内で実行されている、mrubyのTracerTaskは前準備ができたら起床待ち(sleep)状態になり、周期ハンドラが開始されるのを待ちます）

TOPPERSのタスク定義では、MAIN_TASKよりTRACER_TASKの方が優先度が高いため、TRACER_TASKが起動(activate)されるとTRACER_TASKが動き始めtracer_taskクラスが実行されます。MAIN_TASKは待ち状態になりますが、TRACER_TASKが起床待ち(sleep)になるとMAIN_TASKが再開されて、中で実行されているmrubyのコードの続きが実行されます。

(2)スタート待機

TracerTaskクラスの前準備部分が終わったら、MainTaskクラスのタッチセンサー押下のチェックが始まります。

「スタート待機」はETロボコンシミュレータの実行開始コマンドとして、「タッチセンサー押下」が来るので、それを待っています。10ミリ秒ごとに「タッチセンサー押下」が来たかチェックをしています。

「タッチセンサー押下」が来るとスタートします。

(3)周期ハンドラ開始

```  EV3RT::Task.start_cyclic(EV3RT::CYC_TRACER) ```を使って周期ハンドラを開始します。（周期ハンドラから呼ばれているTRACER_TASKタスクで実行されているmrubyのTracerTaskクラスもこの時点から周期的に動作するようになります）

### tracer_task.rb

TracerTaskクラスのexecuteメソッドは以下のようになります。

```ruby
def execute
  #走行モータエンコーダリセット
  @left_motor.reset
  @right_motor.reset
  edge = LEFT_COURCE  # 左右どちらのコースを走行するかの指定
  EV3RT::Task.sleep #・・・(1)

  while true
    # left_powerとright_powerの値を計算する
    （略）
    # 左右モータでロボットのステアリング操作を行う
    @left_motor.power = left_power
    @right_motor.power = right_power

    # 周期ハンドラからwake upされるのを待つ
    EV3RT::Task.sleep #・・・(2)
  end
end
```

まず、```EV3RT::Task.sleep```に着目します。２箇所ありますね。

TracerTaskはまず、MainTask内で起動(activate)されます。

そうすると、initizlizeメソッドが実行され、続いてexecuteメソッドが実行されます。

すると、```EV3RT::Task.sleep #・・・(1)``` に到達して起床待ち（sleep）状態になります。

sleepしていると、周期ハンドラから起床(wake up)されます。

起床(wake up)されると、起床待ち(sleep)していたところの次の行から開始されます。

whileループに入り、left_powerとright_powerの値を計算して、計算結果を左右モーターに設定します。

すると、今度は```EV3RT::Task.sleep #・・・(2)``` に到達して起床待ち（sleep）状態になります。

起床待ち(sleep)していると、周期ハンドラから起床(wake up)されます。

起床(wake up)されると、起床待ち(sleep)したところの次の行から開始されます。

つまり、whileループの先頭から実行されるので、left_powerとright_powerの値を計算して、計算結果を左右モーターに設定し、sleepします。
これを延々と繰り返します。

#### 起動(activate)ではなく起床(wake up)にする理由

ここで、周期ハンドラからTRACER_TASKを呼び出す際に、起動(activate, TOPPERSのAPIではact_tsk)を使わずに、起床(wake up, TOPPERSのAPIではwup_tsk)を使ったのはなぜでしょうか？

act_tskの場合は、タスクの先頭から実行されるため、毎回initializeから始まることになります。

時間もかかるし、毎回オブジェクトを生成して破棄しているとGC対象が増えて、GCが起きやすく実行時間もかかります。
そのため、このようにしています。

## ログについて

ログ出力については、以下のようにコメントアウトされています。

```ruby
# カラーセンサーで取得した値をターミナルに出力
# 操作に悪影響しないように、ステアリング操作が終了してからログを出すようにした
# 実機と違い、ログを出すと走行に影響を及ぼす可能性が高い為コメントアウトしている
# log("color:#{color.to_s}, left:#{left_power.to_s}, right:#{right_power.to_s}")
# log("c:#{color.to_s}, l:#{left_power.to_s}")
# log("c:#{color.to_s}")
```
実機の場合はBluetooth経由でPCにログを送信していましたが、ETロボコンシミュレータではTOPPERSのsyslogを使ってログを出すようにしています。

実機と同じ感覚でログを出しているとログ出力が走行に影響しますので注意してください。
サンプルコードでログの部分をコメントアウトしているのは、そのためです。



## 上級編：C言語からmrubyを実行する方法

各タスクで実行するコードは ```app.c``` に定義しています。

```C
	static mrb_state *mrb = NULL;
	mrb_value ret;
	mrb = mrb_open();
```
``` mrb_open(); ``` で mruby VM（virtual machine）を生成します。生成したVMはmrbにあります。


```C
	struct RClass * ev3rt = mrb_class_get(mrb, "EV3RT");
  mrb_define_const(mrb, ev3rt, "MAIN_TASK", mrb_fixnum_value(MAIN_TASK));
	mrb_define_const(mrb, ev3rt, "TRACER_TASK", mrb_fixnum_value(TRACER_TASK));
	mrb_define_const(mrb, ev3rt, "CYC_TRACER", mrb_fixnum_value(CYC_TRACER));
```
これは、TOPPERSのタスク操作のAPIをmrubyから呼べるようにするためのしかけです。

EV3RTクラスにcfgファイルで設定したタスク名を追加します。

このおかげで、mrubyからTOPPERSのAPIが、例えばMAIN_TASKからCYC_TRACER周期ハンドラを起動したい場合は ``` EV3RT::Task.start_cyclic(EV3RT::CYC_TRACER) ``` という形で、呼び出すことができるようになります。



```C
  #include "main_task.h"

  ret = mrb_load_irep (mrb, bcode);
```
```main_task.h``` の中身は、 ```main_task.rb``` から生成されたmrubyのバイトコードです。

これは ```make app=アプリ名``` でビルドした時に``` generate_bytecode.rb ```が実行され、生成されます。

この関数で、main_task.rbのバイトコードの読み込みと実行が行われます。

```C
  if(mrb->exc){
    （以下略）
```

```if(mrb->exc){}``` の部分はmrubyが正常に実行できたかどうかのログを出力しています。

mruby内でエラーが起きた場合は、 ```#### mruby err msg: mrubyが出したエラーメッセージ ``` の形でsyslogに出力されます。

``` void tracer_task(intptr_t unused) ``` についても同様です。




