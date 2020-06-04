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


static mrb_value
mrb_mruby_task_initialize(mrb_state *mrb, mrb_value self)
{
	return self;
}


static mrb_value
mrb_mruby_task_sleep(mrb_state *mrb, mrb_value self)
{
	mrb_int timeout;
	mrb_int ret = mrb_get_args(mrb, "|i", &timeout);
	if(ret == 0){
		slp_tsk();
	}else{
		tslp_tsk(timeout * 1000);
	}
	return self;
}


static mrb_value
mrb_mruby_task_wake_up(mrb_state *mrb, mrb_value self)
{
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	wup_tsk(task_id);

	//for debug
//	char msg[256];
//	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
//	mrb_value val2 = mrb_fixnum_value(task_id);
//	sprintf(msg, "wakeup taskID=%d", mrb_fixnum(val2));
//	ev3_lcd_draw_string(msg, 0, 8);

	return self;
}


static mrb_value
mrb_mruby_task_active(mrb_state *mrb, mrb_value self)
{
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	act_tsk(task_id);

	// for debug
//	char msg[256];
//	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
//	mrb_value val2 = mrb_fixnum_value(task_id);
//	sprintf(msg, "active taskID=%d", mrb_fixnum(val2));
//	ev3_lcd_draw_string(msg, 0, 8);

	return self;
}


static mrb_value
mrb_mruby_task_delay(mrb_state *mrb, mrb_value self)
{
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	dly_tsk(task_id);

	// for debug
//	char msg[256];
//	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
//	mrb_value val2 = mrb_fixnum_value(task_id);
//	sprintf(msg, "active taskID=%d", mrb_fixnum(val2));
//	ev3_lcd_draw_string(msg, 0, 8);

	return self;
}

static mrb_value
mrb_mruby_task_start_cyclic(mrb_state *mrb, mrb_value self)
{
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	sta_cyc(task_id);

	// for debug
//	char msg[256];
//	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
//	mrb_value val2 = mrb_fixnum_value(task_id);
//	sprintf(msg, "start cyclic taskID=%d", mrb_fixnum(val2));
//	ev3_lcd_draw_string(msg, 0, 8);

	return self;
}


static mrb_value
mrb_mruby_task_stop_cyclic(mrb_state *mrb, mrb_value self)
{
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	sta_cyc(task_id);

	// for debug
//	char msg[256];
//	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
//	mrb_value val2 = mrb_fixnum_value(task_id);
//	sprintf(msg, "stop cyclic taskID=%d", mrb_fixnum(val2));
//	ev3_lcd_draw_string(msg, 0, 8);

	return self;
}

static mrb_value
mrb_mruby_task_exit(mrb_state *mrb, mrb_value self)
{

	ext_tsk();
	return self;
}

void
mrb_mruby_task_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * task_class = mrb_define_class_under(mrb, ev3rt_class, "Task", mrb->object_class);

	mrb_define_method(mrb, task_class, "initialize", mrb_mruby_task_initialize, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, task_class, "sleep", mrb_mruby_task_sleep, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "delay", mrb_mruby_task_delay, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "wakeup", mrb_mruby_task_wake_up, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "active", mrb_mruby_task_active, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "exit", mrb_mruby_task_exit, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, task_class, "start_cyclic", mrb_mruby_task_start_cyclic, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "stop_cyclic", mrb_mruby_task_stop_cyclic, MRB_ARGS_REQ(1));
}

void
mrb_mruby_task_gem_final(mrb_state* mrb)
{
}
