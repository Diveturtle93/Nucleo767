//----------------------------------------------------------------------
// Titel	:	Error.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	16.01.2021
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	STM32F767ZI
//----------------------------------------------------------------------

// Einfügen der STM Include-Dateien
//----------------------------------------------------------------------
#include "main.h"
//----------------------------------------------------------------------

// Einfügen der eigenen Include Dateien
//----------------------------------------------------------------------
#include "Error.h"
#include "BasicUart.h"
//----------------------------------------------------------------------

// Hal Error auswerten und ausgeben
//----------------------------------------------------------------------
void hal_error(uint8_t status)
{
#ifdef DEBUG
	if (status == HAL_OK) {												// HAL OK
		uartTransmit("HAL OK\n", 7);
	}
	else if (status == HAL_ERROR) {										// HAL Error
		uartTransmit("HAL ERROR\n", 10);
	}
	else if (status == HAL_BUSY) {										// HAL Beschäftigt
		uartTransmit("HAL BUSY\n", 9);
	}
	else if (status == HAL_TIMEOUT) {									// HAL Timeout
		uartTransmit("HAL TIMEOUT\n", 12);
	}
#endif
}
//----------------------------------------------------------------------

// Debug Nachricht ueber SWO senden
// Nachricht SWO ITM Data Console
// http://stefanfrings.de/stm32/cube_ide.html
// Core Clock := Maximalfrequenz
// Im String #GRN# oder #RED# oder #ORG# erscheint die Nachricht in einer Farbe
void ITM_SendString(char *ptr)
{
	while(*ptr)
	{
		ITM_SendChar(*ptr);
		ptr++;
	}
}
