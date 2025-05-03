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
float Left_speed, Right_speed; // 内环最终速度输出值
uint8_t OLED_Update_Flag = 0;
float out_left, out_right;
float error_Left, error_Right;
float Error, track_out;
// int8_t menu2;

// 灰度环（内）
PID_t Track_PID = {
	.Kp = 4.5,
	.Ki = 0.01,
	.Kd = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 1000,
	.ErrorIntMin = -1000,
};

// 左右环速度环（中）
PID_t Speed_Control_Left = {
	.Kp = 2,
	.Ki = 0.1,
	.Kd = 0,

	.Target = 30,
	.Actual = 0,
	.Out = 0,

	.Speed = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,

	.ErrorInt = 0,
	.ErrorIntMax = 1000,
	.ErrorIntMin = -1000,
};

PID_t Speed_Control_Right = {
	.Kp = 2,
	.Ki = 0.1,
	.Kd = 0,

	.Target = 30,
	.Actual = 0,
	.Out = 0,

	.Speed = 0,

	.OutMax = 100,
	.OutMin = -100,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 1000,
	.ErrorIntMin = -1000,
};

// 左右环位置环（外）

PID_t Position_Control_Left = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.OutMax = 50,
	.OutMin = -50,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 1000,
	.ErrorIntMin = -1000,
};

PID_t Position_Control_Right = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,

	.Target = 0,
	.Actual = 0,
	.Out = 0,

	.OutMax = 50,
	.OutMin = -50,

	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
	.ErrorIntMax = 1000,
	.ErrorIntMin = -1000,
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

	TIM2_Encoder_Init();
	TIM4_Encoder_Init();
	TIM3_PID_Init();

	RP_Init();

	Track_Init();

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
		Track_Read();

		if (OLED_Update_Flag)
		{
			/*********内环灰度环******************/
			// OLED_Printf(0, 0, OLED_8X16, "Kp:%4.2f", Track_PID.Kp);
			// OLED_Printf(0, 16, OLED_8X16, "Ki:%4.2f", Track_PID.Ki);
			// OLED_Printf(0, 32, OLED_8X16, "Kd:%4.2f", Track_PID.Kd);
			// OLED_Printf(0, 48, OLED_8X16, "%4.2f", track_out);
			// // OLED_Printf(0, 48, OLED_8X16, "Er:%4.2f", Error);

			// OLED_Printf(64, 0, OLED_8X16, "SL:%04d", -Speed_Left);
			// OLED_Printf(64, 16, OLED_8X16, "SLT:%04.0f", Speed_Control_Left.Target);
			// OLED_Printf(64, 32, OLED_8X16, "SR:%04d", -Speed_Right);
			// OLED_Printf(64, 48, OLED_8X16, "SRT:%04.0f", Speed_Control_Right.Target);
			/*************内环灰度环**************/

			/*************灰度值读取测试**************/
			// OLED_Printf(0, 0, OLED_8X16, "Kp:%d", Gray1);
			// OLED_Printf(0, 16, OLED_8X16, "Ki:%d", Gray2);
			// OLED_Printf(0, 32, OLED_8X16, "Kd:%d", Gray3);
			// OLED_Printf(0, 48, OLED_8X16, "Ta:%d", Gray4);

			// OLED_Printf(64, 0, OLED_8X16, "SL:%d", Gray5);
			// OLED_Printf(64, 16, OLED_8X16, "SLT:%d", Gray6);
			// OLED_Printf(64, 32, OLED_8X16, "SR:%d", Gray7);
			// OLED_Printf(64, 48, OLED_8X16, "SRT:%d", Gray8);
			/*************灰度值读取测试**************/

			/*********中环速度环******************/
			// OLED_Printf(0, 0, OLED_8X16, "Kp:%4.2f", Speed_Control_Left.Kp);
			// OLED_Printf(0, 16, OLED_8X16, "Ki:%4.2f", Speed_Control_Left.Ki);
			// OLED_Printf(0, 32, OLED_8X16, "Kd:%4.2f", Speed_Control_Left.Kd);
			// OLED_Printf(0, 48, OLED_8X16, "Ta:%+04.0f", Speed_Control_Left.Target);

			// OLED_Printf(64, 0, OLED_8X16, "SL:%04d", Speed_Left);
			// OLED_Printf(64, 16, OLED_8X16, "SR:%04d", Speed_Right);
			// OLED_Printf(64, 32, OLED_8X16, "Out:%04.0f", (Speed_Control_Left.Out + Speed_Control_Right.Out) / 2);
			// OLED_Printf(64, 48, OLED_8X16, "Act:%04.0f", (Speed_Control_Left.Actual + Speed_Control_Right.Actual) / 2);
			/*************中环速度环**************/

			/*********外环位置环******************/
			OLED_Printf(0, 0, OLED_8X16, "Kp:%4.2f", Position_Control_Left.Kp);
			OLED_Printf(0, 16, OLED_8X16, "Ki:%4.2f", Position_Control_Left.Ki);
			OLED_Printf(0, 32, OLED_8X16, "Kd:%4.2f", Position_Control_Left.Kd);
			OLED_Printf(0, 48, OLED_8X16, "Ta:%+4.0f", Position_Control_Left.Target);

			OLED_Printf(64, 0, OLED_8X16, "LL:%4.2f", Location_Left);
			OLED_Printf(64, 16, OLED_8X16, "LLA:%4.2f", Position_Control_Left.Actual);
			OLED_Printf(64, 32, OLED_8X16, "LR:%4.2f", Location_Right);
			OLED_Printf(64, 48, OLED_8X16, "LRA:%4.2f", Position_Control_Right.Actual);
			/*********外环位置环******************/

			OLED_Update();

			OLED_Update_Flag = 0; // 清除标志位
		}
		/*********内环灰度环******************/
		// Track_PID.Kp = RP_GetValue(1) / 4095.0 * 10; // 内环循迹控制
		// Track_PID.Ki = RP_GetValue(3) / 4095.0 * 2;
		// Track_PID.Kd = RP_GetValue(4) / 4095.0 * 2;
		// Track_PID.Target = RP_GetValue(2) / 4095.0 * 310 - 155;
		/*********内环灰度环******************/

		/*********中环速度环******************/
		// Speed_Control_Left.Kp = RP_GetValue(1) / 4095.0 * 5;
		// Speed_Control_Left.Ki = RP_GetValue(3) / 4095.0 * 2;
		// Speed_Control_Left.Kd = RP_GetValue(4) / 4095.0 * 2;
		// Speed_Control_Left.Target = RP_GetValue(2) / 4095.0 * 310 - 155;

		// Speed_Control_Right.Kp = Speed_Control_Left.Kp;
		// Speed_Control_Right.Ki = Speed_Control_Left.Ki;
		// Speed_Control_Right.Kd = Speed_Control_Left.Kd;
		// Speed_Control_Right.Target = Speed_Control_Left.Target;
		/*********中环速度环******************/

		/*********外环位置环******************/
		// Position_Control_Right.Target = TIM3_PID_Locate(600);
		Position_Control_Left.Kp = RP_GetValue(1) / 4095.0 * 2; // 外环位置控制
		Position_Control_Left.Ki = RP_GetValue(3) / 4095.0 * 2;
		Position_Control_Left.Kd = RP_GetValue(4) / 4095.0 * 2;
		// Position_Control_Right.Target = RP_GetValue(4) / 4095.0 * 2080 - 1040;//测试左右位置环
		Position_Control_Left.Target = RP_GetValue(2) / 4095.0 * 2080 - 1040;

		Position_Control_Right.Kp = Position_Control_Left.Kp;
		Position_Control_Right.Ki = Position_Control_Left.Ki;
		Position_Control_Right.Kd = Position_Control_Left.Kd;
		Position_Control_Right.Target = Position_Control_Left.Target;
		/*********外环位置环******************/

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

		// Serial_Printf("%d,%d\r\n", Speed_Left, Speed_Right);
	}
}

void TIM3_IRQHandler(void)
{
	static uint16_t Count1, Count2, Count3;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		// /*********灰度循迹******************/
		// Count1++;
		// if (Count1 >= 10) // 内环（速度）
		// {
		// 	Count1 = 0;
		// 	OLED_Update_Flag = 1; // OLED刷新标志位（每10ms刷新一下OLED显示）

		// 	Error = Track_Calculate_Error();
		// 	Track_PID.Actual = Error;
		// 	PID_Sim_Update(&Track_PID); // 获取并计算误差

		// 	track_out = TIM3_PID_Limit(Track_PID.Out, -7, 7); // 限幅，防止方向大转
		// 	// 利用灰度误差控制目标速度，使小车转向修正
		// 	// Speed_Control_Left.Target = 30 - track_out;		  // 左轮速度 = 基准速度 + 偏差修正
		// 	// Speed_Control_Right.Target = 30 + track_out;	  // 右轮速度 = 基准速度 - 偏差修正
		// 	/*********灰度循迹******************/

		// 	Left_speed = out_left - track_out;	 // 左轮速度 = 内环输出 + 偏差修正
		// 	Right_speed = out_right + track_out; // 右轮速度 = 内环输出 - 偏差修正

		// 	TIM1_Motor_SetSpeed(-Left_speed, -Right_speed);
		// }

		/*********速度PID******************/
		Count2++;
		if (Count2 >= 10) // 中环（速度）
		{
			Count2 = 0;

			Speed_Left = TIM4_Encoder_Get();
			Speed_Right = -TIM2_Encoder_Get();
			Location_Left += Speed_Left;
			Location_Right += Speed_Right; // 获取速度and位置

			Speed_Control_Left.Actual = -Speed_Left;   // 速度环（左）
			Speed_Control_Right.Actual = -Speed_Right; // 速度环（右）

			PID_Sim_Update(&Speed_Control_Left);
			out_left = Speed_Control_Left.Out;

			PID_Sim_Update(&Speed_Control_Right);
			out_right = Speed_Control_Right.Out;

			TIM1_Motor_SetSpeed(-out_left, -out_right); // 在测试位置环时解除注释
		}

		/*********位置PID******************/
		Count3++;
		if (Count3 >= 100) // 外环（位置环）
		{
			Count3 = 0;

			Position_Control_Left.Actual = Location_Left;
			Position_Control_Right.Actual = Location_Right;

			PID_Sim_Update(&Position_Control_Left);
			PID_Sim_Update(&Position_Control_Right);

			Speed_Control_Left.Target = Position_Control_Left.Out;
			Speed_Control_Right.Target = Position_Control_Right.Out;

			// error_Left = Position_Control_Left.Target - Position_Control_Left.Actual;
			// error_Right = Position_Control_Right.Target - Position_Control_Right.Actual;

			// Update_Speed_By_Position(Position_Control_Left.Out, error_Left,0); // 减速停下
			// Update_Speed_By_Position(Position_Control_Right.Out, error_Right,1);
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
