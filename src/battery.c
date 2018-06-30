#include "mruby.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_battery_initialize(mrb_state *mrb, mrb_value self)
{
	  return self;
}


static mrb_value
mrb_mruby_battery_getVoltage(mrb_state *mrb, mrb_value self)
{
	mrb_int val = ev3_battery_voltage_mV();
	return mrb_fixnum_value(val);
}


static mrb_value
mrb_mruby_battery_getCurrent(mrb_state *mrb, mrb_value self)
{
	mrb_int val = ev3_battery_current_mA();
	return mrb_fixnum_value(val);
}

void
mrb_mruby_battery_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * battery_class = mrb_define_class_under(mrb, ev3rt_class, "Battery", mrb->object_class);

	mrb_define_method(mrb, battery_class, "initialize", mrb_mruby_battery_initialize, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, battery_class, "batteryVoltage", mrb_mruby_battery_getVoltage, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, battery_class, "batteryCurrent", mrb_mruby_battery_getCurrent, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, battery_class, "battery_voltage", mrb_mruby_battery_getVoltage, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, battery_class, "battery_current", mrb_mruby_battery_getCurrent, MRB_ARGS_NONE());
}

void
mrb_mruby_battery_gem_final(mrb_state* mrb)
{
}
