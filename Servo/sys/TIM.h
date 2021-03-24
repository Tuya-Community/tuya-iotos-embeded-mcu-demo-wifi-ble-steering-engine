#ifndef __TIM_H
#define __TIM_H 		
#include "MY_ST_config.h"

extern uint16_t T_servo;
extern uint8_t T_swtich;
extern uint8_t F_TASK_TIM6;

void TASK_TIM6(void);
void TIM16_PWM_Set(uint16_t CCR);
void TIM17_PWM_Set(uint16_t CCR);
void TIM_Init(void);

#endif

