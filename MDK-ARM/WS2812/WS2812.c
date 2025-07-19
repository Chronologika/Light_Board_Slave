#include "WS2812.h"

uint32_t Send_Buffer[24 * LED_number + 50] = {0};
uint8_t High_CRR = 112;
uint8_t Low_CRR = 56;

void WS2812_Set_Single_Color(uint16_t led_index, uint8_t R, uint8_t G, uint8_t B, float brightness)
{
	if (led_index >= LED_number)
		return;

	if (brightness < 0.0f)
		brightness = 0.0f;
	if (brightness > 1.0f)
		brightness = 1.0f;

	uint16_t index = led_index * 24;
	uint8_t g_scaled = G * brightness;
	uint8_t r_scaled = R * brightness;
	uint8_t b_scaled = B * brightness;

	for (int i = 7; i >= 0; i--)
		Send_Buffer[index++] = (g_scaled >> i) & 0x01 ? High_CRR : Low_CRR;

	for (int i = 7; i >= 0; i--)
		Send_Buffer[index++] = (r_scaled >> i) & 0x01 ? High_CRR : Low_CRR;

	for (int i = 7; i >= 0; i--)
		Send_Buffer[index++] = (b_scaled >> i) & 0x01 ? High_CRR : Low_CRR;
}

void WS2812_Append_Reset(void)
{
	uint32_t index = LED_number * 24;
	for (int i = 0; i < 50; i++)
	{
		Send_Buffer[index++] = 0;
	}
}

void WS2812_NewRound_Send(TIM_HandleTypeDef *htim, uint32_t TIM_CHANNEL)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL);
	WS2812_Append_Reset();
	HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL, Send_Buffer, sizeof(Send_Buffer) / sizeof(uint32_t));
}
