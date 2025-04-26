#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "TIM1_Motor.h"
#include "TIM2_4_Encoder.h"
#include "TIM3_pid.h"
#include "Track.h"
#include "LED.h"
#include "Buzzer.h"
#include "USART3_WabCam.h"
#include "math.h"

// #include "Delay.h"
// #include "DDelay.h"
// #include "RP.h"
// #include "Timer3_Blooteeth.h"
// #include "Key.h"
// #include "menu.h"
// #include "MYRTC.h"
// #include "MPU6050.h"
// #include "MPU_EXTI.h"
// #include "inv_mpu.h"
// #include "Infrared.h"

int16_t Speed_Left, Speed_Right, Location_Left, Location_Right;
int16_t out_left, out_right;
int16_t error_pos;
float error,track_out;
// int8_t menu2;

PID_t IInner = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

	.Speed_Left = 0,
	.Speed_Right = 0,

	.Target = 30,
	.Actual = 0,
	.Out = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 20000,
	.ErrorIntMin = -20000,
};

PID_t OOuter = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

	.Speed_Left = 0,
	.Speed_Right = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 20000,
	.ErrorIntMin = -20000,
};

PID_t Track_PID = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

	.Speed_Left = 0,
	.Speed_Right = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 20000,
	.ErrorIntMin = -20000,
};

int main(void)
{
	// OLED_Init();
	// TIM1_Motor_Init();
	// TIM2_Encoder_Init();
	// TIM4_Encoder_Init();
	// TIM3_PID_Init();
	// Track_Init();
	LED_Init();
	LED1_ON();
	LED2_ON();

	Buzzer_Init();
	Buzzer_OFF();

	OLED_Init();
	OLED_ShowChar(0, 0, 'A', OLED_8X16);
	OLED_ShowString(16, 0, "Hello world!", OLED_8X16);
	OLED_ShowChar(0, 18, 'A', OLED_6X8);
	OLED_ShowString(16, 18, "Hello world!", OLED_6X8);
	OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
	OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
	OLED_Printf(32, 0, OLED_8X16, "%4d", 123);
	OLED_Update();

	USART3_Serial_Init();

	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	// Key_Init();
	// Infrared_Init();
	// MyRTC_Init();
	// delay_init();
	// MPU6050_DMP_Init();
	// MPU_EXTI_Init();
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
		OLED_Printf(0, 38, OLED_8X16, "%4d", USART3_Serial_RxData);
		OLED_Update();

		// menu2=Menu1();//二级菜单
		// if(menu2==1)	{Menu2_Motor();}
		// if(menu2==2)	{Menu2_PID();}
		// if(menu2==3)	{}
		// if(menu2==4)	{Menu2_MPU6050();}
		// if(menu2==5)	{}
		// if(menu2==6)	{Menu2_Clock();}
		// if(menu2==7)	{}
		// if(menu2==8)	{}

		// Speed_Left = TIM2_Encoder_Get();
		// Speed_Right = TIM4_Encoder_Get();
		// OLED_Printf(0, 16, OLED_8X16, "%2d", Speed_Left);
		// OLED_Printf(0, 32, OLED_8X16, "%2d", Speed_Right);
		// OLED_Update();
		// Serial_Printf("%d,%d\r\n", Speed_Left, Speed_Right);
	}
}

void TIM3_IRQHandler(void)
{
	static uint16_t Count1, Count2;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		Count1++;
		if (Count1 >= 10)
		{
			Count1 = 0;

			Speed_Left = TIM2_Encoder_Get();
			Speed_Right = TIM4_Encoder_Get();
			Location_Left += Speed_Left;
			Location_Right += Speed_Right; // 获取速度and位置

			error = Track_Calculate_Error();
			Track_PID.Actual = error;
			PID_Sim_Update(&Track_PID); // 获取并计算误差

			track_out = Limit(Track_PID.Out, -20, 20);
			IInner.Speed_Left = IInner.Target + track_out;
			IInner.Speed_Right = IInner.Target - track_out;// 速度校准

			IInner.Actual = Speed_Left; // 速度环（左）
			IInner.Target = IInner.Speed_Left;
			PID_Sim_Update(&IInner);
			out_left = IInner.Out;

			IInner.Actual = Speed_Right; // 速度环（右）
			IInner.Target = IInner.Speed_Right;
			PID_Sim_Update(&IInner);
			out_right = IInner.Out;

			TIM1_Motor_SetSpeed(out_left, -out_right);
		}

		Count2++;
		if (Count2 >= 100)
		{
			Count2 = 0;

			OOuter.Actual = (Location_Left + Location_Right) / 2;

			PID_Sim_Update(&OOuter);

			int16_t error_pos = OOuter.Target - OOuter.Actual;

			if (abs(error_pos) < 10) // 位置误差小于阈值
			{
				IInner.Target = 0; // 速度目标设为0，停车
			}
			else
			{
				IInner.Target = Limit(OOuter.Out, -50, 50);
			}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
