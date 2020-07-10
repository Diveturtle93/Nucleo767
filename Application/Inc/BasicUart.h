/**
*
*/
#ifndef INC_BASICUART_H_
#define INC_BASICUART_H_

#include <stdint.h>
#include <stdio.h>

/**
*
* @param str Pointer to string
* @param size of string
*/
void uartTransmit(const char *str, const size_t size);

/**
*
* @param number number to print
* @param base to convert number to
*/
void uartTransmitNumber(const uint32_t number, const uint32_t base);


/**
* Stops all UART functions and reinitializes for basic polling operation
*/
void uartReInitBasicPolling(void);




#endif /* INC_BASICUART_H_ */
