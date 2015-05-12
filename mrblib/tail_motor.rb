# class EV3RT
#   LARGE_MOTOR = 2
#   class Motor
#     def initialize(port, type)
#     end
#     def count
#       42
#     end
#     def pwm=(pwm_val)
#       @pwm = pwm_val
#     end
#   end
# end

class EV3RT
  class TailMotor < Motor

    attr_accessor :p_gain, :pwm_abs_max

#    def initialize(port, type = EV3RT::LARGE_MOTOR)
#      super(port, type)
#      @p_gain = 2.5       # 完全停止用モータ制御比例係数
#      @pwm_abs_max = 60   # 完全停止用モータ制御PWM絶対最大値
#    end

    def control(angle)
      pwm = (angle - count()) * @p_gain # 比例制御
      # PWM出力飽和処理
      if pwm > @pwm_abs_max
        pwm = @pwm_abs_max
      elsif pwm < -@pwm_abs_max
        pwm = -@pwm_abs_max
      end
      self.pwm = pwm
    end
  end
end


# TAIL_ANGLE_DOWN = 93	# 完全停止時の尻尾の角度
# TAIL_ANGLE_UP = 3	# バランス走行時の尻尾の角度
# GYRO_OFFSET = 0         # ジャイロセンサオフセット値(角速度0[deg/sec]時) 
# LIGHT_WHITE = 40	# 白色の光センサ値
# LIGHT_BLACK = 0		# 黒色の光センサ値
# P_GAIN = 2.5            # 完全停止用モータ制御比例係数
# PWM_ABS_MAX = 60        # 完全停止用モータ制御PWM絶対最大値
# tailMotor = EV3RT::TailMotor.new(0)
# tailMotor.p_gain = P_GAIN
# tailMotor.pwm_abs_max = PWM_ABS_MAX
# tailMotor.control(TAIL_ANGLE_DOWN)
# tailMotor.control(TAIL_ANGLE_UP)


