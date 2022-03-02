# mrubyサンプルの解説（制御編）

このドキュメントでは、[ETロボコンシミュレータ向け EV3RT用mruby サンプルコード](https://github.com/ETrobocon/etrobo/tree/master/dist/sample_mruby)について、プログラム言語に依存しない制御の部分の考え方について解説します。

コードはRubyですが、C/C++など他の言語の場合でも参考にしてください。（Rubyを知らなくても、なんとなく読めるでしょう）


## サンプルの概要

これはライントレースのサンプルです。

カラーセンサーの読み取り値を使って、「ライン上を走れているかチェックし、ラインから外れそうになっていたら軌道修正する」ということをしています。

実現方法はいくつもありますが、ここでは、その中でも理解しやすくシンプルな方法を使っています。

（もっと速く確実に走行できる方法を調べて、実践してくださいね）

このサンプルでは、TOPPERSのタスクを２つ使っています。

mainタスクとtracerタスクです。

tracerタスクは周期ハンドラより10ms周期で「起床(wakeup)」されます。

ここでは、tracerタスクの中で行われているライントレースについて解説します。

## ライントレース

まず、ライントレースのしかたを理解しましょう。

```ruby
color = @color_sensor.rgb_part(EV3RT::ColorSensor::R)
if color >= threshold
 右に外れているか、外れかけている状態と判断して、
 ちょっと左に曲がりつつ前進するような値をleft_powerにセットする
else
 左に外れているか、外れかけている状態と判断して、
 ちょっと右に曲がりつつ前進するような値をright_powerにセットする
end
@left_motor.power = left_power
@right_motor.power = right_power
```

カラーセンサーで読み取った値がthreshold以上の場合は左旋回、それ以外は右旋回するようにleft_power、right_powerの値を作成し、それらを引数としてleft_motorとright_motorのpowerを呼び出します。

(Rubyではこんな書き方ができます。詳しくは末尾の参考情報を参照してください)

大袈裟に書くとこんな感じで蛇行しながら走ります。

<img src="https://github.com/mruby-Forum/mruby-ev3rt/raw/master/doc/images/sample-guide-control-lintrace.png" alt="ライントレースの様子の画像" width="600" />


図1: ライントレースの様子


### カラーセンサーの値を読む

まず、カラーセンサーの値を読む部分を見てみましょう。

```ruby
color = @color_sensor.rgb_part(EV3RT::ColorSensor::R)
```

カラーセンサーの値の読み取りに ```rgb_part``` メソッドを使っています。

このメソッドは内部でEV3RTのAPI、 ```ev3_color_sensor_get_rgb_raw()``` を呼び出し、RGB値を読み取り、引数で指定したRGBいずれかの値を返します。

RGBのRの値が黒と青では大差ないため、黒線から青線に切り替わっても影響なく走れるためRを使用しました。

GやBの場合はどうなるでしょうか？

もちろん、GまたはBの方が安定して走れそうと判断したらそちらを使ってください。

注）閾値threshold は最適化されていません。より良い値に調整してください。

サンプルコードのコメント内にあるように、EV3RTのAPI、ev3_color_sensor_get_reflect()を呼び出すbrightnessメソッドを使う方法もあります。こちらは反射光の強さを測定します。

実機の場合、こちらの方が計測にかかる時間が短いというメリットがあります。

RGB値の方がラインが黒と青の違いに影響されにくいため、本サンプルではrgb_partを使う例を紹介しました。

brightnessメソッドでもパラメータをうまく調整すると黒線も青線もトレースできるかもしれません。

また、青線のところでは閾値を青線用にしたり、青線のところだけrgb_partに切り替える、なども考えられるでしょう。

こっちの方がスムーズに走れるぞ。という方法が見つかったら、そちらを使ってください。

各パラメータは最適化できていないので、みなさんでチューニングしてください。

#### mruby用の参考情報

rgb_partメソッドの他にrgbメソッドがあり、こちらはRGBすべてをarrayの形で返します。

arrayのため、若干メモリ利用量が増え、GC：Garbage collection（ガベージコレクション）が起きやすくなります。

そのため、RGBのうち１つだけで良い場合はrgb_partを使う方が良いでしょう。

	
#### モーターに指示する値（left_powerの値）を求める

続いて、モーターに指示する値を求めている部分を見てみます。

```ruby
if color >= threshold
  left_power = forward - turn * edge
  right_power = forward + turn * edge
else
  left_power = forward + turn * edge
  right_power = forward - turn * edge
end

@left_motor.power = left_power
@right_motor.power = right_power
```

この走行体では、左右のタイヤそれぞれにモーターがついていて、各モーターに操作量を指示して動作させます。

ここでは、左右のタイヤを同じ量だけ正転、反転させることによって、走行体を旋回させています。

ひとまず「* edge」は横に置いておいて、「left_power = forward - turn」の部分について考えてみましょう。

```ruby
left_power = forward - turn
左モーターの操作量として指示する値 = 前進量 - 回転量
```

モーターの操作量は、EV3のモーターを最大の速さで正転する速度を100(%)、停止している時を0(%)、最大の速さで逆転している時を-100(%)として、-100〜100の範囲で指定します。

ここではforwardもturnも定数で指定しているためこの範囲におさまっていますが、みなさんが作成する時には前進量 - 回転量の計算結果がこの範囲になるように注意してください。

前進量も回転量も暫定値です。スムーズに速く走れるよう、最適値を考えてください。

（もちろん、最初から最後まで一定値である必要はありません）

### edge について

edgeとは黒線と白地の境界線のことを意味します。

黒線の両端のどちらを辿るかによって、ラインを外れたと気づいた時に左右どちらに旋回するとライン上に戻れるかが異なりますね。

ETロボコンでは、ライトコースとレフトコースの２種類のコースを走ります。

そのため、各コースの旋回方向を指定する値として、RIGHT_COURCE、LEFT_COURCEという名前の２つの定数を作っています。

ちなみに、基本的にはライントレースは左端と右端のどちらをトレースしても良いのですが、ETロボコンでは、コースに分岐があるなど、どちらをトレースするかが決まっていることが多いです。


<img src="https://github.com/mruby-Forum/mruby-ev3rt/raw/master/doc/images/sample-guide-control-edge.png" alt="左エッジと右エッジどちらをトレースするかによってルートが変わる例の画像" width="600" />

図2: 左エッジと右エッジどちらをトレースするかによってルートが変わる例


さて、このedgeがプログラムでどのように使われているか見てみましょう。

edgeには+1または-1が指定されると想定したプログラムになっています。

冗長に書くと以下のようになります。（turnが加算か減算かに着目してください）

edge使って+と-を入れ替えできるようにして、コードの冗長性をなくしています。コードが短くなるとテストも減らせてうれしいですね。

```ruby
if edge == 1  # left コース
  if color >= threshold
    left_power = forward - turn
    right_power = forward + turn
  else
    left_power = forward + turn
    right_power = forward - turn
  end
elsif edge== -1 # rightコース
  if color >= threshold
    left_power = forward + turn
    right_power = forward - turn
  else
    left_power = forward - turn
    right_power = forward + turn
  end
end
```

### turnについて

このサンプルでは、左右のタイヤの動作量(forward)に対してturnを加減算することにより、2つのタイヤの車軸の中点を軸にして旋回するようになっています。
（forward=0の場合が考えやすそうです）

### もっと改良してみよう

このままでは「ガタガタしてかっこ悪い走り」と言われるので、スムーズに走れるように工夫しましょう。

また、工夫するにつれてコードが複雑になってきます。

走行制御をするクラスやメソッドとして作ると、読みやすく、リファクタリングしやすいコードになるでしょう。


### 参考情報

#### @left_motor.power = left_power の構文について

Rubyを知らない方だと @left_motor.power = left_power をみて、変数に値を入れているだけなのに、なぜモーターがまわせるの？と不思議に思うかもしれません。

「power=」がメソッド名ですが、「power =」のようにスペースを入れて書くこともできます。

（これは、英語では assignment method といい、日本語だと、「代入風メソッド」 https://kikakurui.com/x3/X3017-2013-01.html や「属性参照」https://docs.ruby-lang.org/ja/latest/doc/spec=2foperator.html と呼ばれているようです）

この構文は内部変数に値を保存するだけでなく、メソッドを実行することもできます。

power= では、渡された値を使って、EV3RTのAPIを呼び出しているため、モーターを回すことができるのです。

以下に簡単な例を示します。

```ruby
def power=(val)
  If val == 0
    モーターを止める
   else
    与えられた操作量でモーターを動かす
  end
end
```

#### C言語用サンプルコード( sample_c4 )で使用しているev3_motor_steer() について

C言語用のサンプル、sample_c4では、ev3_motor_steer() を使って走行体を旋回させています。


こちらは引数で指定したturn値だけ片方のタイヤを多く回転させています。


EV3RTのAPIには、ev3_motor_steer() があります。

これは引数に左右のモーターとfowardとturnを渡すと、左右のモーターを制御してくれます。

詳しくはAPIの説明を参照してください。
https://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3motor.html#ga772dadf1354388584c36ba26f3baf7cf

このサンプルでev3_motor_steer() を使わなかった理由として、etrobo環境のwikiの「ETロボコンシミュレータにおけるEV3RTのAPI対応」の「モーター」のところに（以下のリンク参照）
https://github.com/ETrobocon/etrobo/wiki/api_ev3rt_on_athrill#%E3%83%A2%E3%83%BC%E3%82%BF%E3%83%BC

> ev3_motor_steer() : 対応はしていますが、実機との実現方法が異なるのと、細かい制御ができないため、使用しない方が良いでしょう。

とあるため、使用しませんでした。

ちなみに、このAPIの内部では、一方のモーターには、fowardにturnを加工した値を加算または減算、もう一方のモーターにはfowardのみを指定するようになっています。

そのため、「fowardのみ」を指定した側のモーターを軸として旋回するようになります。
このサンプルと動き方が異なりますので、注意してください。

[シミュレータのev3_motor_steer()の実装](
https://github.com/toppers/ev3rt-athrill-ARMv7-A/blob/70654150aa087ed4cf0140a9031e646b141a08e6/sdk/common/ev3api/src/ev3api_motor.c#L240 )


以上。
