#include "stm32f10x.h"                  // Device header


// 变量定义部分
uint8_t stop_way = 0;                // 停止线检测计数
uint8_t sensor_val[8];               // 每个灰度传感器的当前值（从五路改为八路）
uint16_t gray_state = 0x0;           // 当前8路灰度传感器的状态（8 bit）
uint16_t gray_status[2] = {0}, gray_status_backup[2][20] = {0}; // 当前/历史灰度判断状态值
uint32_t gray_status_worse = 0;      // 记录异常（如读取全为0）状态持续次数

/****************************
函数名称：
函数作用：8路循迹获取状态
函数参数：无
函数返回值：当前巡线状态 16进制
****************************/
uint16_t Sensor_GetState(void)
{
    uint16_t State = 0x0000;

    // 读取八路传感器的状态
    for (int8_t i = 7; i >= 0; i--)
    {
        sensor_val[i] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 << i); // 根据不同的引脚读取
        State |= (sensor_val[i] << i);  // 将每个传感器的值按位放入状态
    }

    return State;
}

/****************************
函数名称：
函数作用：灰度检测处理
函数参数：无
函数返回值：无
****************************/
void gray_check(void)
{
    gray_state = Sensor_GetState();  // 获取当前传感器状态

    // 记录上一次传感器量
    for (uint16_t i = 19; i > 0; i--)
    {
        gray_status_backup[0][i] = gray_status_backup[0][i - 1];
    }
    
    gray_status_backup[0][0] = gray_status[0];

    // 灰度检测，根据当前8路传感器状态来判断
    switch (gray_state)
    {
        case 0x01: gray_status[0] = 7; gray_status_worse /= 2; break;    // 00000001b
        case 0x03: gray_status[0] = 6; gray_status_worse /= 2; break;    // 00000011b
        case 0x07: gray_status[0] = 5; gray_status_worse /= 2; break;    // 00000111b
        case 0x0F: gray_status[0] = 4; gray_status_worse /= 2; break;    // 00001111b
        case 0x1F: gray_status[0] = 3; gray_status_worse /= 2; break;    // 00011111b
        case 0x3F: gray_status[0] = 2; gray_status_worse /= 2; break;    // 00111111b
        case 0x7F: gray_status[0] = 1; gray_status_worse /= 2; break;    // 01111111b
        case 0xFF: gray_status[0] = 0; gray_status_worse /= 2; break;    // 11111111b
        case 0x00: gray_status[0] = gray_status_backup[0][0]; gray_status_worse++; break; // 全黑，可能偏离轨道
        default:    gray_status[0] = gray_status_backup[0][0]; gray_status_worse++; break;  // 其它异常情况
    }

    // 停止线检测（判断8个传感器的状态是否全部是黑线）
    switch (gray_state)
    {
        case 0xFF:  // 11111111b (全黑，可能是停止线)
        case 0x7F:  // 01111111b
        case 0x3F:  // 00111111b
            stop_way++;  // 增加停止线计数
            break;
    }
}
