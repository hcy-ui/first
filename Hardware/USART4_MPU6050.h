#ifndef __USART3_WabCam_H
#define __USART3_WabCam_H

#include <stdio.h>

void UART4_UART4_Serial_Init(void);
void UART4_Serial_SendByte(uint8_t Byte);
void UART4_Serial_SendArray(uint8_t *Array, uint16_t Length);
void UART4_Serial_SendString(char *String);
void UART4_Serial_SendNumber(uint32_t Number, uint8_t Length);
void UART4_Serial_Printf(char *format, ...);

uint8_t UART4_Serial_GetRxFlag(void);
uint8_t UART4_Serial_GetRxData(void);

#endif
