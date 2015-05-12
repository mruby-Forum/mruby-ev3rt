#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_color_sensor_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(n));

	sensor_port_t port = (sensor_port_t)n;
	ev3_sensor_config(port, COLOR_SENSOR);
	return self;
}

static mrb_value
mrb_mruby_color_sensor_getBrightness(mrb_state *mrb, mrb_value self)
{
	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);
	mrb_int ret = ev3_color_sensor_get_reflect(port);
	return mrb_fixnum_value(ret);
}

void
mrb_mruby_color_sensor_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * sensor_class = mrb_class_get_under(mrb, ev3rt_class, "Sensor");
	struct RClass * color_sensor_class = mrb_define_class_under(mrb, ev3rt_class, "ColorSensor", sensor_class);

	mrb_define_method(mrb, color_sensor_class, "initialize", mrb_mruby_color_sensor_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, color_sensor_class, "brightness", mrb_mruby_color_sensor_getBrightness, MRB_ARGS_NONE());
}

void
mrb_mruby_color_sensor_gem_final(mrb_state* mrb)
{
}
