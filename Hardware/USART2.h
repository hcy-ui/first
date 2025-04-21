#ifndef __USART2_WabCam_H
#define __USART2_WabCam_H

#include <stdio.h>

void USART2_Serial_Init(void);
void USART2_Serial_SendByte(uint8_t Byte);
void USART2_Serial_SendArray(uint8_t *Array, uint16_t Length);
void USART2_Serial_SendString(char *String);
void USART2_Serial_SendNumber(uint32_t Number, uint8_t Length);
void USART2_Serial_Printf(char *format, ...);

uint8_t USART2_Serial_GetRxFlag(void);
uint8_t USART2_Serial_GetRxData(void);
    
#endif
