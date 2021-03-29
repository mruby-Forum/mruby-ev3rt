#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_steering_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_value left_motor, right_motor;
	mrb_get_args(mrb, "oo", &left_motor, &right_motor);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@left_motor"), left_motor);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@right_motor"), right_motor);

	return self;
}

static mrb_value
mrb_mruby_steer(mrb_state *mrb, mrb_value self)
{
	mrb_int power, turn;
	mrb_get_args(mrb, "ii", &power, &turn);
	mrb_value left_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@left_motor"));
	mrb_value right_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@right_motor"));
	motor_port_t left_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, left_motor, mrb_intern_lit(mrb, "@port")));
	motor_port_t right_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, right_motor, mrb_intern_lit(mrb, "@port")));
	ev3_motor_steer(left_motor_port, right_motor_port, (int)power, (int)turn);

	return self;
}


static mrb_value
mrb_mruby_reset_motors(mrb_state *mrb, mrb_value self)
{
	mrb_value left_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@left_motor"));
	motor_port_t left_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, left_motor, mrb_intern_lit(mrb, "@port")));
	ev3_motor_reset_counts(left_motor_port);

	mrb_value right_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@right_motor"));
	motor_port_t right_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, right_motor, mrb_intern_lit(mrb, "@port")));
	ev3_motor_reset_counts(right_motor_port);

	return self;
}

static mrb_value
mrb_mruby_motors_stop(mrb_state *mrb, mrb_value self)
{
	mrb_value left_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@left_motor"));
	motor_port_t left_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, left_motor, mrb_intern_lit(mrb, "@port")));
	ev3_motor_stop(left_motor_port, false);

	mrb_value right_motor = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@right_motor"));
	motor_port_t right_motor_port = (motor_port_t)mrb_fixnum(mrb_iv_get(mrb, right_motor, mrb_intern_lit(mrb, "@port")));
	ev3_motor_stop(right_motor_port, false);

	return self;
}

void
mrb_mruby_steering_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * steering_class = mrb_define_class_under(mrb, ev3rt_class, "Steering", mrb->object_class);

	mrb_define_method(mrb, steering_class, "initialize", mrb_mruby_steering_initialize, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, steering_class, "steer", mrb_mruby_steer, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, steering_class, "reset_motors", mrb_mruby_reset_motors, MRB_ARGS_NONE());
	mrb_define_method(mrb, steering_class, "stop", mrb_mruby_motors_stop, MRB_ARGS_NONE());
}

void
mrb_mruby_steering_gem_final(mrb_state* mrb)
{
}
