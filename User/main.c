#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "DDelay.h"
#include "OLED.h"
#include "TIM1_Motor.h"
#include "Encoder.h"
#include "RP.h"
//#include "Timer3_Blooteeth.h"
#include "Key.h"
#include "menu.h"
// #include "MYRTC.h"
// #include "MPU6050.h"
// #include "MPU_EXTI.h"
// #include "inv_mpu.h"
#include "TIM1_PWM.h"

// int16_t Speed_Left, Speed_Right, Location;
int8_t menu2;
int main(void)
{
	OLED_Init();
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	Key_Init();
	// MyRTC_Init();
	// delay_init();
	// MPU6050_DMP_Init();
	// MPU_EXTI_Init();
	// Motor_Init();
	// PWM_Init();
	// TIM2_Encoder_Init();
	// TIM4_Encoder_Init();			
	// Serial_Init();
	// RP_Init();

	// OLED_ShowChar(0, 0, 'A', OLED_8X16);
	// OLED_ShowString(16, 0, "Hello world!", OLED_8X16);
	//	OLED_ShowChar(0,18,'A',OLED_6X8);
	//	OLED_ShowString(16,18,"Hello world!",OLED_6X8);
	//	OLED_ShowNum(0,28,12345,5,OLED_6X8);
	//	OLED_ShowSignedNum(40,28,-66,2,OLED_6X8);
	// OLED_Printf(32,0,OLED_8X16,"%4d",123);
	// OLED_Update();

	// 电位器测试代码
	//  OLED_Printf(0, 0, OLED_8X16, "RP1:%04d", RP_GetValue(1));
	//  OLED_Printf(0, 16, OLED_8X16, "RP2:%04d", RP_GetValue(2));
	//  OLED_Printf(0, 32, OLED_8X16, "RP3:%04d", RP_GetValue(3));
	//  OLED_Printf(0, 48, OLED_8X16, "RP4:%04d", RP_GetValue(4));

	// OLED_Update();

	// Motor_SetSpeed(10, -10);

	while (1)
	{
		menu2=Menu1();//二级菜单
		if(menu2==1)	{Menu2_Motor();}
		if(menu2==2)	{Menu2_PID();}
		if(menu2==3)	{}
		if(menu2==4)	{Menu2_MPU6050();}
		if(menu2==5)	{}
		if(menu2==6)	{Menu2_Clock();}
		if(menu2==7)	{}
		if(menu2==8)	{}

		// Speed_Left = TIM2_Encoder_Get();
		// Speed_Right = TIM4_Encoder_Get();
		// OLED_Printf(0, 16, OLED_8X16, "%2d", Speed_Left);
		// OLED_Printf(0, 32, OLED_8X16, "%2d", Speed_Right);
		// OLED_Update();
		// Serial_Printf("%d,%d\r\n", Speed_Left, Speed_Right);
	}
}
