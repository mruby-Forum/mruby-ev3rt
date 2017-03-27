#include "mruby.h"
#include "mruby/string.h"
#include "mruby/value.h"
#include "ev3api.h"

#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6)
#define CALIB_FONT_HEIGHT (8)

sensor_port_t port;

static mrb_value
mrb_mruby_lcd_initialize(mrb_state *mrb, mrb_value self)
{
	return self;
}

static mrb_value
mrb_mruby_lcd_print(mrb_state *mrb, mrb_value self)
{
	mrb_value s;
	char *msg;
	mrb_get_args(mrb, "S", &s);
	msg = mrb_str_to_cstr(mrb, s);
	ev3_lcd_fill_rect(0, CALIB_FONT_HEIGHT*1, EV3_LCD_WIDTH, CALIB_FONT_HEIGHT*2, EV3_LCD_WHITE);
	ev3_lcd_draw_string(msg, 0, CALIB_FONT_HEIGHT*1);
	return mrb_true_value();
}

void
mrb_mruby_lcd_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * lcd_class = mrb_define_class_under(mrb, ev3rt_class, "LCD", mrb->object_class);

	mrb_define_method(mrb, lcd_class, "initialize", mrb_mruby_lcd_initialize, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, lcd_class, "print", mrb_mruby_lcd_print, MRB_ARGS_REQ(1));
}

void
mrb_mruby_lcd_gem_final(mrb_state* mrb)
{
}
