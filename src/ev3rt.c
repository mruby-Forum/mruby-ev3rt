#include "mruby.h"
#include "ev3api.h"


void mrb_mruby_balancer_gem_init(mrb_state *mrb);
void mrb_mruby_button_gem_init(mrb_state *mrb);
void mrb_mruby_clock_gem_init(mrb_state *mrb);
void mrb_mruby_lcd_gem_init(mrb_state *mrb);
void mrb_mruby_led_gem_init(mrb_state *mrb);
void mrb_mruby_motor_gem_init(mrb_state *mrb);
void mrb_mruby_sensor_gem_init(mrb_state *mrb);
void mrb_mruby_color_sensor_gem_init(mrb_state *mrb);
void mrb_mruby_gyro_sensor_gem_init(mrb_state *mrb);
void mrb_mruby_sonar_sensor_gem_init(mrb_state *mrb);
void mrb_mruby_touch_sensor_gem_init(mrb_state *mrb);
void mrb_mruby_battery_gem_init(mrb_state *mrb);
void mrb_mruby_sound_gem_init(mrb_state *mrb);
void mrb_mruby_serial_gem_init(mrb_state *mrb);
void mrb_mruby_task_gem_init(mrb_state *mrb);
void mrb_mruby_random_gem_init(mrb_state *mrb);
void mrb_mruby_shared_memory_gem_init(mrb_state *mrb);
void mrb_mruby_steering_gem_init(mrb_state *mrb);

static mrb_value
mrb_mruby_ev3rt_initialize(mrb_state *mrb, mrb_value self)
{
	  return self;
}

void
mrb_mruby_ev3rt_gem_init(mrb_state* mrb)
{
	struct RClass * ev3rt_class = mrb_define_class(mrb, "EV3RT", mrb->object_class);

	mrb_define_const(mrb,ev3rt_class, "PORT_1", mrb_fixnum_value(0));
	mrb_define_const(mrb,ev3rt_class, "PORT_2", mrb_fixnum_value(1));
	mrb_define_const(mrb,ev3rt_class, "PORT_3", mrb_fixnum_value(2));
	mrb_define_const(mrb,ev3rt_class, "PORT_4", mrb_fixnum_value(3));
	mrb_define_const(mrb,ev3rt_class, "TNUM_SENSOR_PORT", mrb_fixnum_value(4));

	//sensor_type_t
	mrb_define_const(mrb,ev3rt_class, "NONE_SENSOR", mrb_fixnum_value(0));
	mrb_define_const(mrb,ev3rt_class, "ULTRASONIC_SENSOR", mrb_fixnum_value(1));
	mrb_define_const(mrb,ev3rt_class, "GYRO_SENSOR", mrb_fixnum_value(2));
	mrb_define_const(mrb,ev3rt_class, "TOUCH_SENSOR", mrb_fixnum_value(3));
	mrb_define_const(mrb,ev3rt_class, "COLOR_SENSOR", mrb_fixnum_value(4));
	mrb_define_const(mrb,ev3rt_class, "TNUM_SENSOR_TYPE", mrb_fixnum_value(5));

	//motor_port_t
	mrb_define_const(mrb, ev3rt_class, "PORT_A", mrb_fixnum_value(0));
	mrb_define_const(mrb, ev3rt_class, "PORT_B", mrb_fixnum_value(1));
	mrb_define_const(mrb, ev3rt_class, "PORT_C", mrb_fixnum_value(2));
	mrb_define_const(mrb, ev3rt_class, "PORT_D", mrb_fixnum_value(3));
	mrb_define_const(mrb, ev3rt_class, "TNUM_MOTOR_PORT", mrb_fixnum_value(4));

	//motor_type_t
	mrb_define_const(mrb, ev3rt_class, "NONE_MOTOR", mrb_fixnum_value(0));
	mrb_define_const(mrb, ev3rt_class, "MEDIUM_MOTOR", mrb_fixnum_value(1));
	mrb_define_const(mrb, ev3rt_class, "LARGE_MOTOR", mrb_fixnum_value(2));
	mrb_define_const(mrb, ev3rt_class, "UNREGULATED_MOTOR", mrb_fixnum_value(3));
	mrb_define_const(mrb, ev3rt_class, "TNUM_MOTOR_TYPE", mrb_fixnum_value(4));

        //ledcolor_t
	mrb_define_const(mrb, ev3rt_class, "LED_OFF", mrb_fixnum_value(0));
	mrb_define_const(mrb, ev3rt_class, "LED_RED", mrb_fixnum_value(1));
	mrb_define_const(mrb, ev3rt_class, "LED_GREEN", mrb_fixnum_value(2));
	mrb_define_const(mrb, ev3rt_class, "LED_ORANGE", mrb_fixnum_value(3));

        //button_t
	mrb_define_const(mrb, ev3rt_class, "LEFT_BUTTON", mrb_fixnum_value(0));
	mrb_define_const(mrb, ev3rt_class, "RIGHT_BUTTON", mrb_fixnum_value(1));
	mrb_define_const(mrb, ev3rt_class, "UP_BUTTON", mrb_fixnum_value(2));
	mrb_define_const(mrb, ev3rt_class, "DOWN_BUTTON", mrb_fixnum_value(3));
	mrb_define_const(mrb, ev3rt_class, "ENTER_BUTTON", mrb_fixnum_value(4));
	mrb_define_const(mrb, ev3rt_class, "BACK_BUTTON", mrb_fixnum_value(5));
	mrb_define_const(mrb, ev3rt_class, "TNUM_BUTTON", mrb_fixnum_value(6));

	//serial port
	mrb_define_const(mrb, ev3rt_class, "SIO_PORT_UART", mrb_fixnum_value(1));
	mrb_define_const(mrb, ev3rt_class, "SIO_PORT_BT", mrb_fixnum_value(2));
	mrb_define_const(mrb, ev3rt_class, "SIO_PORT_LCD", mrb_fixnum_value(3));


  	//methods
	mrb_define_method(mrb, ev3rt_class, "initialize", mrb_mruby_ev3rt_initialize, MRB_ARGS_REQ(1));

        // initialize under classes
        mrb_mruby_balancer_gem_init(mrb);
        mrb_mruby_button_gem_init(mrb);
        mrb_mruby_clock_gem_init(mrb);
        mrb_mruby_lcd_gem_init(mrb);
        mrb_mruby_led_gem_init(mrb);
        mrb_mruby_battery_gem_init(mrb);
        mrb_mruby_sound_gem_init(mrb);
        mrb_mruby_task_gem_init(mrb);
        mrb_mruby_random_gem_init(mrb);
        mrb_mruby_shared_memory_gem_init(mrb);

        // motor
        mrb_mruby_motor_gem_init(mrb);
        mrb_mruby_steering_gem_init(mrb);

        // sensor
        mrb_mruby_sensor_gem_init(mrb);
        mrb_mruby_color_sensor_gem_init(mrb);
        mrb_mruby_gyro_sensor_gem_init(mrb);
        mrb_mruby_sonar_sensor_gem_init(mrb);
        mrb_mruby_touch_sensor_gem_init(mrb);

        // log
        mrb_mruby_serial_gem_init(mrb);
}

void
mrb_mruby_ev3rt_gem_final(mrb_state* mrb)
{
}
