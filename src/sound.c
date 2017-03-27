#include "mruby.h"
#include "ev3api.h"

static mrb_value
mrb_mruby_sound_initialize(mrb_state *mrb, mrb_value self)
{
	  return self;
}

static mrb_value
mrb_mruby_set_volume(mrb_state *mrb, mrb_value self){
	mrb_int volume;
	mrb_get_args(mrb, "i", &volume);
	ER ret = ev3_speaker_set_volume((uint8_t)volume);
	if(ret == E_OK){
		return mrb_true_value();
	}else{
		return mrb_false_value();
	}
}


static mrb_value
mrb_mruby_play_tone(mrb_state *mrb, mrb_value self){
	mrb_int frequency, duration;
	mrb_get_args(mrb, "ii", &frequency, &duration);
	ER ret = ev3_speaker_play_tone((uint16_t)frequency, duration);
	if(ret == E_OK){
		return mrb_true_value();
	}else{
		return mrb_false_value();
	}
}


static mrb_value
mrb_mruby_play_wav(mrb_state *mrb, mrb_value self){
// TODO
//	mrb_int frequency, duration;
//	mrb_get_args(mrb, "ii", &frequency, &duration);
//	ER ret = sound_play_tone((uint16_t)frequency, duration);
//	if(ret == E_OK){
		return mrb_true_value();
//	}else{
//		return mrb_false_value();
//	}
}

static mrb_value
mrb_mruby_sound_off(mrb_state *mrb, mrb_value self){
	ER ret = ev3_speaker_stop();
	if(ret == E_OK){
		return mrb_true_value();
	}else{
		return mrb_false_value();
	}
}

void
mrb_mruby_sound_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * sound_class = mrb_define_class_under(mrb, ev3rt_class, "Sound", mrb->object_class);

	mrb_define_method(mrb, sound_class, "initialize", mrb_mruby_sound_initialize, MRB_ARGS_NONE());
	mrb_define_method(mrb, sound_class, "volume", mrb_mruby_set_volume, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, sound_class, "play_tone", mrb_mruby_play_tone, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, sound_class, "play_wav", mrb_mruby_play_wav, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, sound_class, "off", mrb_mruby_sound_off, MRB_ARGS_NONE());
}

void
mrb_mruby_sound_gem_final(mrb_state* mrb)
{
}
