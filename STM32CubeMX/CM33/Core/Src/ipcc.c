/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ipcc.c
  * @brief   This file provides code for the configuration
  *          of the IPCC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "ipcc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

IPCC_HandleTypeDef hipcc1;

/* IPCC1 init function */
void MX_IPCC1_Init(void)
{

  /* USER CODE BEGIN IPCC1_Init 0 */

  /* USER CODE END IPCC1_Init 0 */

  /* USER CODE BEGIN IPCC1_Init 1 */

  /* USER CODE END IPCC1_Init 1 */
  hipcc1.Instance = IPCC1;
  if (HAL_IPCC_Init(&hipcc1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IPCC1_Init 2 */

  /* USER CODE END IPCC1_Init 2 */

}

void HAL_IPCC_MspInit(IPCC_HandleTypeDef* ipccHandle)
{

  if(ipccHandle->Instance==IPCC1)
  {
  /* USER CODE BEGIN IPCC1_MspInit 0 */

  /* USER CODE END IPCC1_MspInit 0 */

    /* IPCC1 interrupt Init */
    HAL_NVIC_SetPriority(IPCC1_RX_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(IPCC1_RX_IRQn);
  /* USER CODE BEGIN IPCC1_MspInit 1 */

  /* USER CODE END IPCC1_MspInit 1 */
  }
}

void HAL_IPCC_MspDeInit(IPCC_HandleTypeDef* ipccHandle)
{

  if(ipccHandle->Instance==IPCC1)
  {
  /* USER CODE BEGIN IPCC1_MspDeInit 0 */

  /* USER CODE END IPCC1_MspDeInit 0 */

    /* IPCC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(IPCC1_RX_IRQn);
  /* USER CODE BEGIN IPCC1_MspDeInit 1 */

  /* USER CODE END IPCC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

