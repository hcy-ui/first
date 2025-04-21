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
