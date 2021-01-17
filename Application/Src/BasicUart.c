//----------------------------------------------------------------------
// Titel	:	Error.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	16.01.2021
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	STM32F767ZI
//----------------------------------------------------------------------

// Einfügen der standard Include-Dateien
//----------------------------------------------------------------------
#include "string.h"
#include "stdlib.h"
#include "inttypes.h"
//----------------------------------------------------------------------

// Einfügen der STM Include-Dateien
//----------------------------------------------------------------------
#include "main.h"
#include "usart.h"
//----------------------------------------------------------------------

// Einfügen der eigenen Include Dateien
//----------------------------------------------------------------------
#include "BasicUart.h"
//----------------------------------------------------------------------

// Uart3 Transmit Funktion
//----------------------------------------------------------------------
void uartTransmit(const char *str, const size_t size)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)str, size, 1000);
}
//----------------------------------------------------------------------

// Uart3 Transmit Nummer Funktion
//----------------------------------------------------------------------
void uartTransmitNumber(const uint32_t number, const uint32_t base)
{
	char str[11];

	itoa(number, str, base);
	HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), 1000);
}
//----------------------------------------------------------------------

// Deinitialisieren des Uart3
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
