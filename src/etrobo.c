#include "mruby.h"
#include "ev3api.h"
#include "etroboc_ext.h"

void mrb_mruby_etrobo_gem_init(mrb_state* mrb);
void mrb_mruby_etrobo_gem_final(mrb_state* mrb);

static mrb_value
mrb_etrobo_notify_completed(mrb_state *mrb, mrb_value self)
{
    ETRoboc_notifyCompletedToSimulator();
    return self;
}

mrb_value
mrb_etrobo_get_course_info(mrb_state *mrb, mrb_value self)
{
    mrb_int info;
    char pos[2] = {0, 0};
    mrb_get_args(mrb, "i", &info);
    pos[0] = ETRoboc_getCourseInfo((enum ETROBOC_COURSE_INFO_ID)info);
    return mrb_str_new_cstr(mrb, pos);
}

void
mrb_mruby_etrobo_gem_init(mrb_state* mrb)
{
	struct RClass * etrobo = mrb_define_class(mrb, "ETRobo", mrb->object_class);

    // constants
    // enum ETROBOC_COURCE_INFO_ID
    mrb_int n = 0;
	mrb_define_const(mrb, etrobo, "CARD_NUMBER", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_NUMBER", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_START", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_BLACK1", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_BLACK2", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_RED1", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_RED2", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_YELLOW1", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_YELLOW2", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_BLUE1", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_BLUE2", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_GREEN1", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_GREEN2", mrb_fixnum_value(n++));
	mrb_define_const(mrb, etrobo, "BLOCK_POS_END", mrb_fixnum_value(n++));

  	// class methods
    mrb_define_class_method(mrb, etrobo, "notify_completed", mrb_etrobo_notify_completed, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, etrobo, "get_course_info", mrb_etrobo_get_course_info, MRB_ARGS_REQ(1));
}

void
mrb_mruby_etrobo_gem_final(mrb_state* mrb)
{
}
