#include "mruby.h"
#include "mruby/array.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"
#include "balancer.h"

static mrb_value
mrb_mruby_balancer_initialize(mrb_state *mrb, mrb_value self)
{
	// default gyro direction is normal
	mrb_int n1 = 1;
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@gyro_direction"), mrb_fixnum_value(n1));

	balance_init();
	return self;
}

static mrb_value
mrb_mruby_balancer_calculate(mrb_state *mrb, mrb_value self)
{

	mrb_int forward, turn, gyro, offset, motor_ang_l, motor_ang_r, volt;
	signed char pwm_L, pwm_R;
	mrb_get_args(mrb, "iiiiiii", &forward, &turn, &gyro, &offset, &motor_ang_l, &motor_ang_r, &volt);

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

	return ary;
}


static mrb_value
mrb_mruby_balancer_calculate_auto(mrb_state *mrb, mrb_value self)
{

	mrb_int forward, turn, gyro, offset, motor_ang_l, motor_ang_r, volt, gyro_direction;
	signed char pwm_L, pwm_R;
	mrb_get_args(mrb, "iii", &forward, &turn, &offset);

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

	volt = ev3_battery_voltage_mV();

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
}

void
mrb_mruby_balancer_gem_final(mrb_state* mrb)
{
}
