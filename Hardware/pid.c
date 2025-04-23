#include "stm32f10x.h" // Device header
#include "pid.h"       // Device header
#include "math.h"

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
            if (fabs(p->Error0 < 50)) // 积分分离（开始时误差大，不需要积分项）（50为参考值）
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
        }
        else
        {
            p->ErrorInt = 0;
        }

        float a = 0.9;                                                         // （a越大，滤波效果越强，但响应越慢）
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
    else if (p->Out < p->OutMin)
    {
        p->Out = p->OutMin;
    }
}

#include "stm32f10x.h"                  // Device header

/**
  * 函    数：定时中断初始化
  * 参    数：无
  * 返 回 值：无
  */
void Tim2_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//开启TIM2的时钟
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元	
	
	/*中断输出配置*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行
}

/* 定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/

