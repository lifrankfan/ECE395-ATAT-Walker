/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>

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
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;


TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

int32_t CCR_reg = 0;
int32_t hip = 0;
int32_t knee = 0;

int32_t receivedNumber = 0;
uint8_t flag = 0;
uint8_t Degree[]  = {0,0};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void init_legs();
void rest_legs();
void move_leg(TIM_HandleTypeDef *tim, uint32_t hip_channel, uint32_t knee_channel);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// hip, knee
int32_t Init[] = {110, 110}; // straight
int32_t Rest[] = {140, 140};
int32_t Bend[] = {110, 25};
int32_t Reach[] = {45, 25};
int32_t Step[] = {80, 80};

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
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);



	HAL_UART_Receive_IT(&huart1, Degree, sizeof(Degree));
	CCR_reg = 0;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	//  	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, CCR_reg); //PWM1 110 knee
	//  	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, CCR_reg); //PWM2 110 hip
	//  	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, CCR_reg); //PWM3 110 knee
	//  	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, CCR_reg); //PWM4 110 hip
	//  	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, CCR_reg); //PWM5 110 knee
	//  	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, CCR_reg); //PWM6	110 hip
	//  	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, CCR_reg); //PWM7 110 knee
	//  	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, CCR_reg); //PWM8 110 hip
	uint8_t start = 1;
	uint32_t counter = 0;
	char buffer[50];
	uint8_t button_state = 1; // 0 means button pressed
	uint8_t walk = 1;			// 0 means walk
	while (1) {
	/* USER CODE END WHILE */

	button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
	if (button_state == 0) {
		walk = 0;
	}

	if (start) {
		strcpy(buffer, "initializing legs\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

		init_legs();
		start = 0;
	}

	button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
		if (button_state == 0) {
			walk = 0;
	}
	sprintf(buffer, "Button Value: %d\r\n", walk);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);

	if (walk == 0) {
		sprintf(buffer, "Button Value: %d\r\n", walk);
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
		rest_legs();
				sprintf(buffer, "Button Value: %d\r\n", walk);
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);

		button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
		if (button_state == 0) {
			walk = 1;
		}
		// hip, knee
		if (walk == 0) {
		move_leg(&htim2, TIM_CHANNEL_4, TIM_CHANNEL_3); // leg 4
		}
		button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
			if (button_state == 0) {
			walk = 1;
		}
		sprintf(buffer, "Button Value: %d\r\n", walk);
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
		if (walk == 0) {
		move_leg(&htim3, TIM_CHANNEL_1, TIM_CHANNEL_2); // leg 2
		}
		button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
			if (button_state == 0) {
			walk = 1;
		}
		sprintf(buffer, "Button Value: %d\r\n", walk);
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
		if (walk == 0) {
		move_leg(&htim3, TIM_CHANNEL_4, TIM_CHANNEL_3); // leg 1
		}
		button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
			if (button_state == 0) {
			walk = 1;
		}
		sprintf(buffer, "Button Value: %d\r\n", walk);
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
		if (walk == 0) {
		move_leg(&htim2, TIM_CHANNEL_2, TIM_CHANNEL_1); // leg 3
		}
		button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // Read button state
			if (button_state == 0) {
			walk = 1;
		}
		sprintf(buffer, "Button Value: %d\r\n", walk);
		HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
		init_legs();

	}
	sprintf(buffer, "Button Value: %d\r\n", walk);
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);

	counter++;
	HAL_Delay(1000);
	}
    /* USER CODE BEGIN 3 */
}
void init_legs() {
  // leg1
    hip = ((Init[0]) / 0.09 + 250);
    knee = ((Init[1]) / 0.09 + 250);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, hip);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, knee);
    HAL_Delay(1000);
    // leg3
    hip = ((Init[0]) / 0.09 + 250);
    knee = ((Init[1]) / 0.09 + 250);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, hip);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, knee);
    HAL_Delay(1000);
    // leg2
    hip = ((Init[0]) / 0.09 + 250);
    knee = ((Init[1]) / 0.09 + 250);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, hip);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, knee);
    HAL_Delay(1000);
    // leg4
    hip = ((Init[0]) / 0.09 + 250);
    knee = ((Init[1]) / 0.09 + 250);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, hip);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, knee);
    HAL_Delay(1000);
}

void rest_legs() {
	// leg4
	hip = ((Rest[0]) / 0.09 + 250);
	knee = ((Rest[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, hip);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, knee);
	HAL_Delay(2000);
	// leg2
	hip = ((Rest[0]) / 0.09 + 250);
	knee = ((Rest[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, hip);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, knee);
	HAL_Delay(1000);
	// leg3
	hip = ((Rest[0]) / 0.09 + 250);
	knee = ((Rest[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, hip);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, knee);
	HAL_Delay(1000);
  // leg1
	hip = ((Rest[0]) / 0.09 + 250);
	knee = ((Rest[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, hip);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, knee);
	HAL_Delay(1000);
}

void move_leg(TIM_HandleTypeDef *tim, uint32_t hip_channel, uint32_t knee_channel) {
	// Bend
	hip = ((Bend[0]) / 0.09 + 250);
	knee = ((Bend[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(tim, hip_channel, hip);
	__HAL_TIM_SET_COMPARE(tim, knee_channel, knee);

	HAL_Delay(1000);
	// Reach
	hip = ((Reach[0]) / 0.09 + 250);
	knee = ((Reach[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(tim, hip_channel, hip);
	__HAL_TIM_SET_COMPARE(tim, knee_channel, knee);

	HAL_Delay(1000);
	// Step
	hip = ((Step[0]) / 0.09 + 250);
	knee = ((Step[1]) / 0.09 + 250);
	__HAL_TIM_SET_COMPARE(tim, hip_channel, hip);
	__HAL_TIM_SET_COMPARE(tim, knee_channel, knee);
    HAL_Delay(1000);
}


  /* USER CODE END 3 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//Function for writing to UART
	receivedNumber = (Degree[0] - 48);
	if(receivedNumber == 0){
		flag = 1;
	}else{
		flag = 0;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	}
	memset(Degree, 0, sizeof(Degree));
	HAL_UART_Receive_IT(&huart1, Degree, sizeof(Degree));
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 15;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 15;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  // Configure GPIOC2 as input
  GPIO_InitStruct.Pin = GPIO_PIN_2;        // Only PC2
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Set as input
  GPIO_InitStruct.Pull = GPIO_NOPULL;     // No internal pull (since external pull-up exists)
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Configure GPIOC1 as output (original configuration)
  GPIO_InitStruct.Pin = GPIO_PIN_1;        // Only PC1
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;         // No internal pull
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
