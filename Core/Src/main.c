/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fdcan.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FDcan_bsp.h"
#include "WS2812.h"
#include "Color.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Peripherals_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t VISION_Enable = 1;
uint8_t DRIBBLE_Enable = 1;
uint8_t PUSHSHOT_Enable = 1;
uint8_t CHASSIS_Enable = 1;
uint8_t SELF_TEST_Begin = 0;
uint8_t CHALLENGE_Begin = 0;
uint8_t OFFEND_OR_DEFEND_Status = 2;
uint8_t LAST_Status = 2;
uint8_t SELF_TEST_Begin_TxFlag = 0;
uint8_t SELF_TEST_End_TxFlag = 0;
uint8_t CHALLENGE_Begin_TxFlag = 0;
uint8_t CHALLENGE_End_TxFlag = 0;
uint8_t OFFEND_TO_DEFEND_TxFlag = 0;
uint8_t DEFEND_TO_OFFEND_TxFlag = 0;
uint8_t SOMETHING_IS_GOING = 0;
uint8_t HeartBeat_Tx_Index = 0;
uint64_t USER_Systick = 0;
uint64_t SELF_TEST_Begin_Time = 0;
uint64_t CHALLENGE_Begin_Time = 0;
uint64_t LAST_Status_Change_Time = 0;
uint8_t VISION_TxData[1] = {1};
uint8_t DRIBBLE_TxData[1] = {2};
uint8_t PUSHSHOT_TxData[1] = {3};
uint8_t CHASSIS_TxData[1] = {4};
uint8_t SELF_TEST_Begin_TxData[1] = {5};
uint8_t SELF_TEST_End_TxData[1] = {6};
uint8_t CHALLENGE_Begin_TxData[1] = {7};
uint8_t CHALLENGE_End_TxData[1] = {8};
uint8_t OFFEND_TO_DEFEND_TxData[1] = {9};
uint8_t DEFEND_TO_OFFEND_TxData[1] = {10};
volatile uint8_t SYSTEM_INTERRUPT_FLAG = 0;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_FDCAN3_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  Peripherals_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* ------------------------ interrupt ------------------------ */
    if (SYSTEM_INTERRUPT_FLAG)
      continue;

    /* ------------------------ self_test ------------------------ */
    if (SELF_TEST_Begin)
    {
      SELF_TEST_Begin_Time = USER_Systick;
      SELF_TEST_Begin--;
      SELF_TEST_Begin_TxFlag = 1;
      SELF_TEST_End_TxFlag = 1;
    }

    if (SELF_TEST_Begin_TxFlag)
    {
      FDCAN_SendData(&hfdcan3, SELF_TEST_Begin_TxData, 0x105, 1);
      SELF_TEST_Begin_TxFlag = 0;
      HAL_Delay(1);
    }

    if (SELF_TEST_End_TxFlag)
    {
      if (USER_Systick - SELF_TEST_Begin_Time <= 8000)
      {
        uint8_t Spark_Number = ((USER_Systick & 0x1FF) > 256);
        uint8_t color = Spark_Number ? COLOR_GOLD : COLOR_PINK;
        WS2812_Set_Single_Color(1, Color_Table[color].R, Color_Table[color].G, Color_Table[color].B, 0.1);
      }
      else
      {
        FDCAN_SendData(&hfdcan3, SELF_TEST_End_TxData, 0x106, 1);
        SELF_TEST_End_TxFlag = 0;
        SOMETHING_IS_GOING = 0;
        HAL_Delay(1);
      }
    }

    /* ------------------------ challenge ------------------------ */
    if (CHALLENGE_Begin)
    {
      CHALLENGE_Begin_Time = USER_Systick;
      CHALLENGE_Begin--;
      CHALLENGE_Begin_TxFlag = 1;
      CHALLENGE_End_TxFlag = 1;
    }

    if (CHALLENGE_Begin_TxFlag)
    {
      FDCAN_SendData(&hfdcan3, CHALLENGE_Begin_TxData, 0x107, 1);
      CHALLENGE_Begin_TxFlag = 0;
      HAL_Delay(1);
    }

    if (CHALLENGE_End_TxFlag)
    {
      if (USER_Systick - CHALLENGE_Begin_Time <= 25000)
      {
        uint8_t Spark_Number = ((USER_Systick & 0x1FF) > 256);
        uint8_t color = Spark_Number ? COLOR_BLUE : COLOR_GREEN;
        WS2812_Set_Single_Color(1, Color_Table[color].R, Color_Table[color].G, Color_Table[color].B, 0.1);
      }
      else
      {
        FDCAN_SendData(&hfdcan3, CHALLENGE_End_TxData, 0x108, 1);
        CHALLENGE_End_TxFlag = 0;
        SOMETHING_IS_GOING = 0;
        HAL_Delay(1);
      }
    }

    /* ------------------------ offend_to_defend ------------------------ */
    if (OFFEND_OR_DEFEND_Status == 0 && USER_Systick - LAST_Status_Change_Time < 2100)
    {
      uint8_t Spark_Number = ((USER_Systick & 0x1FF) > 256);
      uint8_t color = Spark_Number ? COLOR_CYAN : COLOR_ORANGE;
      WS2812_Set_Single_Color(1, Color_Table[color].R, Color_Table[color].G, Color_Table[color].B, 0.1);

      if (OFFEND_TO_DEFEND_TxFlag)
      {
        FDCAN_SendData(&hfdcan3, OFFEND_TO_DEFEND_TxData, 0x109, 1);
        OFFEND_TO_DEFEND_TxFlag = 0;
        HAL_Delay(1);
      }

      if (USER_Systick - LAST_Status_Change_Time > 2000)
      {
        SOMETHING_IS_GOING = 0;
      }
    }

    /* ------------------------ defend_to_offend ------------------------ */
    if (OFFEND_OR_DEFEND_Status == 1 && USER_Systick - LAST_Status_Change_Time < 2100)
    {
      uint8_t Spark_Number = ((USER_Systick & 0x1FF) > 256);
      uint8_t color = Spark_Number ? COLOR_INDIGO : COLOR_MAGENTA;
      WS2812_Set_Single_Color(1, Color_Table[color].R, Color_Table[color].G, Color_Table[color].B, 0.1);

      if (DEFEND_TO_OFFEND_TxFlag)
      {
        FDCAN_SendData(&hfdcan3, DEFEND_TO_OFFEND_TxData, 0x10A, 1);
        DEFEND_TO_OFFEND_TxFlag = 0;
        HAL_Delay(1);
      }

      if (USER_Systick - LAST_Status_Change_Time > 2000)
      {
        SOMETHING_IS_GOING = 0;
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV5;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7)
  {
    WS2812_NewRound_Send(&htim3, TIM_CHANNEL_1);
  }
  else if (htim->Instance == TIM6)
  {
    USER_Systick++;
  }
  else if (htim->Instance == TIM5)
  {
    HeartBeat_Tx_Index++;
    switch (HeartBeat_Tx_Index)
    {
    case 1:
    {
      if (VISION_Enable)
      {
        FDCAN_SendData(&hfdcan1, VISION_TxData, 0x101, 1);
      }
      break;
    }
    case 2:
    {
      if (DRIBBLE_Enable)
      {
        FDCAN_SendData(&hfdcan1, DRIBBLE_TxData, 0x102, 1);
      }
      break;
    }
    case 3:
    {
      if (PUSHSHOT_Enable)
      {
        FDCAN_SendData(&hfdcan2, PUSHSHOT_TxData, 0x103, 1);
      }
      break;
    }
    case 4:
    {
      if (CHASSIS_Enable)
      {
        FDCAN_SendData(&hfdcan2, CHASSIS_TxData, 0x104, 1);
      }
      break;
    }
    default:
    {
      break;
    }
    }
    if (HeartBeat_Tx_Index == 4)
    {
      HeartBeat_Tx_Index = 0;
    }
  }
}
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if (hfdcan == &hfdcan1)
  {
    HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &CAN1_RxMessage, CAN1_RxData);
    if (CAN1_RxMessage.Identifier == 0x20)
    {
      if (SOMETHING_IS_GOING == 0)
      {
        OFFEND_OR_DEFEND_Status = 1;
        SOMETHING_IS_GOING = 1;
        DEFEND_TO_OFFEND_TxFlag = 1;
        LAST_Status_Change_Time = USER_Systick;
      }
    }
    else if (CAN1_RxMessage.Identifier == 0x90)
    {
      SYSTEM_INTERRUPT_FLAG = 1;
      Reset_All_Task_Flags();
    }
  }
  else if (hfdcan == &hfdcan3)
  {
    HAL_FDCAN_GetRxMessage(&hfdcan3, FDCAN_RX_FIFO0, &CAN3_RxMessage, CAN3_RxData);
    if (CAN3_RxMessage.Identifier == 0x30)
    {
      if (SOMETHING_IS_GOING == 0)
      {
        OFFEND_OR_DEFEND_Status = 0;
        SOMETHING_IS_GOING = 1;
        OFFEND_TO_DEFEND_TxFlag = 1;
        LAST_Status_Change_Time = USER_Systick;
      }
    }
    else if (CAN3_RxMessage.Identifier == 0x100)
    {
      SYSTEM_INTERRUPT_FLAG = 1;
    }
  }
}
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
  if (hfdcan == &hfdcan2)
  {
    HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO1, &CAN2_RxMessage, CAN2_RxData);
    if (CAN2_RxMessage.Identifier == 0x40)
    {
      if (SOMETHING_IS_GOING == 0)
      {
        SELF_TEST_Begin = 1;
        SOMETHING_IS_GOING = 1;
      }
    }
    else if (CAN2_RxMessage.Identifier == 0x50)
    {
      if (SOMETHING_IS_GOING == 0)
      {
        CHALLENGE_Begin = 1;
        SOMETHING_IS_GOING = 1;
      }
    }
  }
}
void Peripherals_Init(void)
{
  FDCAN1_Init(&hfdcan1);
  FDCAN2_Init(&hfdcan2);
  FDCAN3_Init(&hfdcan3);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim5);
}
void Reset_All_Task_Flags(void)
{
  SELF_TEST_Begin = 0;
  SELF_TEST_Begin_TxFlag = 0;
  SELF_TEST_End_TxFlag = 0;
  CHALLENGE_Begin = 0;
  CHALLENGE_Begin_TxFlag = 0;
  CHALLENGE_End_TxFlag = 0;
  OFFEND_TO_DEFEND_TxFlag = 0;
  DEFEND_TO_OFFEND_TxFlag = 0;
  SOMETHING_IS_GOING = 0;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
