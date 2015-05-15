#ifndef _ETROBO_BALANCER_H_INCLUDED
#define _ETROBO_BALANCER_H_INCLUDED

extern void balance_init(void);

extern void balance_control(float args_cmd_forward, float args_cmd_turn,
  float args_gyro, float args_gyro_offset, float args_theta_m_l,
  float args_theta_m_r, float args_battery, signed char *ret_pwm_l, signed char
  *ret_pwm_r);


#endif
