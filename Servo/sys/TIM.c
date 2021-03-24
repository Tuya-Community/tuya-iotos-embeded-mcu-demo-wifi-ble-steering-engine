#include "TIM.h"
#include "IO.h"

uint16_t T_servo=0;
uint8_t T_swtich=0;

void TIM6_Init(uint16_t arr,uint16_t psc)//TASK����
{
	RCC->APBENR1 |= 1<<4;
	TIM6->ARR = arr;
	TIM6->PSC = psc;
	TIM6->DIER |= 1<<0;//ʹ�ܸ����ж�
	TIM6->CR1 |= 1<<0;//ʹ�ܼ�����
	NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 1); 
	NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
}
void TIM6_Start(void)
{
	TIM6->CR1 |=1<<0;//ʹ�ܼ�����
}
void TIM6_Stop(void)
{
	TIM6->CR1 &=~(1<<0);	//��ֹ������
	TIM6->CNT =0;//��������
}
uint8_t F_TASK_TIM6=0;
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
	if ((TIM6->SR & 1) == 1) /* Check ARR match */ 
	{	
		TIM6->SR &= ~(1<<0);//����жϱ�־λ
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



void ConfigureTIM16_CH1_AsPWM_EdgeAligned(void)//T16_CH1 PWM PD0 AF2 ���
{
  RCC->APBENR2 |= 1<<17;
  RCC->IOPENR |= 1<<3;
	GPIOD->MODER &=~(3<<0);
  GPIOD->MODER |=  2<<0; //���ù���
	GPIOD->AFR[0]&=0XFFFFFFF0;
  GPIOD->AFR[0] |= 0x02 << 0;
  
  TIM16->PSC = 159; //16M/(159+1)=0.1M, step=10us
  TIM16->ARR = 1999;//10*��1999+1��=20Ms ������
  TIM16->CCR1 = 1; // �ߵ�ƽʱ��
  TIM16->CCMR1 |=6<<4|1<<3;//CH_1 110ģʽ,Ԥװ��ʹ�ܣ�Ĭ��Ϊ���ģʽ��
  TIM16->CCER |= 1<<0;  //CH_1�������PWM
  TIM16->CR1 |= 1<<0; //ʹ�ܼ�����(���ض��� ����)
  TIM16->EGR |= 1<<0; //�����ж�
	TIM16->BDTR |=1<<14;//�Զ����ʹ��AOE
}
void TIM16_PWM_Stop(void)
{
	TIM16->CCER &=~(1<<0);//�ر����PWM
	TIM16->CR1 &=~(1<<0);	//��ֹ������
	TIM16->CNT =0;//��������
}
void TIM16_PWM_Start(void)
{
	TIM16->CCER |=1<<0;//�������PWM
	TIM16->CR1 |=1<<0;//ʹ�ܼ�����
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
  GPIOD->MODER |=  2<<2; //���ù���
	GPIOD->AFR[0]&=0XFFFFFF0F;
  GPIOD->AFR[0] |= 0x02 << 4;
  
  TIM17->PSC = 159; //16M/(159+1)=0.1M, step=10us
  TIM17->ARR = 1999;//10*��999+1��=10Ms ������
  TIM17->CCR1 = 1; // �ߵ�ƽʱ��
  TIM17->CCMR1 |=6<<4|1<<3;//CH_1 110ģʽ,Ԥװ��ʹ�ܣ�Ĭ��Ϊ���ģʽ��
  TIM17->CCER |= 1<<0;  //CH_1�������PWM
  TIM17->CR1 |= 1<<0; //ʹ�ܼ�����(���ض��� ����)
  TIM17->EGR |= 1<<0; //�����ж�
	TIM17->BDTR |=1<<14;//�Զ����ʹ��AOE
}
void TIM17_PWM_Stop(void)
{
	TIM17->CCER &=~(1<<0);//�ر����PWM
	TIM17->CR1 &=~(1<<0);	//��ֹ������
	TIM17->CNT =0;//��������
}
void TIM17_PWM_Start(void)
{
	TIM17->CCER |=1<<0;//�������PWM
	TIM17->CR1 |=1<<0;//ʹ�ܼ�����
}
void TIM17_PWM_Set(uint16_t CCR)
{
	TIM17->CCR1 = CCR;
}



void TIM_Init(void)
{	
	ConfigureTIM16_CH1_AsPWM_EdgeAligned();//��� 20ms����
	TIM16_PWM_Set(200);
	ConfigureTIM17_CH1_AsPWM_EdgeAligned();
	TIM17_PWM_Set(200);
	TIM6_Init(100,15999);//1000=1s
}




