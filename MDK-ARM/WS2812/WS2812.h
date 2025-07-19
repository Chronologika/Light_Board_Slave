#ifndef _WS2812_H
#define _WS2812_H

#include "tim.h"
#include "Color.h"

#define LED_number 2

extern uint32_t Send_Buffer[24 * LED_number + 50];

void WS2812_Set_Single_Color(uint16_t led_index, uint8_t R, uint8_t G, uint8_t B, float brightness);
void WS2812_Append_Reset(void);
void WS2812_NewRound_Send(TIM_HandleTypeDef *htim, uint32_t TIM_CHANNEL);
void WS2812_DisPlay_HeartBeat_Error(void);


#endif 
