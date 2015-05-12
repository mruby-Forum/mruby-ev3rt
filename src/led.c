#include "mruby.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_led_initialize(mrb_state *mrb, mrb_value self)
{
	  return self;
}

static mrb_value
mrb_mruby_led_on(mrb_state *mrb, mrb_value self){
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	ledcolor_t color = n;
	ev3_led_set_color(color);
	return self;
}


static mrb_value
mrb_mruby_led_off(mrb_state *mrb, mrb_value self){
	ev3_led_set_color(LED_OFF);
	return self;
}

void
mrb_mruby_led_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * led_class = mrb_define_class_under(mrb, ev3rt_class, "LED", mrb->object_class);

	mrb_define_method(mrb, led_class, "initialize", mrb_mruby_led_initialize, MRB_ARGS_NONE());
	mrb_define_method(mrb, led_class, "on", mrb_mruby_led_on, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, led_class, "off", mrb_mruby_led_off, MRB_ARGS_NONE());
}

void
mrb_mruby_led_gem_final(mrb_state* mrb)
{
}
