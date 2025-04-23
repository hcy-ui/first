#include "stm32f10x.h"  // Device header
#include "OLED.h"       // Device header
#include "MYRTC.h"      // Device header
#include "Key.h"        // Device header
#include "TIM1_Motor.h" // Device header
#include "stdlib.h"     // Device header
#include "TIM2_PID.h"        // Device header

uint8_t KeyNum;
uint8_t Menu1_Direction = 2;
uint8_t Flag = 1;
extern float Pitch, Roll, Yaw;
int Mo_Length, Mo_Angle, Mo_Speed, Mo_Direction;
float PID_Length;

PID_t Inner = {
    .Kp = 0,
    .Ki = 0,
    .Kd = 0,
    .OutMin = -100,
    .OutMax = 100,
    .ErrorIntMax = 20000,
    .ErrorIntMin = -20000,
    .Speed_Left = 0,
    .Speed_Right = 0,
    .Target = 0,
    .Actual = 0,
    .Error0 = 0,
    .Error1 = 0,
    .ErrorInt = 0,
};

PID_t Outer = {
    .Kp = 0,
    .Ki = 0,
    .Kd = 0,
    .OutMin = -100,
    .OutMax = 100,
    .ErrorIntMax = 20000,
    .ErrorIntMin = -20000,
    .Speed_Left = 0,
    .Speed_Right = 0,
    .Target = 0,
    .Actual = 0,
    .Error0 = 0,
    .Error1 = 0,
    .ErrorInt = 0,
};

void TIM2_IRQHandler(void)
{
	static uint16_t Count1, Count2;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Count1 ++;
		if (Count1 >= 40)
		{
			Count1 = 0;
			
			Speed = Encoder_Get();
			Location += Speed;
			
			Inner.Actual = Speed;
			
			PID_Update(&Inner);
			
			Motor_SetPWM(Inner.Out);
		}
		
		Count2 ++;
		if (Count2 >= 40)
		{
			Count2 = 0;
			
			Outer.Actual = Location;
			
			PID_Update(&Outer);
			
			Inner.Target = Outer.Out;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


int Menu1()
{
    // OLED_Printf(0, 0, OLED_8X16, "电机参数             ");
    // OLED_Printf(0, 16, OLED_8X16, "PID                  ");
    // OLED_Printf(0, 32, OLED_8X16, "遥感数据            ");
    // OLED_Printf(0, 48, OLED_8X16, "MPU6050             ");

    // OLED_Update();

    while (1)
    {
        // 按键控制加减
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Menu1_Direction = 1;
            Flag--;
            if (Flag == 0)
            {
                Flag = 8;
            }
            OLED_AnimUpdate();
        }
        if (KeyNum == 2)
        {
            Menu1_Direction = 2;
            Flag++;
            if (Flag == 9)
            {
                Flag = 1;
            }
            OLED_AnimUpdate();
        }
        if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            OLED_AnimUpdate();
            return Flag; // 将Flag返回给主函数
        }
        // 显示菜单
        switch (Flag)
        {
        case 1:
        {

            OLED_Printf(0, 0, OLED_8X16, "电机参数             ");
            OLED_Printf(0, 16, OLED_8X16, "PID                  ");
            OLED_Printf(0, 32, OLED_8X16, "遥感数据            ");
            OLED_Printf(0, 48, OLED_8X16, "MPU6050             ");
            if (Menu1_Direction == 1) // 向上
            {
                OLED_Animation(0, 16, 16, 16, 0, 0, 64, 16); // 2->1
            }
            else if (Menu1_Direction == 2) // 向下
            {
                OLED_Animation(0, 0, 0, 0, 0, 0, 64, 16); // 0->1
            }

            break;
        }

        case 2:
        {

            OLED_Printf(0, 0, OLED_8X16, "电机参数             ");
            OLED_Printf(0, 16, OLED_8X16, "PID                  ");
            OLED_Printf(0, 32, OLED_8X16, "遥感数据            ");
            OLED_Printf(0, 48, OLED_8X16, "MPU6050             ");
            OLED_ReverseArea(0, 16, 24, 16);
            OLED_Update();
            break;
        }

        case 3:
        {
            OLED_Printf(0, 0, OLED_8X16, "电机参数             ");
            OLED_Printf(0, 16, OLED_8X16, "PID                  ");
            OLED_Printf(0, 32, OLED_8X16, "遥感数据            ");
            OLED_Printf(0, 48, OLED_8X16, "MPU6050             ");
            OLED_ReverseArea(0, 32, 64, 16);
            OLED_Update();
            break;
        }

        case 4:
        {
            OLED_Printf(0, 0, OLED_8X16, "电机参数             ");
            OLED_Printf(0, 16, OLED_8X16, "PID                  ");
            OLED_Printf(0, 32, OLED_8X16, "遥感数据            ");
            OLED_Printf(0, 48, OLED_8X16, "MPU6050             ");
            OLED_ReverseArea(0, 48, 56, 16);
            OLED_Update();
            break;
        }

        case 5:
        {
            OLED_Printf(0, 0, OLED_8X16, "PID                 ");
            OLED_Printf(0, 16, OLED_8X16, "时钟               ");
            OLED_Printf(0, 32, OLED_8X16, "音乐               ");
            OLED_Printf(0, 48, OLED_8X16, "设置               ");
            OLED_ReverseArea(0, 0, 24, 16);
            OLED_Update();
            break;
        }

        case 6:
        {
            OLED_Printf(0, 0, OLED_8X16, "PID                 ");
            OLED_Printf(0, 16, OLED_8X16, "时钟               ");
            OLED_Printf(0, 32, OLED_8X16, "音乐               ");
            OLED_Printf(0, 48, OLED_8X16, "设置               ");
            OLED_ReverseArea(0, 16, 32, 16);
            OLED_Update();
            break;
        }

        case 7:
        {
            OLED_Printf(0, 0, OLED_8X16, "PID                 ");
            OLED_Printf(0, 16, OLED_8X16, "时钟               ");
            OLED_Printf(0, 32, OLED_8X16, "音乐               ");
            OLED_Printf(0, 48, OLED_8X16, "设置               ");
            OLED_ReverseArea(0, 32, 32, 16);
            OLED_Update();
            break;
        }

        case 8:
        {
            OLED_Printf(0, 0, OLED_8X16, "PID                 ");
            OLED_Printf(0, 16, OLED_8X16, "时钟               ");
            OLED_Printf(0, 32, OLED_8X16, "音乐               ");
            OLED_Printf(0, 48, OLED_8X16, "设置               ");
            OLED_ReverseArea(0, 48, 32, 16);
            OLED_Update();
            break;
        }
        }
    }
}

/// @brief 电机三级菜单，设置步长
/// @param
/// @return
int Menu3_Motor_SetLength(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Mo_Length--;
            if (Mo_Length == 0)
            {
                Mo_Length = 50;
            }
        }
        if (KeyNum == 2)
        {
            Mo_Length++;
            if (Mo_Length == 51)
            {
                Mo_Length = 1;
            }
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "<-                        ");
        OLED_Printf(0, 16, OLED_8X16, "单次步长:%d               ", Mo_Length);
        OLED_Printf(0, 32, OLED_8X16, "速度控制:%d               ", Mo_Speed);
        // 步长位数改变翻转区域
        if (Mo_Length < 10)
        {
            OLED_ReverseArea(72, 16, 8, 16);
        }
        else
        {
            OLED_ReverseArea(72, 16, 16, 16);
        }

        OLED_Update();
    }
}

/// @brief 电机三级菜单，设置速度
/// @param  无
/// @return
int Menu3_Motor_SetSpeed(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Mo_Speed -= Mo_Length;
            if (Mo_Speed <= -100)
            {
                Mo_Speed = -100;
            }
            Motor_SetSpeed(Mo_Speed, -Mo_Speed);
        }
        if (KeyNum == 2)
        {
            Mo_Speed += Mo_Length;
            if (Mo_Speed >= 100)
            {
                Mo_Speed = 100;
            }
            Motor_SetSpeed(Mo_Speed, -Mo_Speed);
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "<-                        ");
        OLED_Printf(0, 16, OLED_8X16, "单次步长:%d               ", Mo_Length);
        OLED_Printf(0, 32, OLED_8X16, "速度控制:%d               ", Mo_Speed);
        // 速度位数改变翻转区域
        if (abs(Mo_Speed) < 10)
        {
            OLED_ReverseArea(72, 32, 8, 16);
        }
        else if (abs(Mo_Speed) < 100)
        {
            OLED_ReverseArea(72, 32, 16, 16);
        }
        else
        {
            OLED_ReverseArea(72, 32, 24, 16);
        }
        OLED_Update();
    }
}

/// @brief 二级菜单，电机控制
/// @param  无
/// @return
int Menu2_Motor(void)
{
    uint8_t MoFlag = 1;
    int MoMoFlag = 0; // 三级菜单标志
    OLED_Printf(0, 0, OLED_8X16, "<-                        ");
    OLED_Printf(0, 16, OLED_8X16, "单次步长:%d               ", Mo_Length);
    OLED_Printf(0, 32, OLED_8X16, "速度控制:%d               ", Mo_Speed);

    OLED_Update();

    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            MoFlag--;
            if (MoFlag == 0)
            {
                MoFlag = 3;
            }
            OLED_AnimUpdate();
        }
        if (KeyNum == 2)
        {
            MoFlag++;
            if (MoFlag == 4)
            {
                MoFlag = 1;
            }
            OLED_AnimUpdate();
        }
        if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            OLED_AnimUpdate();
            MoMoFlag = MoFlag; // 核心
        }

        if (MoMoFlag == 1)
        {
            return 0;
        }
        if (MoMoFlag == 2)
        {
            MoMoFlag = Menu3_Motor_SetLength();
        }
        if (MoMoFlag == 3)
        {
            MoMoFlag = Menu3_Motor_SetSpeed();
        }

        switch (MoFlag)
        {
        case 1:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%d               ", Mo_Length);
            OLED_Printf(0, 32, OLED_8X16, "速度控制:%d               ", Mo_Speed);
            OLED_ReverseArea(0, 0, 16, 16);
            OLED_Update();
            break;
        }

        case 2:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%d               ", Mo_Length);
            OLED_Printf(0, 32, OLED_8X16, "速度控制:%d               ", Mo_Speed);
            OLED_ReverseArea(0, 16, 64, 16);
            OLED_Update();
            break;
        }

        case 3:
        {
            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%d               ", Mo_Length);
            OLED_Printf(0, 32, OLED_8X16, "速度控制:%d               ", Mo_Speed);
            OLED_ReverseArea(0, 32, 64, 16);
            OLED_Update();
            break;
        }
        }
    }
}

/// @brief 三级菜单，PID控制，设置步长
/// @param  无
/// @return 无
int Menu3_PID_SetLength(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            PID_Length-=0.1;
            if (PID_Length == 0)
            {
                PID_Length = 10;
            }
        }
        if (KeyNum == 2)
        {
            PID_Length+=0.1;
            if (PID_Length == 11)
            {
                PID_Length = 0;
            }
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "<-                        ");
        OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ", PID_Length);
        OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
        OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
        // 步长位数改变翻转区域
        if (PID_Length < 10)
        {
            OLED_ReverseArea(72, 16, 32, 16);
        }
        else
        {
            OLED_ReverseArea(72, 16, 40, 16);
        }

        OLED_Update();
    }
}

/// @brief 三级菜单，PID控制，设置Kp
/// @param  
/// @return 
int Menu3_PID_SetKp(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Inner.Kp-=PID_Length;
            if (Inner.Kp == 0)
            {
                Inner.Kp = 100;
            }
        }
        if (KeyNum == 2)
        {
            Inner.Kp+=PID_Length;
            if (Inner.Kp == 101)
            {
                Inner.Kp = 0;
            }
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "<-                        ");
        OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ",PID_Length);
        OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
        OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
        // 步长位数改变翻转区域
        if (Inner.Kp < 10)
        {
            OLED_ReverseArea(24, 32, 40, 16);
        }
        else
        {
            OLED_ReverseArea(24, 32, 40, 16);
        }

        OLED_Update();
    }
}

/// @brief 三级菜单，PID控制，设置Ki
/// @param  无
/// @return 无
int Menu3_PID_SetKi(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Inner.Ki-=PID_Length;
            if (Inner.Ki == 0)
            {
                Inner.Ki = 100;
            }
        }
        if (KeyNum == 2)
        {
            Inner.Ki+=PID_Length;
            if (Inner.Ki == 101)
            {
                Inner.Ki = 0;
            }
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "<-                        ");
        OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ",PID_Length);
        OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
        OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
        // 步长位数改变翻转区域
        if (Inner.Ki < 10)
        {
            OLED_ReverseArea(24, 48, 40, 16);
        }
        else
        {
            OLED_ReverseArea(24, 48, 40, 16);
        }

        OLED_Update();
    }
}

/// @brief 三级菜单，PID控制，设置Kd
/// @param  无
/// @return 无
int Menu3_PID_SetKd(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Inner.Kd-=PID_Length;
            if (Inner.Kd == 0)
            {
                Inner.Kd = 100;
            }
        }
        if (KeyNum == 2)
        {
            Inner.Kd+=PID_Length;
            if (Inner.Kd == 101)
            {
                Inner.Kd = 0;
            }
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "单次步长:%.2f               ", PID_Length);
        OLED_Printf(0, 16, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
        OLED_Printf(0, 32, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
        OLED_Printf(0, 48, OLED_8X16, "Kd:%.2f               ", Inner.Kd);
        // 步长位数改变翻转区域
        if (Inner.Kd < 10)
        {
            OLED_ReverseArea(24, 48, 40, 16);
        }
        else
        {
            OLED_ReverseArea(24, 48, 40, 16);
        }

        OLED_Update();
    }
}

int Menu3_PID_SetActual(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Inner.Actual-=PID_Length;
            if (Inner.Actual == 0)
            {
                Inner.Actual = 100;
            }
        }
        if (KeyNum == 2)
        {
            Inner.Actual+=PID_Length;
            if (Inner.Actual == 101)
            {
                Inner.Actual = 0;
            }
        }
        if (KeyNum == 3)
        {
            return 0;
        }
        OLED_Printf(0, 0, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
        OLED_Printf(0, 16, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
        OLED_Printf(0, 32, OLED_8X16, "Kd:%.2f               ", Inner.Kd);
        OLED_Printf(0, 48, OLED_8X16, "Actual:%.2f               ", Inner.Actual);
        // 步长位数改变翻转区域
        if (Inner.Actual < 10)
        {
            OLED_ReverseArea(56, 48, 40, 16);
        }
        else
        {
            OLED_ReverseArea(56, 48, 40, 16);
        }

        OLED_Update();
    }
}

/// @brief 二级菜单，PID控制
/// @param  无
/// @return
int Menu2_PID(void)
{
    uint8_t PIDFlag = 1;
    int PPIDFlag = 0; // 三级菜单标志
    OLED_Printf(0, 0, OLED_8X16, "<-                        ");
    OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ", PID_Length);
    OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
    OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);

    OLED_Update();

    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            PIDFlag--;
            if (PIDFlag == 0)
            {
                PIDFlag = 6;
            }
            OLED_AnimUpdate();
        }
        if (KeyNum == 2)
        {
            PIDFlag++;
            if (PIDFlag == 7)
            {
                PIDFlag = 1;
            }
            OLED_AnimUpdate();
        }
        if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            OLED_AnimUpdate();
            PPIDFlag = PIDFlag; // 核心
        }

        if (PPIDFlag == 1)
        {
            return 0;
        }
        if (PPIDFlag == 2)
        {
            PPIDFlag = Menu3_PID_SetLength();
        }
        if (PPIDFlag == 3)
        {
            PPIDFlag = Menu3_PID_SetKp();
        }
        if (PPIDFlag == 4)
        {
            PPIDFlag = Menu3_PID_SetKi();
        }
        if (PPIDFlag == 5)
        {
            PPIDFlag = Menu3_PID_SetKd();
        }
        if(PPIDFlag == 6)
        {
            PPIDFlag = Menu3_PID_SetActual();
        }

        switch (PIDFlag)
        {
        case 1:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ", PID_Length);
            OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
            OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);

            OLED_ReverseArea(0, 0, 16, 16);
            OLED_Update();
            break;
        }

        case 2:
        {
            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ", PID_Length);
            OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
            OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);

            OLED_ReverseArea(0, 16, 64, 16);
            OLED_Update();
            break;
        }

        case 3:
        {
            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ", PID_Length);
            OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
            OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
            OLED_ReverseArea(0, 32, 16, 16);

            OLED_Update();
            break;
        }

        case 4:
        {
            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "单次步长:%.2f               ", PID_Length);
            OLED_Printf(0, 32, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
            OLED_Printf(0, 48, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
            OLED_ReverseArea(0, 48, 16, 16);

            OLED_Update();
            break;
        }

        case 5:
        {
            OLED_Printf(0, 0, OLED_8X16, "单次步长:%.2f               ", PID_Length);
            OLED_Printf(0, 16, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
            OLED_Printf(0, 32, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
            OLED_Printf(0, 48, OLED_8X16, "Kd:%.2f               ", Inner.Kd);
            OLED_ReverseArea(0, 48, 16, 16);

            OLED_Update();
            break;
        }

        case 6:
        {
            OLED_Printf(0, 0, OLED_8X16, "Kp:%.2f               ", Inner.Kp);
            OLED_Printf(0, 16, OLED_8X16, "Ki:%.2f               ", Inner.Ki);
            OLED_Printf(0, 32, OLED_8X16, "Kd:%.2f               ", Inner.Kd);
            OLED_Printf(0, 48, OLED_8X16, "Actual:%.2f               ", Inner.Actual);
            OLED_ReverseArea(0, 48, 48, 16);

            OLED_Update();
            break;
        }
        }
    }
}

int Menu2_Clock(void)
{
    uint8_t ClFlag = 1;
    int CClFlag = 0;
    OLED_Printf(0, 0, OLED_8X16, "<-                        ");
    OLED_Printf(0, 16, OLED_8X16, "Data:%d-%d-%d            ", MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);
    OLED_Printf(0, 32, OLED_8X16, "Time:%d:%d:%d            ", MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);
    OLED_Printf(0, 48, OLED_8X16, "                      设置");

    OLED_Update();

    while (1)
    {
        MyRTC_ReadTime();
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            ClFlag--;
            if (ClFlag == 0)
            {
                ClFlag = 2;
            }
        }
        if (KeyNum == 2)
        {
            ClFlag++;
            if (ClFlag == 3)
            {
                ClFlag = 1;
            }
        }
        if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            CClFlag = ClFlag;
        }

        if (CClFlag == 1)
        {
            return 0;
        }
        if (CClFlag == 2)
        {
        }

        switch (ClFlag)
        {
        case 1:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "Data:%d-%d-%d            ", MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);
            OLED_Printf(0, 32, OLED_8X16, "Time:%d:%d:%d            ", MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);
            OLED_Printf(0, 48, OLED_8X16, "            设置");
            OLED_ReverseArea(0, 0, 16, 16);
            OLED_Update();
            break;
        }

        case 2:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "Data:%d-%d-%d            ", MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);
            OLED_Printf(0, 32, OLED_8X16, "Time:%d:%d:%d            ", MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);
            OLED_Printf(0, 48, OLED_8X16, "            设置");
            OLED_ReverseArea(96, 48, 32, 16);
            OLED_Update();
            break;
        }
        }
    }
}

int Menu2_MPU6050(void)
{
    uint8_t MPUFlag = 1;
    int MMPUFlag = 0;
    OLED_Printf(0, 0, OLED_8X16, "<-                        ");
    OLED_Printf(0, 16, OLED_8X16, "俯仰角:%.2f     ", Pitch);
    OLED_Printf(0, 32, OLED_8X16, "横滚角:%.2f     ", Roll);
    OLED_Printf(0, 48, OLED_8X16, "偏航角:%.2f     ", Yaw);

    OLED_Update();

    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            MPUFlag--;
            if (MPUFlag == 0)
            {
                MPUFlag = 4;
            }
        }
        if (KeyNum == 2)
        {
            MPUFlag++;
            if (MPUFlag == 5)
            {
                MPUFlag = 1;
            }
        }
        if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            MMPUFlag = MPUFlag;
        }

        if (MMPUFlag == 1)
        {
            return 0;
        }
        if (MMPUFlag == 2)
        {
        }
        if (MMPUFlag == 3)
        {
        }
        if (MMPUFlag == 4)
        {
        }

        switch (MPUFlag)
        {
        case 1:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "俯仰角:%.2f     ", Pitch);
            OLED_Printf(0, 32, OLED_8X16, "横滚角:%.2f     ", Roll);
            OLED_Printf(0, 48, OLED_8X16, "偏航角:%.2f     ", Yaw);
            OLED_ReverseArea(0, 0, 16, 16);
            OLED_Update();
            break;
        }

        case 2:
        {

            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "俯仰角:%.2f     ", Pitch);
            OLED_Printf(0, 32, OLED_8X16, "横滚角:%.2f     ", Roll);
            OLED_Printf(0, 48, OLED_8X16, "偏航角:%.2f     ", Yaw);
            OLED_ReverseArea(0, 16, 96, 16);
            OLED_Update();
            break;
        }

        case 3:
        {
            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "俯仰角:%.2f     ", Pitch);
            OLED_Printf(0, 32, OLED_8X16, "横滚角:%.2f     ", Roll);
            OLED_Printf(0, 48, OLED_8X16, "偏航角:%.2f     ", Yaw);
            OLED_ReverseArea(0, 32, 96, 16);
            OLED_Update();
            break;
        }

        case 4:
        {
            OLED_Printf(0, 0, OLED_8X16, "<-                        ");
            OLED_Printf(0, 16, OLED_8X16, "俯仰角:%.2f     ", Pitch);
            OLED_Printf(0, 32, OLED_8X16, "横滚角:%.2f     ", Roll);
            OLED_Printf(0, 48, OLED_8X16, "偏航角:%.2f     ", Yaw);
            OLED_ReverseArea(0, 48, 96, 16);
            OLED_Update();
            break;
        }
        }
    }
}
