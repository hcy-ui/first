#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"

float Pitch,Roll,Yaw;	//ŷ����(����̬����ָ���ȡ),�ֱ�Ϊ������,�����,ƫ����

void MPU_EXTI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//AFIO���������ⲿ�ж�����
	
	/*GPIO����*/
	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO�ṹ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������ģʽ(Ĭ�ϸߵ�ƽ)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			//PB14��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);				//��ʼ��GPIOB����
	
	/*AFIO�ⲿ�ж���������*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);//ѡ��MPU��INT�ӵ�PB14��Ϊ�ⲿ�ж�����
	
	/*EXTI����*/
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	/*NVIC����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//����2,��2λ��ռ2λ��Ӧ
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//10��15��ͨ���ϲ�Ϊ��ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//�ж�ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�0
	NVIC_Init(&NVIC_InitStructure);
		
}

/**
  * @brief �ж���14,PB14�жϺ���,��MPU6050���ݱ仯ʱ,�����ж�
  * @param  ��
  * @retval ��
  */
//void EXTI15_10_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line14) == SET)//ȷ����PB14�������ж�
//	{
//		/*���ݽ���*/
//		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
//		EXTI_ClearITPendingBit(EXTI_Line14);//����жϱ�־λ
//	}	
//}


