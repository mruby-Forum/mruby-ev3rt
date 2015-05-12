#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/value.h"
#include "ev3rt.h"
#include "ev3api.h"
#include "platform_interface_layer.h"

static mrb_value
mrb_mruby_sensor_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(n));
	return self;
}

void
mrb_mruby_sensor_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * sensor_class = mrb_define_class_under(mrb, ev3rt_class, "Sensor", mrb->object_class);

	//methods
	mrb_define_method(mrb, sensor_class, "initialize", mrb_mruby_sensor_initialize, MRB_ARGS_REQ(1));
}

void
mrb_mruby_sensor_gem_final(mrb_state* mrb)
{
}
