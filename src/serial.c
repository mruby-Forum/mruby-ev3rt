#include "mruby.h"
#include "mruby/string.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"
#include "serial.h"

static mrb_value
mrb_mruby_serial_initialize(mrb_state *mrb, mrb_value self)
{
#ifndef SIM
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@serialPort"), mrb_fixnum_value(n));
#endif
	return self;
}

static mrb_value
mrb_mruby_serial_write(mrb_state *mrb, mrb_value self)
{
	const char *msg;
#ifdef SIM	// for Simulator
	mrb_get_args(mrb, "z", &msg);
	syslog(LOG_NOTICE, msg);
#else		// for EV3
	size_t len;
	mrb_get_args(mrb, "s", &msg, &len);

	signed int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@serialPort")));
	ER err = serial_wri_dat(port, msg, (uint_t)len);
	if(err < 0){
		return mrb_false_value();
	}
#endif
	return mrb_true_value();
}

static mrb_value
mrb_mruby_serial_read_byte(mrb_state *mrb, mrb_value self)
{
	uint8_t buf;

	signed int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@serialPort")));
	ER_UINT erlen = serial_rea_dat(port, (char*)&buf, sizeof(buf));
	if(erlen > 0){
		return mrb_fixnum_value(buf);
	}else{
		return mrb_nil_value();
	}

}

void
mrb_mruby_serial_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * serial_class = mrb_define_class_under(mrb, ev3rt_class, "Serial", mrb->object_class);

	mrb_define_method(mrb, serial_class, "initialize", mrb_mruby_serial_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, serial_class, "write", mrb_mruby_serial_write, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, serial_class, "read_byte", mrb_mruby_serial_read_byte, MRB_ARGS_NONE());
}

void
mrb_mruby_serial_gem_final(mrb_state* mrb)
{
}
