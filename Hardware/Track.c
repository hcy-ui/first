#include "stm32f10x.h" // Device header

uint16_t Coe;

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

/// @brief 循迹权重计算
/// @param  无
/// @return 无
float Track_Calculate_Error(void)
{
    int weight[8] = {-3, -2, -1, -0.5, 0.5, 1, 2, 3};
    uint8_t sensor[8];

    sensor[0] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
    sensor[1] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
    sensor[2] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
    sensor[3] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    sensor[4] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
    sensor[5] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
    sensor[6] = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
    sensor[7] = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);

    float error = 0;
    int count = 0;

    for (int i = 0; i < 8; i++)
    {
        if (sensor[i] == 0) // 黑线为低电平
        {
            error += weight[i];
            count++;
        }
    }

    if (count == 0)
        return 0; // 全白或全黑处理

    return error / count;
}
