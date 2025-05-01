#ifndef __Track_H
#define __Track_H

extern uint8_t Gray1, Gray2, Gray3, Gray4, Gray5, Gray6, Gray7, Gray8;

void Track_Init(void);
float Track_Calculate_Error(void);
void Trck_Read(void);


#endif
