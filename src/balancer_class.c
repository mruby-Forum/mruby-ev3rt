#include "mruby.h"
#include "mruby/array.h"
#include "mruby/value.h"
#include "ev3api.h"
#include "balancer.h"

static mrb_value
mrb_mruby_balancer_initialize(mrb_state *mrb, mrb_value self)
{
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

void
mrb_mruby_balancer_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * balancer_class = mrb_define_class_under(mrb, ev3rt_class, "Balancer", mrb->object_class);

	mrb_define_class_method(mrb, balancer_class, "reset", mrb_mruby_balancer_initialize, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, balancer_class, "calculate", mrb_mruby_balancer_calculate, MRB_ARGS_REQ(7));
}

void
mrb_mruby_balancer_gem_final(mrb_state* mrb)
{
}
