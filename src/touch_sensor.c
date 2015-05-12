#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"

//センサークラスのextendsとして作成する
static mrb_value
mrb_mruby_touch_sensor_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(n));

	sensor_port_t port = n;
	ev3_sensor_config(port, TOUCH_SENSOR);
	return self;
}

static mrb_value
mrb_touch_sensor_ispressed(mrb_state *mrb, mrb_value self)
{
	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);
	bool_t pressed = ev3_touch_sensor_is_pressed(port);
	return mrb_bool_value(pressed);
}

void
mrb_mruby_touch_sensor_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt = mrb_class_get(mrb, "EV3RT");
	struct RClass * sensor = mrb_class_get_under(mrb, ev3rt, "Sensor");
	struct RClass * touch_sensor = mrb_define_class_under(mrb, ev3rt, "TouchSensor", sensor);

	mrb_define_method(mrb, touch_sensor, "initialize", mrb_mruby_touch_sensor_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, touch_sensor, "pressed?", mrb_touch_sensor_ispressed, MRB_ARGS_REQ(0));
}

void
mrb_mruby_touch_sensor_gem_final(mrb_state* mrb)
{
}
