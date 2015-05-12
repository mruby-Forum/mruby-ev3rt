#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_gyro_sensor_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(n));

	sensor_port_t port = n;
	ev3_sensor_config(port, GYRO_SENSOR);
	return self;
}

static mrb_value
mrb_mruby_gyro_sensor_getRate(mrb_state *mrb, mrb_value self)
{
	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);
	mrb_int rate = ev3_gyro_sensor_get_rate(port);
	return mrb_fixnum_value(rate);
}

static mrb_value
mrb_mruby_gyro_sensor_reset(mrb_state *mrb, mrb_value self)
{
	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);
	ER err = ev3_gyro_sensor_reset(port);
	return self;
}

void
mrb_mruby_gyro_sensor_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * sensor_class = mrb_class_get_under(mrb, ev3rt_class, "Sensor");
	struct RClass * gyro_sensor_class = mrb_define_class_under(mrb, ev3rt_class, "GyroSensor", sensor_class);

	mrb_define_method(mrb, gyro_sensor_class, "initialize", mrb_mruby_gyro_sensor_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, gyro_sensor_class, "rate", mrb_mruby_gyro_sensor_getRate, MRB_ARGS_NONE());
	mrb_define_method(mrb, gyro_sensor_class, "reset", mrb_mruby_gyro_sensor_reset, MRB_ARGS_NONE());
}

void
mrb_mruby_gyro_sensor_gem_final(mrb_state* mrb)
{
}
