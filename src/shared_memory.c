#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "ev3api.h"
#include "kernel.h"

#define SHARED_MEMORY_MAX 10

mrb_int shared_memory_array[SHARED_MEMORY_MAX]= {0};

static mrb_value
mrb_mruby_shared_memory_read(mrb_state *mrb, mrb_value self)
{
	mrb_int index;
	mrb_get_args(mrb, "i", &index);
	if(index < 0 || index >= SHARED_MEMORY_MAX){
		mrb_raise(mrb, E_INDEX_ERROR, "index is out of shared memory");
	}
	return mrb_fixnum_value(shared_memory_array[index]);
}

static mrb_value
mrb_mruby_shared_memory_write(mrb_state *mrb, mrb_value self)
{
	mrb_int index, value;
	mrb_get_args(mrb, "ii", &index, &value);
	if(index < 0 || index >= SHARED_MEMORY_MAX){
		mrb_raise(mrb, E_INDEX_ERROR, "index is out of shared memory");
	}
	shared_memory_array[index] = value;

	return mrb_fixnum_value(value);
}

void
mrb_mruby_shared_memory_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_class_get(mrb, "EV3RT");
	struct RClass * mem_class = mrb_define_module_under(mrb, ev3rt_class, "SharedMemory");

	mrb_define_module_function(mrb, mem_class, "[]", mrb_mruby_shared_memory_read, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, mem_class, "[]=", mrb_mruby_shared_memory_write, MRB_ARGS_REQ(2));
}

void
mrb_mruby_shared_memory_gem_final(mrb_state* mrb)
{
}
