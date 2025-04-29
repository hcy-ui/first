#include "stm32f10x.h" // Device header
#include "LED.h"
#include "Buzzer.h"
#include "OLED.h"
#include "TIM1_Motor.h"
#include "TIM2_4_Encoder.h"
#include "TIM3_pid.h"
#include "Track.h"
#include "USART3_WabCam.h"
#include "math.h"
#include "RP.h"

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

int32_t Speed_Left, Speed_Right, Location_Left, Location_Right;
uint8_t OLED_Update_Flag = 0;
float out_left, out_right;
float error_pos;
float error, track_out;
// int8_t menu2;

// 左右内环速度环
PID_t Inner_Left = {
	.Kp = 4,
	.Ki = 0.4,
	.Kd = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.Speed = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,

	.ErrorInt = 0,
	.ErrorIntMax = 10000,
	.ErrorIntMin = -10000,
};

PID_t Inner_Right = {
	.Kp = 4,
	.Ki = 0.4,
	.Kd = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.Speed = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 10000,
	.ErrorIntMin = -10000,
};

// 外环位置环
PID_t OOuter = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

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

// 内环灰度环
PID_t Track_PID = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

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

	// Buzzer_Init();
	// Buzzer_OFF();

	OLED_Init();
	// OLED_ShowChar(0, 0, 'A', OLED_8X16);
	// OLED_ShowString(16, 0, "Hello world!", OLED_8X16);
	// OLED_ShowChar(0, 18, 'A', OLED_6X8);
	// OLED_ShowString(16, 18, "Hello world!", OLED_6X8);
	// OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
	// OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
	// OLED_Printf(32, 0, OLED_8X16, "%4d", 123);
	// OLED_Update();

	TIM1_Motor_Init();
	// TIM1_Motor_SetSpeed(100, 100);

	TIM2_Encoder_Init();
	TIM4_Encoder_Init();
	TIM3_PID_Init();

	RP_Init();

	// USART3_Serial_Init();

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
	// OLED_ShowChar(0, 18, 'A', OLED_6X8);
	// OLED_ShowString(16, 18, "Hello world!", OLED_6X8);
	// OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
	// OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
	// OLED_Printf(32, 0, OLED_8X16, "%4d", 123);
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

		if (OLED_Update_Flag) 
		{
			OLED_Printf(0, 0, OLED_8X16, "Kp:%4.2f", Inner_Left.Kp);
			OLED_Printf(0, 16, OLED_8X16, "Ki:%4.2f", Inner_Left.Ki);
			OLED_Printf(0, 32, OLED_8X16, "Kd:%4.2f", Inner_Left.Kd);
			OLED_Printf(0, 48, OLED_8X16, "Ta:%+04.0f", Inner_Left.Target);
	
			OLED_Printf(64, 0, OLED_8X16, "SL:%04d", Speed_Left);
			OLED_Printf(64, 16, OLED_8X16, "SR:%04d", Speed_Right);
			OLED_Printf(64, 32, OLED_8X16, "Out:%04.0f", (Inner_Left.Out + Inner_Right.Out) / 2);
			OLED_Printf(64, 48, OLED_8X16, "Act:%04.0f", (Inner_Left.Actual + Inner_Right.Actual) / 2);
	
			OLED_Update();
	
			OLED_Update_Flag = 0;  // 清除标志位
		}
		// Inner_Left.Kp = RP_GetValue(1) / 4095.0 * 5;//内环速度控制
		// Inner_Left.Ki = RP_GetValue(3) / 4095.0 * 2;
		// Inner_Left.Kd = RP_GetValue(4) / 4095.0 * 2;
		Inner_Left.Target = RP_GetValue(2) / 4095.0 * 310 - 155;

		// Inner_Right.Kp = Inner_Left.Kp;
		// Inner_Right.Ki = Inner_Left.Ki;
		// Inner_Right.Kd = Inner_Left.Kd;
		Inner_Right.Target = Inner_Left.Target;


		OOuter.Target = TIM3_PID_Locate(600);
		// OOuter.Target = RP_GetValue(1) / 4095.0 * 5;//外环位置控制
		// Inner_Left.Ki = RP_GetValue(3) / 4095.0 * 2;
		// Inner_Left.Kd = RP_GetValue(4) / 4095.0 * 2;
		// Inner_Left.Target = RP_GetValue(2) / 4095.0 * 310 - 155;



		// OLED_Printf(0, 38, OLED_8X16, "%4d", USART3_Serial_RxData);
		// OLED_Update();

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
	static uint16_t Count1, Count2, Count3, Count4;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		Count1++;
		if (Count1 >= 10) // 内环（速度+灰度）
		{
			Count1 = 0;
			OLED_Update_Flag=1;//OLED刷新标志位（每10ms刷新一下OLED显示）

			Speed_Right = -TIM2_Encoder_Get();
			Speed_Left = TIM4_Encoder_Get();
			Location_Left += Speed_Left;
			Location_Right += Speed_Right; // 获取速度and位置

			/***********测试速度用法
			// Inner.Actual = (float)(-Speed_Left + Speed_Right) / 2.0;
			// PID_Sim_Update(&Inner);
			// TIM1_Motor_SetSpeed(Inner.Out, Inner.Out);
			***************/

			// error = Track_Calculate_Error();
			// Track_PID.Actual = error;
			// PID_Sim_Update(&Track_PID); // 获取并计算误差

			track_out = TIM3_PID_Limit(Track_PID.Out, -40, 40); // 限幅，防止方向大转
			Inner_Left.Speed = Inner_Left.Target + track_out;
			Inner_Right.Speed = Inner_Right.Target - track_out; // 速度校准

			Inner_Left.Actual = Speed_Left; // 速度环（左）
			Inner_Right.Actual = Speed_Right; // 速度环（右）

			Inner_Left.Target = Inner_Left.Speed;//左
			PID_Sim_Update(&Inner_Left);
			out_left = Inner_Left.Out;

			Inner_Right.Target = Inner_Right.Speed;//右
			PID_Sim_Update(&Inner_Right);
			out_right = Inner_Right.Out;

			TIM1_Motor_SetSpeed(out_left, out_right);
		}

		// Count2++;
		// if(Count2 >= 10)
		// {
		// 	Count2 = 0;
			
		// }
		// }
		Count4++;
		if (Count4 >= 100)//外环（位置）
		{
			Count4 = 0;

			OOuter.Actual = (Location_Left + Location_Right) / 2;

			PID_Sim_Update(&OOuter);

			error_pos = OOuter.Target - OOuter.Actual;
			Update_Speed_By_Position(OOuter.Out, error_pos); // 减速停下

		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}

}