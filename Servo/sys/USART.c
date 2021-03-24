#include "USART.h"
#include "TIM.h"

uint8_t USART_Send(USART_TypeDef * MY_usart,uint8_t *data,uint16_t len)
{
	uint8_t ret=1;
	uint16_t timeout=0x8000;
	while(len>0)
	{
		timeout=0x8000;
		MY_usart->TDR = *data;
		while((MY_usart->ISR&1<<6)!=1<<6)//等待发送完毕
		{
			timeout--;
			if( 0 == timeout )
			{
				ret = 1;
				break;
			}
		}
		data++;
		len--;
	}
	if( 0 != timeout )
	{
		ret = 0;
	}
	return ret;
}


void Configure_USART_MAIN(uint32_t bound) //TX PC4, RX PC5 USART1
{
	RCC->APBRSTR2 &=~(1<<14);//恢复串口1
	RCC->IOPENR |= 1<<2;//使能GPIOC时钟
	GPIOC->MODER &=~(3<<8|3<<10);
	GPIOC->MODER |=2<<8|2<<10;//复用模式
	GPIOC->AFR[0] &=~(0xf<<16|0xf<<20);
	GPIOC->AFR[0] |=1<<16|1<<20;//选择复用功能AF1
	RCC->APBENR2 |=1<<14;//使能串口1时钟	
	USART_MAIN->BRR = 16000000 / bound; 
	USART_MAIN->CR1 |= 1<<0|1<<2|1<<3|1<<5;//串口使能，使能接收，使能发送,
	while((USART_MAIN->ISR & 1<<6) != 1<<6)//发送完成标志位
	{ 
		break;/* add time out here for a robust application */
	}	
	NVIC_SetPriority(USART1_IRQn, 1);
	NVIC_EnableIRQ(USART1_IRQn);
}
void USART1_IRQHandler(void)
{
	if((USART_MAIN->ISR & 1<<5) == 1<<5)//接收寄存器数据不为空
	{
		uart_receive_input((unsigned char)(USART_MAIN->RDR));
	}
	else
	{
		//NVIC_DisableIRQ(USART1_IRQn); /* Disable USART1_IRQn */
	}	
	if((USART_MAIN->ISR & (1<<3)) == (1<<3))//ORE
	{
		USART_MAIN->ICR =1<<3;
	}
}




