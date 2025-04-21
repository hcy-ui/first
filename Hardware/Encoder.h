#ifndef __ENCODER_H
#define __ENCODER_H

void TIM2_Encoder_Init(void);
void TIM4_Encoder_Init(void);
int16_t TIM2_Encoder_Get(void);
int16_t TIM4_Encoder_Get(void);


#endif
