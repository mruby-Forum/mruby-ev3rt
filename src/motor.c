#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_motor_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n1, n2;
	mrb_get_args(mrb, "ii", &n1, &n2);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(n1));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@type"), mrb_fixnum_value(n2));

	motor_port_t port = (motor_port_t)n1;
	motor_type_t type = (motor_type_t)n2;
	ev3_motor_config(port, type);
	return self;
}

static mrb_value
mrb_mruby_motor_getCount(mrb_state *mrb, mrb_value self)
{
	motor_port_t port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
	mrb_int val = ev3_motor_get_counts(port);
	return mrb_fixnum_value(val);
}

static mrb_value
mrb_mruby_motor_resetCount(mrb_state *mrb, mrb_value self)
{
	motor_port_t port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
	ev3_motor_reset_counts(port);
	return mrb_true_value();
}

static mrb_value
mrb_mruby_motor_setPWM(mrb_state *mrb, mrb_value self)
{
	mrb_int pwm;
	mrb_get_args(mrb, "i", &pwm);
	motor_port_t port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));

	if (pwm == 0) {
		ev3_motor_stop(port, true);
	} else {
		ev3_motor_set_power(port, pwm);
	}
	return self;
}

static mrb_value
mrb_mruby_motor_rotate(mrb_state *mrb, mrb_value self)
{
	mrb_int degrees, speed;
	mrb_bool blocking;
	mrb_get_args(mrb, "iib", &degrees, &speed, &blocking);
	motor_port_t port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));

	ER ret = ev3_motor_rotate(port, degrees, speed, blocking);
	return self;
}

static mrb_value
mrb_mruby_motor_stop(mrb_state *mrb, mrb_value self)
{
	motor_port_t port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
	ev3_motor_stop(port, false);
	return self;
}

void
mrb_mruby_motor_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * motor_class = mrb_define_class_under(mrb, ev3rt_class, "Motor", mrb->object_class);

	mrb_define_method(mrb, motor_class, "initialize", mrb_mruby_motor_initialize, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, motor_class, "count", mrb_mruby_motor_getCount, MRB_ARGS_NONE());
	mrb_define_method(mrb, motor_class, "reset", mrb_mruby_motor_resetCount, MRB_ARGS_NONE());
	mrb_define_method(mrb, motor_class, "pwm=", mrb_mruby_motor_setPWM, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, motor_class, "power=", mrb_mruby_motor_setPWM, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, motor_class, "rotate", mrb_mruby_motor_rotate, MRB_ARGS_REQ(3));
	mrb_define_method(mrb, motor_class, "stop", mrb_mruby_motor_stop, MRB_ARGS_NONE());
}

void
mrb_mruby_motor_gem_final(mrb_state* mrb)
{
}
