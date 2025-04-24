#include "stm32f10x.h" // Device header

uint16_t Coe;

void Track_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出
}

float Track_Num(void)
{
    if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)&&   (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==1)&&
    (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==1))
    {   }

    
    if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)&&   (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==1)&&
    (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==1))
    {   }

    
    if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)&&   (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==1)&&
    (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==1))
    {   }

    
    if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)&&   (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==1)&&
    (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1)&&(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==1))
    {   }


}
