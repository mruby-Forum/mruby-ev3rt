#include "mruby.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"
#include "serial.h"	// for serial debug

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

static mrb_value
mrb_mruby_color_sensor_getRGB(mrb_state *mrb, mrb_value self)
{
	rgb_raw_t rgb;

	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);

	ev3_color_sensor_get_rgb_raw(port, &rgb);

	mrb_value ary = mrb_ary_new_capa(mrb, 3);
	mrb_ary_set(mrb, ary, 0, mrb_fixnum_value((mrb_int)rgb.r));
	mrb_ary_set(mrb, ary, 1, mrb_fixnum_value((mrb_int)rgb.g));
	mrb_ary_set(mrb, ary, 2, mrb_fixnum_value((mrb_int)rgb.b));

	int r = mrb_fixnum(RARRAY_PTR(ary)[0]);
	int g = mrb_fixnum(RARRAY_PTR(ary)[1]);
	int b = mrb_fixnum(RARRAY_PTR(ary)[2]);

	return ary;
}

static mrb_value
mrb_mruby_color_sensor_getRGBPart(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_int iret;
	rgb_raw_t rgb;

	mrb_get_args(mrb, "i", &n);
	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);

	ev3_color_sensor_get_rgb_raw(port, &rgb);
	switch(n){
		case 0:
				iret = (mrb_int)rgb.r;
			break;
		case 1:
				iret = (mrb_int)rgb.g;
			break;
		case 2:
				iret = (mrb_int)rgb.b;
			break;
	}

	return mrb_fixnum_value(iret);
}

static mrb_value
mrb_mruby_color_sensor_getColor(mrb_state *mrb, mrb_value self)
{
	mrb_value port_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port"));
	sensor_port_t port = mrb_fixnum(port_val);
	int color = ev3_color_sensor_get_color(port);

	return mrb_fixnum_value(color);
}

void
mrb_mruby_color_sensor_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * sensor_class = mrb_class_get_under(mrb, ev3rt_class, "Sensor");
	struct RClass * color_sensor_class = mrb_define_class_under(mrb, ev3rt_class, "ColorSensor", sensor_class);


	//RGB
	mrb_define_const(mrb, color_sensor_class, "R", mrb_fixnum_value(0));
	mrb_define_const(mrb, color_sensor_class, "G", mrb_fixnum_value(1));
	mrb_define_const(mrb, color_sensor_class, "B", mrb_fixnum_value(2));

	//Color Sensor's color
	mrb_define_const(mrb, color_sensor_class, "COLOR_NONE", mrb_fixnum_value(0));	//無色
	mrb_define_const(mrb, color_sensor_class, "BLACK", mrb_fixnum_value(1));
	mrb_define_const(mrb, color_sensor_class, "BLUE", mrb_fixnum_value(2));
	mrb_define_const(mrb, color_sensor_class, "GREEN", mrb_fixnum_value(3));
	mrb_define_const(mrb, color_sensor_class, "YELLOW", mrb_fixnum_value(4));
	mrb_define_const(mrb, color_sensor_class, "RED", mrb_fixnum_value(5));
	mrb_define_const(mrb, color_sensor_class, "WHITE", mrb_fixnum_value(6));
	mrb_define_const(mrb, color_sensor_class, "BROWN", mrb_fixnum_value(7));


	mrb_define_method(mrb, color_sensor_class, "initialize", mrb_mruby_color_sensor_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, color_sensor_class, "brightness", mrb_mruby_color_sensor_getBrightness, MRB_ARGS_NONE());
	mrb_define_method(mrb, color_sensor_class, "rgb", mrb_mruby_color_sensor_getRGB, MRB_ARGS_NONE());
	mrb_define_method(mrb, color_sensor_class, "rgb_part", mrb_mruby_color_sensor_getRGBPart, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, color_sensor_class, "color", mrb_mruby_color_sensor_getColor, MRB_ARGS_NONE());
}

void
mrb_mruby_color_sensor_gem_final(mrb_state* mrb)
{
}
