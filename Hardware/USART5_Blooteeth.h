#ifndef __USART5_WabCam_H
#define __USART5_WabCam_H

#include <stdio.h>

void UART5_Serial_Init(void);
void UART5_Serial_SendByte(uint8_t Byte);
void UART5_Serial_SendArray(uint8_t *Array, uint16_t Length);
void UART5_Serial_SendString(char *String);
void UART5_Serial_SendNumber(uint32_t Number, uint8_t Length);
void UART5_Serial_Printf(char *format, ...);

uint8_t UART5_Serial_GetRxFlag(void);
uint8_t UART5_Serial_GetRxData(void);

#endif
