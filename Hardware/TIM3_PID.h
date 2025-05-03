#ifndef __TIM3_PID_H
#define __TIM3_PID_H

typedef struct 
{
    float Target;
    float Actual;
    float Out;

    float Kp;
    float Ki;
    float Kd;

    float Error0;
    float Error1;
    float ErrorInt;

    float OutMax;
    float OutMin;

    float DifOut;//滤波变量


    int32_t ErrorIntMax;//最大积分（参考20000）
    int32_t ErrorIntMin;//反向最大积分

    int16_t Speed;
    
} PID_t;

void PID_Update(PID_t *p);
void PID_Sim_Update(PID_t *p);
void TIM3_PID_Init(void);
float TIM3_PID_Limit(float x, float min, float max);
void Update_Speed_By_Position(float outer_out, float error_pos, uint8_t wheel_id);
float TIM3_PID_Locate(float Actual_Location);

#endif
