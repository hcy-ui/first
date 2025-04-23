#include "stm32f10x.h"

void Infrared_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/// @brief 检测到障碍，则返回1，否则返回0
/// @param  无
/// @return 0/1
uint8_t Infrared_Get(void)
{
	uint8_t Temp;
	Temp=!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
	return Temp;
}

