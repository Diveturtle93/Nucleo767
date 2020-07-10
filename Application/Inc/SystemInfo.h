/**
*
*/

#ifndef SYSTEMINFO_H_
#define SYSTEMINFO_H_

#include <inttypes.h>

typedef enum {
	STARTUP = 0x00,
	IWDG1 = 0x01,
	WWDG1 = 0x02,
	CPURST1 = 0x04,
	BORST1 = 0x08,
	PORST1 = 0x10,
	SFTRST1 = 0x20,
	PINRST1 = 0x40,
	RMVF1 = 0x80,
} reset_reason;

/**
* Collects informations and print it
*/
void collectSystemInfo();

/**
* Reads CPU registers to determine if system is restarting
*/
reset_reason readResetSource();

/**
* Prints restart information to console
*/
void printResetSource(reset_reason reset_flags);

#endif /* SYSTEMINFO_H_ */
/** @} */ // End of GroupLabel
