#include "stm32f10x.h" // Device header
#include "TIM1_PWM.h"

/**
 * 函    数：直流电机初始化
 * 参    数：无
 * 返 回 值：无
 */
void TIM1_Motor_Init(void) // 准确来说是PWM用的定时器一，这里直接标记
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 将PA4和PA5引脚初始化为推挽输出

	TIM1_PWM_Init(); // 初始化直流电机的底层PWM
}

/**
 * 函    数：直流电机设置速度
 * 参    数：Speed 要设置的速度，范围：-100~100
 * 返 回 值：无
 */
void TIM1_Motor_SetSpeed(int Motor_Left, int Motor_Right) // 满速100
{
	if (Motor_Left <= 0) // 如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	// PA4置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_13); // PA5置低电平，设置方向为正转
		PWM_SetCompare1(-Motor_Left);		// PWM设置为速度值
	}
	else // 否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12); // PA4置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_13);	// PA5置高电平，设置方向为反转
		PWM_SetCompare1(Motor_Left);		// PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}

	if (Motor_Right <= 0) // 如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	// PA4置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_15); // PA5置低电平，设置方向为正转
		PWM_SetCompare4(-Motor_Right);		// PWM设置为速度值
	}
	else // 否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14); // PA4置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	// PA5置高电平，设置方向为反转
		PWM_SetCompare4(Motor_Right);		// PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
}
