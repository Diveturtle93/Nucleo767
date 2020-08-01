/*
*
*/
#include <string.h>
#include <stdlib.h>

#include "inttypes.h"
#include "main.h"
#include "usart.h"
#include "BasicUart.h"

void uartTransmit(const char *str, const size_t size)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)str, size, 1000);
}

void uartTransmitNumber(const uint32_t number, const uint32_t base)
{
	char str[11];

	itoa(number, str, base);
	HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), 1000);
}

void uartReInitBasicPolling(void)
{
	HAL_UART_DMAStop(&huart3);
	HAL_UART_DeInit(&huart3);
	HAL_UART_MspDeInit(&huart3);

	HAL_UART_Init(&huart3);
	HAL_UART_MspInit(&huart3);

	HAL_UART_DMAStop(&huart3);
	HAL_DMA_DeInit(huart3.hdmatx);
	HAL_DMA_DeInit(huart3.hdmarx);
}
