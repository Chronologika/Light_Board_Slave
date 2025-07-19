#ifndef _FDCAN_BSP_H_
#define _FDCAN_BSP_H_

#include "fdcan.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"

extern FDCAN_RxHeaderTypeDef CAN1_RxMessage;
extern FDCAN_RxHeaderTypeDef CAN2_RxMessage;
extern FDCAN_RxHeaderTypeDef CAN3_RxMessage;

extern uint8_t CAN1_RxData[64];
extern uint8_t CAN2_RxData[64];
extern uint8_t CAN3_RxData[64];

uint8_t FDCAN1_Init(FDCAN_HandleTypeDef *hfdcan);
uint8_t FDCAN2_Init(FDCAN_HandleTypeDef *hfdcan);
uint8_t FDCAN3_Init(FDCAN_HandleTypeDef *hfdcan);
uint8_t FDCAN_SendData(FDCAN_HandleTypeDef *hfdcan, uint8_t *TxData, uint32_t StdId, uint32_t Length);
uint8_t FDCAN_SendData_Ext(FDCAN_HandleTypeDef *hfdcan, uint8_t *TxData, uint32_t ExtId, uint32_t Length, uint32_t Data_type);

#endif
