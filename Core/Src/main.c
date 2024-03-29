/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BasicUart.h"
#include "SystemInfo.h"
#include "Error.h"
#include "..\..\Application\Src\my_math.c"
#include "CAN_Bus.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEMP_SENSOR_AVG_SLOPE_MV_PER_CELSIUS                        2.5f
#define TEMP_SENSOR_VOLTAGE_MV_AT_25                                760.0f
#define ADC_REFERENCE_VOLTAGE_MV                                    3300.0f
#define ADC_MAX_OUTPUT_VALUE                                        4095.0f
#define TEMP110_CAL_VALUE                                           ((uint16_t*)((uint32_t)0x1FF0F44E))
#define TEMP30_CAL_VALUE                                            ((uint16_t*)((uint32_t)0x1FF0F44C))
#define TEMP110                                                     110.0f
#define TEMP30                                                      30.0f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static volatile uint16_t rising = 0;
static volatile uint16_t falling = 0;
//CAN_RxHeaderTypeDef rxHeader;
uint8_t rxData[8];
int32_t temperature;
float sensorValue;
//float adcCalValue30 = (float)(*TEMP30_CAL_VALUE);
//float adcCalValue110 = (float)(*TEMP110_CAL_VALUE);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	// Definiere Variable
	CAN_FilterTypeDef sFilterConfig;
	uint16_t dutyCycle, timerPeriod, frequency;
	uint8_t HAL_STATUS = 0;
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
  MX_USART3_UART_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USART2_UART_Init();
  MX_CAN3_Init();
  MX_ADC1_Init();
  MX_TIM13_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	printResetSource(readResetSource());

//	timerPeriod = (HAL_RCC_GetPCLK2Freq() / htim1.Init.Prescaler);

//	if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK);
//	if (HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1) != HAL_OK);
//	if (HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2) != HAL_OK);
	CAN_TxHeaderTypeDef TxHeader;

	TxHeader.StdId = 0x321;
	TxHeader.ExtId = 0x01;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime=DISABLE;
	uint8_t txData[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	/*
	* Testing UART, polling
	*/
	#define TEST_STRING_UART  "\nUART3 Transmitting in polling mode, Hello Diveturtle93!\n"
	uartTransmit(TEST_STRING_UART, sizeof(TEST_STRING_UART));
//	ITM_SendString(TEST_STRING_UART);

	collectSystemInfo();

	/*
	* Testing LEDs
	*
	* Toggles each of the 3 available LEDs seperatly for a second.
	*/
	#define TEST_STRING_LEDS  "\nTesting LEDs\n"
	uartTransmit(TEST_STRING_LEDS, sizeof(TEST_STRING_LEDS));

	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	uartTransmit(".", 1);
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);

	HAL_Delay(1000);
	uartTransmit(".", 1);

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	uartTransmit(".", 1);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	HAL_Delay(1000);
	uartTransmit(".", 1);

	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	uartTransmit(".", 1);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	HAL_Delay(1000);
	uartTransmit(".\n", 2);

	HAL_TIM_Base_Start_IT(&htim13);

	#define STRING_LED_FUNCTION	"All LEDs are functional.\n"
	uartTransmit(STRING_LED_FUNCTION, sizeof(STRING_LED_FUNCTION));

	uartTransmit("CAN START\n", 10);
	if((HAL_STATUS = HAL_CAN_Start(&hcan1)) != HAL_OK)
	{
		hal_error(HAL_STATUS);
		Error_Handler();
	}

	// Filter Bank initialisieren um Daten zu empfangen
	// Akzeptiere alle CAN-Pakete
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000 << 5;
	sFilterConfig.FilterIdLow = 0x0;
	sFilterConfig.FilterMaskIdHigh = 0x0000 << 5;
	sFilterConfig.FilterMaskIdLow = 0x0;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterActivation = ENABLE;

	// Filter Bank schreiben
	if((HAL_STATUS = HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig)) != HAL_OK)
	{
		// Fehler beim konfigurieren der Filterbank fuer den CAN-Bus
		hal_error(HAL_STATUS);
		Error_Handler();
	}

	// Aktiviere Interrupts fuer CAN Bus
	if((HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)) != HAL_OK)
	{
		// Fehler in der Initialisierung des CAN Interrupts
		Error_Handler();
	}

	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, txData, (uint32_t *)CAN_TX_MAILBOX0);
	/*uartTransmit("Temperatur messen\n", 18);
	uartTransmitNumber(*TEMP30_CAL_VALUE, 10);
	uartTransmit("\n", 1);
	uartTransmitNumber(*TEMP110_CAL_VALUE, 10);
	uartTransmit("\n", 1);
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
	{
	    sensorValue = (float)HAL_ADC_GetValue(&hadc1);
	    HAL_ADC_Stop(&hadc1);
	    temperature = (int32_t)((TEMP110 - TEMP30) / ((float)(*TEMP110_CAL_VALUE) - (float)(*TEMP30_CAL_VALUE)) * (sensorValue - (float)(*TEMP30_CAL_VALUE)) + TEMP30);
	}
	else
	{
	    temperature = -273;
	}
	uartTransmitNumber(temperature, 10);
	uartTransmit("\n", 1);

	uartTransmit("Send Message\n", 13);
	ITM_SendString(TEST_STRING_UART);*/
  /* USER CODE END 2 */

  /* Init scheduler */
//  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
//  MX_FREERTOS_Init();

  /* Start scheduler */
//  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	CAN_config();
  while (1)
  {
	  CAN_write(&hcan1);
	  /*if (rising != 0 && falling != 0) {
		  int diff = getDifference(rising, falling);
		  dutyCycle = round((float)(diff * 100) / (float)rising);               // (width / period ) * 100
		  frequency = timerPeriod / rising;               // timer restarts after rising edge so time between two rising edge is whatever is measured
	  } else {
		  dutyCycle = 0;
		  frequency = 0;
	  }

	  uartTransmitNumber(dutyCycle, 10);
	  uartTransmitNumber(frequency, 10);*/
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_3);
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_rx_read(hcan, CAN_RX_FIFO0);
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
	if (htim->Instance == TIM13) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	}
	if (htim == &htim1) {
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			rising = calculateMovingAverage(rising, HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1), 64);
		} else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
			falling = calculateMovingAverage(falling, HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_2), 64);
		}
	}
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	uartTransmit("Error\n", 6);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
