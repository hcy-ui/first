#ifndef __USART3_WabCam_H
#define __USART3_WabCam_H

#include <stdio.h>

void USART3_Serial_Init(void);
void USART3_Serial_SendByte(uint8_t Byte);
void USART3_Serial_SendArray(uint8_t *Array, uint16_t Length);
void USART3_Serial_SendString(char *String);
void USART3_Serial_SendNumber(uint32_t Number, uint8_t Length);
void USART3_Serial_Printf(char *format, ...);

uint8_t USART3_Serial_GetRxFlag(void);
uint8_t USART3_Serial_GetRxData(void);

#endif
