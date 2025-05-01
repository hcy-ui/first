#include "stm32f10x.h" // Device header

uint16_t Coe;
uint8_t Gray1 = 0, Gray2 = 0, Gray3 = 0, Gray4 = 0, Gray5 = 0, Gray6 = 0, Gray7 = 0, Gray8 = 0;

/// @brief 八路循迹
/// @param  无
void Track_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/// @brief 读取灰度值（6和7不灵敏，要么全亮，要么不亮）
/// @param
void Trck_Read(void)
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        Gray1 = 1;
    }
    else
    {
        Gray1 = 0;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
    {
        Gray2 = 1;
    }
    else
    {
        Gray2 = 0;
    }

    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)
    {
        Gray3 = 1;
    }
    else
    {
        Gray3 = 0;
    }
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)
    {
        Gray4 = 1;
    }
    else
    {
        Gray4 = 0;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)
    {
        Gray5 = 1;
    }
    else
    {
        Gray5 = 0;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
    {
        Gray6 = 1;
    }
    else
    {
        Gray6 = 0;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
    {
        Gray7 = 1;
    }
    else
    {
        Gray7 = 0;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
    {
        Gray8 = 1;
    }
    else
    {
        Gray8 = 0;
    }
}

/// @brief 循迹权重计算
/// @param  无
/// @return 无
float Track_Calculate_Error(void)
{
    float weight[8] = {-4, -2, -1.5, -1, 1, 1.5, 2, 4};//误差为负则偏右，误差为正则偏左
    uint8_t sensor[8];

    sensor[0] = Gray1;
    sensor[1] = Gray2;
    sensor[2] = Gray3;
    sensor[3] = Gray4;
    sensor[4] = Gray5;
    sensor[5] = Gray6;
    sensor[6] = Gray7;
    sensor[7] = Gray8;

    float error = 0;
    int count = 0;

    for (int i = 0; i < 8; i++)
    {
        if (sensor[i] == 1) // 黑线为低电平
        {
            error += weight[i];
            count++;
        }
    }

    if (count == 0)
        return 0; // 全白或全黑处理

    return error / count;
}
