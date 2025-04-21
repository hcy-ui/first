#ifndef __USART3_WabCam_H
#define __USART3_WabCam_H

#include <stdio.h>

void USART1_Serial_Init(void);
void USART1_Serial_SendByte(uint8_t Byte);
void USART1_Serial_SendArray(uint8_t *Array, uint16_t Length);
void USART1_Serial_SendString(char *String);
void USART1_Serial_SendNumber(uint32_t Number, uint8_t Length);
void USART1_Serial_Printf(char *format, ...);

uint8_t USART1_Serial_GetRxFlag(void);
uint8_t USART1_Serial_GetRxData(void);

#endif
