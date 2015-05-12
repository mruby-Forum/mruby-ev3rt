#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"
#include "kernel.h"

static mrb_int
getTim()
{
	SYSTIM time;
	get_tim(&time);
	return time;
}

static mrb_value mrb_mruby_clock_reset(mrb_state *mrb, mrb_value self);

static mrb_value
mrb_mruby_clock_initialize(mrb_state *mrb, mrb_value self)
{
	return mrb_mruby_clock_reset(mrb, self);
}

static mrb_value
mrb_mruby_clock_wait(mrb_state *mrb, mrb_value self)
{
	mrb_int duration;
	mrb_get_args(mrb, "i", &duration);

	dly_tsk(duration);
	return self;
}

static mrb_value
mrb_mruby_clock_sleep(mrb_state *mrb, mrb_value self)
{
	mrb_int duration;
	mrb_get_args(mrb, "i", &duration);

	tslp_tsk(duration);
	return self;
}

static mrb_value
mrb_mruby_clock_reset(mrb_state *mrb, mrb_value self)
{
	mrb_int start_clock = getTim();
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@start_clock"), mrb_fixnum_value(start_clock));
	return mrb_true_value();
}

static mrb_value
mrb_mruby_clock_now(mrb_state *mrb, mrb_value self)
{
	mrb_value start_clock_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@start_clock"));
	mrb_int start_clock = mrb_fixnum(start_clock_val);
	mrb_int now = getTim() - start_clock;
	return mrb_fixnum_value(now);
}

void
mrb_mruby_clock_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * clock_class = mrb_define_class_under(mrb, ev3rt_class, "Clock", mrb->object_class);

	mrb_define_method(mrb, clock_class, "initialize", mrb_mruby_clock_initialize, MRB_ARGS_NONE());
	mrb_define_method(mrb, clock_class, "reset", mrb_mruby_clock_reset, MRB_ARGS_NONE());
	mrb_define_method(mrb, clock_class, "now", mrb_mruby_clock_now, MRB_ARGS_NONE());
	mrb_define_method(mrb, clock_class, "wait", mrb_mruby_clock_wait, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, clock_class, "sleep", mrb_mruby_clock_sleep, MRB_ARGS_REQ(1));
}

void
mrb_mruby_clock_gem_final(mrb_state* mrb)
{
}
