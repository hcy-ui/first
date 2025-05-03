#include "stm32f10x.h" // Device header
#include "TIM3_PID.h"  // Device header
#include "math.h"
#include "menu.h"
#include "TIM2_4_ENCODER.h"

extern PID_t Speed_Control_Left;
extern PID_t Speed_Control_Right;
extern PID_t OOuter;
#define PI 3.14159265358979323846		 // 圆周率
#define encoder_pulses_per_revolution 13.0 // 电机编码器线数
#define gear_reduction_ratio 20.0			 // 减速比
#define wheel_diameter_mm 48.0			 // 轮子直径(mm)
#define pulse 4.0							 // 编码器脉冲倍频（一般不需要改）

/// @brief 超级pid模板
/// @param 无
void PID_Update(PID_t *p)
{

	p->Error1 = p->Error0;
	p->Error0 = p->Target - p->Actual;
	if (fabs(p->Error0) < 5) // 输入死区（针对误差较小，防止频繁调控）
	{
		p->Out = 0;
	}
	else
	{
		if (p->Ki != 0) // Ki不等于0，才开始积分
		{
			if (fabs(p->Error0) < 50) // 积分分离（开始时误差大，不需要积分项）（50为参考值）
			{
				p->ErrorInt += p->Error0;
				if (p->ErrorInt > p->ErrorIntMax) // 积分限幅（参考值20000）
				{
					p->ErrorInt = p->ErrorIntMax;
				}
				if (p->ErrorInt < p->ErrorIntMin)
				{
					p->ErrorInt = p->ErrorIntMin;
				}
			}
			else
			{
				p->ErrorInt = 0;
			}
		}
		else
		{
			p->ErrorInt = 0;
		}

		float a = 0.9;														   // （a越大，滤波效果越强，但响应越慢）
		p->DifOut = (1 - a) * p->Kd * (p->Error0 - p->Error1) + a * p->DifOut; // 低通滤波（微分先行）（对积分项进行滤波）

		p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->DifOut; // 位置式积分
	}

	if (p->Out > 0) // 输出偏移（以小博大）

	{
		p->Out += 5; // （5为参考值）
	}
	else if (p->Out < 0)
	{
		p->Out -= 5;
	}
	else
	{
		p->Out = 0;
	}

	if (p->Out > p->OutMax) // 输出限幅（一般电机为100）
	{
		p->Out = p->OutMax;
	}
	if (p->Out < p->OutMin)
	{
		p->Out = p->OutMin;
	}
}

/// @brief 简化pid模板
/// @param 无
void PID_Sim_Update(PID_t *p)
{

	p->Error1 = p->Error0;
	p->Error0 = p->Target - p->Actual;
	if (p->Ki != 0) // Ki不等于0，才开始积分
	{
		if (fabs(p->Error0) < 1000) // 积分分离（开始时误差大，不需要积分项）（50为参考值）
		{
			p->ErrorInt += p->Error0;
			if (p->ErrorInt > p->ErrorIntMax) // 积分限幅（参考值20000）
			{
				p->ErrorInt = p->ErrorIntMax;
			}
			if (p->ErrorInt < p->ErrorIntMin)
			{
				p->ErrorInt = p->ErrorIntMin;
			}
		}
		else
		{
			p->ErrorInt = 0;
		}
	}
	else
	{
		p->ErrorInt = 0;
	}

	float a = 0.5;														   // （a越大，滤波效果越强，但响应越慢）
	p->DifOut = (1 - a) * p->Kd * (p->Error0 - p->Error1) + a * p->DifOut; // 低通滤波（微分先行）（对积分项进行滤波）

	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->DifOut; // 位置式积分

	if (p->Out > p->OutMax) // 输出限幅（一般电机为100）
	{
		p->Out = p->OutMax;
	}
	if (p->Out < p->OutMin)
	{
		p->Out = p->OutMin;
	}
}

/**
 * 函    数：定时中断初始化
 * 参    数：无
 * 返 回 值：无
 */
void TIM3_PID_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启TIM3的时钟

	/*配置时钟源*/
	TIM_InternalClockConfig(TIM3); // 选择TIM3为内部时钟，若不调用此函数，TIM默认也为内部时钟

	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				// 定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;				// 计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				// 预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);				// 将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元

	/*中断输出配置*/
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); // 清除定时器更新标志位
										  // TIM_TimeBaseInit函数末尾，手动产生了更新事件
										  // 若不清除此标志位，则开启中断后，会立刻进入一次中断
										  // 如果不介意此问题，则不清除此标志位也可

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 开启TIM3的更新中断

	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置NVIC为分组2
													// 即抢占优先级范围：0~3，响应优先级范围：0~3
													// 此分组配置在整个工程中仅需调用一次
													// 若有多个中断，可以把此代码放在main函数内，while循环之前
													// 若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置

	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					  // 定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  // 选择配置NVIC的TIM3线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  // 指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							  // 将结构体变量交给NVIC_Init，配置NVIC外设

	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE); // 使能TIM3，定时器开始运行
}

float TIM3_PID_Limit(float x, float min, float max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

void Update_Speed_By_Position(float outer_out, float error_pos, uint8_t wheel_id)
{
    float speed_cmd;

    if (abs(error_pos) < 10)
    {
        speed_cmd = 0;
    }
    else if (abs(error_pos) < 50)
    {
        speed_cmd = TIM3_PID_Limit(outer_out, -15, 15);
    }
    else if (abs(error_pos) < 100)
    {
        speed_cmd = TIM3_PID_Limit(outer_out, -30, 30);
    }
    else
    {
        speed_cmd = TIM3_PID_Limit(outer_out, -50, 50);
    }

    if (wheel_id == 0)
	Speed_Control_Left.Target = speed_cmd;
    else if (wheel_id == 1)
	Speed_Control_Right.Target = speed_cmd;
}


/// @brief 直接输入位置即可（单位：mm）
/// @param Actual_Location 
float TIM3_PID_Locate(float Actual_Location)
{
	return Actual_Location*encoder_pulses_per_revolution*gear_reduction_ratio*pulse/PI/wheel_diameter_mm;
}

