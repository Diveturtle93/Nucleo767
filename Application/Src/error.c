//----------------------------------------------------------------------
// Titel	:	Error.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	16.01.2021
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	STM32F767ZI
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
	if (status == HAL_OK) {
		uartTransmit("HAL OK\n", 7);
	}
	else if (status == HAL_ERROR) {
		uartTransmit("HAL ERROR\n", 10);
	}
	else if (status == HAL_BUSY) {
		uartTransmit("HAL BUSY\n", 9);
	}
	else if (status == HAL_TIMEOUT) {
		uartTransmit("HAL TIMEOUT\n", 12);
	}

}
//----------------------------------------------------------------------
