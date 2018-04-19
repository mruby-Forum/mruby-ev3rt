#include "mruby.h"
#include "mruby/array.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"
#include "serial.h"	// for Bluetooth(log)
#include "balancer.h"
#include "balancer_private.h"


//float K_F[4]
// @K_F0 車輪回転角度係数, @K_F1 車体傾斜角度係数, @K_F2 車輪回転角速度係数 ,@K_F3 車体傾斜角速度係数

static mrb_value
mrb_mruby_balancer_initialize(mrb_state *mrb, mrb_value self)
{
	// default gyro direction is normal
	mrb_int n1 = 1;
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@gyro_direction"), mrb_fixnum_value(n1));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pwm_L"), mrb_fixnum_value(0));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pwm_R"), mrb_fixnum_value(0));

	balance_init();
	return self;
}

//K_F[0]をセットする関数
static mrb_value
mrb_mruby_set_K_F0(mrb_state *mrb, mrb_value self)
{
	mrb_get_args(mrb, "f", &K_F[0]);
	return self;
}

//K_F[1]をセットする関数
static mrb_value
mrb_mruby_set_K_F1(mrb_state *mrb, mrb_value self)
{
	mrb_get_args(mrb, "f", &K_F[1]);
	return self;
}

//K_F[2]をセットする関数
static mrb_value
mrb_mruby_set_K_F2(mrb_state *mrb, mrb_value self)
{
	mrb_get_args(mrb, "f", &K_F[2]);
	return self;
}

//K_F[3]をセットする関数
static mrb_value
mrb_mruby_set_K_F3(mrb_state *mrb, mrb_value self)
{
	mrb_get_args(mrb, "f", &K_F[3]);
	return self;
}


//K_F[0]を取得する関数
static mrb_value
mrb_mruby_get_K_F0(mrb_state *mrb, mrb_value self)
{
	return mrb_float_value(mrb, (mrb_float)K_F[0]);
}

//K_F[1]を取得する関数
static mrb_value
mrb_mruby_get_K_F1(mrb_state *mrb, mrb_value self)
{
	return mrb_float_value(mrb, (mrb_float)K_F[1]);
}

//K_F[2]を取得する関数
static mrb_value
mrb_mruby_get_K_F2(mrb_state *mrb, mrb_value self)
{
	return mrb_float_value(mrb, (mrb_float)K_F[2]);
}

//K_F[3]を取得する関数
static mrb_value
mrb_mruby_get_K_F3(mrb_state *mrb, mrb_value self)
{
	return mrb_float_value(mrb, (mrb_float)K_F[3]);
}

static mrb_value
mrb_mruby_balancer_calculate(mrb_state *mrb, mrb_value self)
{

	mrb_int forward, turn, gyro, offset, motor_ang_l, motor_ang_r, volt;
	signed char pwm_L, pwm_R;
	mrb_get_args(mrb, "iiiiiii", &forward, &turn, &gyro, &offset, &motor_ang_l, &motor_ang_r, &volt);

	backlashCanceler(mrb, self, &motor_ang_l, &motor_ang_r);
	balance_control(
		(float)forward,
		(float)turn,
		(float)gyro,
		(float)offset,
		(float)motor_ang_l,
		(float)motor_ang_r,
		(float)volt,
		(signed char*)&pwm_L,
		(signed char*)&pwm_R);
	mrb_value ary = mrb_ary_new_capa(mrb, 2);
	mrb_ary_set(mrb, ary, 0, mrb_fixnum_value((mrb_int)pwm_L));
	mrb_ary_set(mrb, ary, 1, mrb_fixnum_value((mrb_int)pwm_R));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pwm_R"), mrb_fixnum_value((mrb_int)pwm_R));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pwm_L"), mrb_fixnum_value((mrb_int)pwm_L));

	return ary;
}


static mrb_value
mrb_mruby_balancer_calculate_auto(mrb_state *mrb, mrb_value self)
{

	mrb_int forward, turn, gyro, offset, motor_ang_l, motor_ang_r, volt, gyro_direction;
	signed char pwm_L, pwm_R;
	mrb_get_args(mrb, "iii", &forward, &turn, &offset);
	char msg[64];

	mrb_value gyro_sensor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@gyro"));
	sensor_port_t gyro_port = (sensor_port_t)mrb_fixnum(mrb_iv_get(mrb, gyro_sensor, mrb_intern_lit(mrb, "@port")));
	mrb_int rate = ev3_gyro_sensor_get_rate(gyro_port);

	mrb_value gyro_direction_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@gyro_direction"));
	gyro_direction = mrb_fixnum(gyro_direction_val);

	gyro = gyro_direction * rate;

	mrb_value left_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@left_motor"));
	motor_port_t left_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, left_motor, mrb_intern_lit(mrb, "@port")));
	motor_ang_l = ev3_motor_get_counts(left_motor_port);

	mrb_value right_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@right_motor"));
	motor_port_t right_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, right_motor, mrb_intern_lit(mrb, "@port")));
	motor_ang_r = ev3_motor_get_counts(right_motor_port);
	int r1 = motor_ang_r;
	int l1 = motor_ang_l;

	volt = ev3_battery_voltage_mV();

  const int BACKLASHHALF = 4;   // バックラッシュの半分[deg]

	int mRightPwm = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pwm_R")));
	int mLeftPwm = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pwm_L")));
  if(mRightPwm < 0) motor_ang_r += BACKLASHHALF;
  else if(mRightPwm > 0) motor_ang_r -= BACKLASHHALF;

  if(mLeftPwm < 0) motor_ang_l += BACKLASHHALF;
  else if(mLeftPwm > 0) motor_ang_l -= BACKLASHHALF;


	int r2 = motor_ang_r;
	int l2 = motor_ang_l;

	balance_control(
		(float)forward,
		(float)turn,
		(float)gyro,
		(float)offset,
		(float)motor_ang_l,
		(float)motor_ang_r,
		(float)volt,
		(signed char*)&pwm_L,
		(signed char*)&pwm_R);
	mrb_value ary = mrb_ary_new_capa(mrb, 6);
	mrb_ary_set(mrb, ary, 0, mrb_fixnum_value((mrb_int)pwm_L));
	mrb_ary_set(mrb, ary, 1, mrb_fixnum_value((mrb_int)pwm_R));
	mrb_ary_set(mrb, ary, 2, mrb_fixnum_value(rate));	// for log : gyro
	mrb_ary_set(mrb, ary, 3, mrb_fixnum_value(motor_ang_r));	// for log : right motor
	mrb_ary_set(mrb, ary, 4, mrb_fixnum_value(motor_ang_l));	// for log : left motor
	mrb_ary_set(mrb, ary, 5, mrb_fixnum_value(volt));	// for log : battery
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pwm_R"), mrb_fixnum_value((mrb_int)pwm_R));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pwm_L"), mrb_fixnum_value((mrb_int)pwm_L));

	int r3 = (int)pwm_R;
	int l3 = (int)pwm_L;
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "r0:%d, l0:%d r1:%d, l1:%d, r2:%d, l2:%d, r3:%d, l3:%d\r\n",mRightPwm,mLeftPwm, r1, l1, r2, l2, r3, l3);
	serial_wri_dat(SIO_PORT_BT, msg, sizeof(msg));
	return ary;
}

static mrb_value
mrb_mruby_balancer_set_right_motor(mrb_state *mrb, mrb_value self)
{

	mrb_value v;
	mrb_get_args(mrb, "o", &v);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@right_motor"), v);

	return self;
}

static mrb_value
mrb_mruby_balancer_set_left_motor(mrb_state *mrb, mrb_value self)
{

	mrb_value v;
	mrb_get_args(mrb, "o", &v);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@left_motor"), v);

	return self;
}

static mrb_value
mrb_mruby_balancer_set_gyro(mrb_state *mrb, mrb_value self)
{

	mrb_value v;
	mrb_get_args(mrb, "o", &v);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@gyro"), v);

	return self;
}

static mrb_value
mrb_mruby_balancer_set_gyro_direction(mrb_state *mrb, mrb_value self)
{

	mrb_int n1;
	mrb_get_args(mrb, "i", &n1);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@gyro_direction"), mrb_fixnum_value(n1));

	return self;
}

void
mrb_mruby_balancer_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * balancer_class = mrb_define_class_under(mrb, ev3rt_class, "Balancer", mrb->object_class);

	mrb_define_method(mrb, balancer_class, "reset", mrb_mruby_balancer_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "calculate", mrb_mruby_balancer_calculate, MRB_ARGS_REQ(7));
	mrb_define_method(mrb, balancer_class, "calculate_auto", mrb_mruby_balancer_calculate_auto, MRB_ARGS_REQ(3));
	mrb_define_method(mrb, balancer_class, "right_motor=", mrb_mruby_balancer_set_right_motor, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "left_motor=", mrb_mruby_balancer_set_left_motor, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "gyro=", mrb_mruby_balancer_set_gyro, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "gyro_direction=", mrb_mruby_balancer_set_gyro_direction, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F0=", mrb_mruby_set_K_F0, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F1=", mrb_mruby_set_K_F1, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F2=", mrb_mruby_set_K_F2, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F3=", mrb_mruby_set_K_F3, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F0", mrb_mruby_get_K_F0, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F1", mrb_mruby_get_K_F1, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F2", mrb_mruby_get_K_F2, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, balancer_class, "K_F3", mrb_mruby_get_K_F3, MRB_ARGS_REQ(1));
}
void
mrb_mruby_balancer_gem_final(mrb_state* mrb)
{
}
