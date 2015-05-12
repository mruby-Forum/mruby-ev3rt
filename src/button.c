#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/value.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_button_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@num"), mrb_fixnum_value(n));
	return self;
}

static mrb_value
mrb_mruby_button_ispressed(mrb_state *mrb, mrb_value self)
{
	button_t button = (button_t)mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@num")));
	bool_t pressed = ev3_button_is_pressed(button);
	if(pressed){
	    return mrb_true_value();
	}else{
		return mrb_false_value();
	}
}


static mrb_value
mrb_mruby_button_clicked(mrb_state *mrb, mrb_value self)
{
	//TODO
	// ER ev3_button_set_on_clicked(button_t button, ISR handler, intptr_t exinf)
	return self;
}

void
mrb_mruby_button_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * button_class = mrb_define_class_under(mrb, ev3rt_class, "Button", mrb->object_class);

	mrb_define_method(mrb, button_class, "initialize", mrb_mruby_button_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, button_class, "pressed?", mrb_mruby_button_ispressed, MRB_ARGS_NONE());
	mrb_define_method(mrb, button_class, "clicked", mrb_mruby_button_clicked, MRB_ARGS_REQ(1));
}

void
mrb_mruby_button_gem_final(mrb_state* mrb)
{
}
