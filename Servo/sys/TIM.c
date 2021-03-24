#include "TIM.h"
#include "IO.h"

uint16_t T_servo=0;
uint8_t T_swtich=0;

void TIM6_Init(uint16_t arr,uint16_t psc)//TASK分配
{
	RCC->APBENR1 |= 1<<4;
	TIM6->ARR = arr;
	TIM6->PSC = psc;
	TIM6->DIER |= 1<<0;//使能更新中断
	TIM6->CR1 |= 1<<0;//使能计数器
	NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 1); 
	NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
}
void TIM6_Start(void)
{
	TIM6->CR1 |=1<<0;//使能计数器
}
void TIM6_Stop(void)
{
	TIM6->CR1 &=~(1<<0);	//禁止计数器
	TIM6->CNT =0;//计数清零
}
uint8_t F_TASK_TIM6=0;
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
	if ((TIM6->SR & 1) == 1) /* Check ARR match */ 
	{	
		TIM6->SR &= ~(1<<0);//清除中断标志位
		F_TASK_TIM6=1;
	}
}
void TASK_TIM6(void)
{
	if(T_swtich)
	{
		if(TIM17->CCR1>T_servo)
		{
			TIM17->CCR1--;
		}
		else if(TIM17->CCR1<T_servo)
		{
			TIM17->CCR1++;
		}
		if(TIM16->CCR1>T_servo)
		{
			TIM16->CCR1--;
		}
		else if(TIM16->CCR1<T_servo)
		{
			TIM16->CCR1++;
		}	
	}
	else
	{
		TIM16->CCR1=0;
		TIM17->CCR1=0;
	}
}



void ConfigureTIM16_CH1_AsPWM_EdgeAligned(void)//T16_CH1 PWM PD0 AF2 舵机
{
  RCC->APBENR2 |= 1<<17;
  RCC->IOPENR |= 1<<3;
	GPIOD->MODER &=~(3<<0);
  GPIOD->MODER |=  2<<0; //复用功能
	GPIOD->AFR[0]&=0XFFFFFFF0;
  GPIOD->AFR[0] |= 0x02 << 0;
  
  TIM16->PSC = 159; //16M/(159+1)=0.1M, step=10us
  TIM16->ARR = 1999;//10*（1999+1）=20Ms 总周期
  TIM16->CCR1 = 1; // 高电平时间
  TIM16->CCMR1 |=6<<4|1<<3;//CH_1 110模式,预装载使能（默认为输出模式）
  TIM16->CCER |= 1<<0;  //CH_1开启输出PWM
  TIM16->CR1 |= 1<<0; //使能计数器(边沿对齐 递增)
  TIM16->EGR |= 1<<0; //更新中断
	TIM16->BDTR |=1<<14;//自动输出使能AOE
}
void TIM16_PWM_Stop(void)
{
	TIM16->CCER &=~(1<<0);//关闭输出PWM
	TIM16->CR1 &=~(1<<0);	//禁止计数器
	TIM16->CNT =0;//计数清零
}
void TIM16_PWM_Start(void)
{
	TIM16->CCER |=1<<0;//开启输出PWM
	TIM16->CR1 |=1<<0;//使能计数器
}
void TIM16_PWM_Set(uint16_t CCR)
{
	TIM16->CCR1 = CCR;
}
void ConfigureTIM17_CH1_AsPWM_EdgeAligned(void)//T17_CH1 PWM PD1 AF2
{
  RCC->APBENR2 |= 1<<18;
  RCC->IOPENR |= 1<<3;
	GPIOD->MODER &=~(3<<2);
  GPIOD->MODER |=  2<<2; //复用功能
	GPIOD->AFR[0]&=0XFFFFFF0F;
  GPIOD->AFR[0] |= 0x02 << 4;
  
  TIM17->PSC = 159; //16M/(159+1)=0.1M, step=10us
  TIM17->ARR = 1999;//10*（999+1）=10Ms 总周期
  TIM17->CCR1 = 1; // 高电平时间
  TIM17->CCMR1 |=6<<4|1<<3;//CH_1 110模式,预装载使能（默认为输出模式）
  TIM17->CCER |= 1<<0;  //CH_1开启输出PWM
  TIM17->CR1 |= 1<<0; //使能计数器(边沿对齐 递增)
  TIM17->EGR |= 1<<0; //更新中断
	TIM17->BDTR |=1<<14;//自动输出使能AOE
}
void TIM17_PWM_Stop(void)
{
	TIM17->CCER &=~(1<<0);//关闭输出PWM
	TIM17->CR1 &=~(1<<0);	//禁止计数器
	TIM17->CNT =0;//计数清零
}
void TIM17_PWM_Start(void)
{
	TIM17->CCER |=1<<0;//开启输出PWM
	TIM17->CR1 |=1<<0;//使能计数器
}
void TIM17_PWM_Set(uint16_t CCR)
{
	TIM17->CCR1 = CCR;
}



void TIM_Init(void)
{	
	ConfigureTIM16_CH1_AsPWM_EdgeAligned();//舵机 20ms周期
	TIM16_PWM_Set(200);
	ConfigureTIM17_CH1_AsPWM_EdgeAligned();
	TIM17_PWM_Set(200);
	TIM6_Init(100,15999);//1000=1s
}




