/*
*
*/

#include "inttypes.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "SystemInfo.h"
#include "BasicUart.h"
#include "git.h"

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"         //Collect Information from RTOS CMSIS LAYER
#include "task.h"             //Collect Information from RTOS


/**
* Collects Version information from Middleware and prints it
*/
void collectMiddlewareInfo()
{
	#define STRING_CMSIS_VERSION        "\nCMSIS Version:\t\t"
	#define STRING_HAL_VERSION          "\nHAL Version:\t\t"
	#define STRING_RTOS_CMSIS_VERSION   "\nRTOS CMSIS Version:\t"
	#define STRING_RTOS_VERSION         "\nRTOS Version:\t\t"

	uartTransmit(STRING_CMSIS_VERSION, sizeof(STRING_CMSIS_VERSION));
	uartTransmitNumber(__CM7_CMSIS_VERSION_MAIN, 10);
	uartTransmit(".", 1);
	uartTransmitNumber(__CM7_CMSIS_VERSION_SUB, 10);


	uartTransmit(STRING_HAL_VERSION, sizeof(STRING_HAL_VERSION));
	uartTransmitNumber((uint32_t)((HAL_GetHalVersion() >> 24) & 0xFF), 10);

	uartTransmit(".", 1);
	uartTransmitNumber((uint32_t)((HAL_GetHalVersion() >> 16) & 0xFF), 10);

	uartTransmit(".", 1);
	uartTransmitNumber((uint32_t)((HAL_GetHalVersion() >> 8) & 0xFF), 10);

	uartTransmit(".", 1);
	uartTransmitNumber((uint32_t)(HAL_GetHalVersion() & 0xFF), 10);


	uartTransmit(STRING_RTOS_CMSIS_VERSION, sizeof(STRING_RTOS_CMSIS_VERSION));
	uartTransmitNumber((osCMSIS >> 16), 10);

	uartTransmit(".", 1);
	uartTransmitNumber((osCMSIS & 0xFFFF), 10);


	uartTransmit(STRING_RTOS_VERSION , sizeof(STRING_RTOS_VERSION));
	uartTransmitNumber(tskKERNEL_VERSION_MAJOR, 10);

	uartTransmit(".", 1);
	uartTransmitNumber(tskKERNEL_VERSION_MINOR, 10);

	uartTransmit(".", 1);
	uartTransmitNumber(tskKERNEL_VERSION_BUILD, 10);

	uartTransmit("\n", 1);
}


/**
* Collects Software information and prints it
*/
void collectSoftwareInfo()
{
	#define STRING_GIT_VERSION			"\nGit Version:\t\t"
	#define STRING_GIT_BRANCH			"\nGit Branch:\t\t"
	#define STRING_GIT_HASH				"\nGit Hash:\t\t"
	#define STRING_BUILD_DATE			"\nBuild Date:\t\t"
	#define STRING_BUILD_TIME			"\nBuild Time:\t\t"

	uartTransmit(STRING_GIT_VERSION, sizeof(STRING_GIT_VERSION));
	uartTransmit(GIT_COMMIT, sizeof(GIT_COMMIT));

	uartTransmit(STRING_GIT_BRANCH, sizeof(STRING_GIT_BRANCH));
	uartTransmit(GIT_BRANCH, sizeof(GIT_BRANCH));

	uartTransmit(STRING_GIT_HASH, sizeof(STRING_GIT_HASH));
	uartTransmit(GIT_HASH, sizeof(GIT_HASH));

	uartTransmit(STRING_BUILD_DATE, sizeof(STRING_BUILD_DATE));
	uartTransmit(BUILD_DATE, sizeof(BUILD_DATE));

	uartTransmit(STRING_BUILD_TIME, sizeof(STRING_BUILD_TIME));
	uartTransmit(BUILD_TIME, sizeof(BUILD_TIME));

	//uartTransmit("\n", 1);
}


/**
* Collects hardware information from microcontroller and prints it
*/
void collectHardwareInfo()
{
	#define STRING_STM_DEVICE_ID        "\nSTM32 Device ID:\t"
	#define STRING_STM_REVISION         "\nSTM32 Revision ID:\t"
	#define STRING_STM_FREQ             "\nSTM32 CPU-Freq:\t\t"
	#define STRING_STM_UUID             "\nSTM32 UUID:\t\t"

	uartTransmit(STRING_STM_DEVICE_ID, sizeof(STRING_STM_DEVICE_ID));
	uartTransmitNumber(HAL_GetDEVID(), 10);

	uartTransmit(STRING_STM_REVISION, sizeof(STRING_STM_REVISION));
	
	switch(HAL_GetREVID())
	{
		case 0x1001:
			uartTransmit("Z", 1);
			break;
		case 0x1003:
			uartTransmit("Y", 1);
			break;
		case 0x2001:
			uartTransmit("X", 1);
			break;
		default:
			uartTransmitNumber(HAL_GetREVID(), 10);
			break;
	}


	uartTransmit(STRING_STM_FREQ, sizeof(STRING_STM_FREQ));
	{
		uint32_t frequency = HAL_RCC_GetSysClockFreq();
		frequency = frequency/1000000;

		uartTransmitNumber(frequency, 10);
	}

	uartTransmit(" MHz", 4);


	uartTransmit(STRING_STM_UUID, sizeof(STRING_STM_UUID));
	uartTransmitNumber(HAL_GetUIDw0(), 16);

	uartTransmit(" ", 1);
	uartTransmitNumber(HAL_GetUIDw1(), 16);

	uartTransmit(" ", 1);
	uartTransmitNumber(HAL_GetUIDw2(), 16);

	uartTransmit("\n", 1);
}


/**
* Collects Information from microcontroller and send to UART
*/
void collectSystemInfo()
{
	#define STRING_HARDWARE_TITEL "\n\t --Hardware--\n"
	#define STRING_MIDDLEWARE_TITEL "\n\t --Middleware--\n"
	#define STRING_SOFTWARE_TITEL "\n\t --Software--\n"

	uartTransmit(STRING_HARDWARE_TITEL, sizeof(STRING_HARDWARE_TITEL));
	collectHardwareInfo();

	uartTransmit(STRING_MIDDLEWARE_TITEL, sizeof(STRING_MIDDLEWARE_TITEL));
	collectMiddlewareInfo();

	uartTransmit(STRING_SOFTWARE_TITEL, sizeof(STRING_SOFTWARE_TITEL));
	collectSoftwareInfo();

	uartTransmit("\n", 1);
}


/**
* Collects Reset source Flag microcontroller
*/
reset_reason readResetSource(void)
{
	reset_reason reset_flags = STARTUP;

	// Prüfe Reset Flag Internen Watchdog
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) == true)
	{
		reset_flags += IWDG1;
	}

	// Prüfe Reset Flag Window Watchdog
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) == true)
	{
		reset_flags += WWDG1;
	}

	// Prüfe Reset Flag Low Power Reset
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) == true)
	{
		reset_flags += CPURST1;
	}

	// Prüfe Reset Flag Brown Out Reset
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) == true)
	{
		reset_flags += BORST1;
	}

	// Prüfe Reset Flag Power On Reset
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) == true)
	{
		reset_flags += PORST1;
	}

	// Prüfe Reset Flag Software Reset
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) == true)
	{
		reset_flags += SFTRST1;
	}

	// Prüfe Reset Flag Pin-Reset
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) == true)
	{
		reset_flags += PINRST1;
	}

	// Lösche alle Reset Flags
	__HAL_RCC_CLEAR_RESET_FLAGS();

	return reset_flags;
}


/**
* Print reset source from microcontroller
*/
void printResetSource(reset_reason reset_flags)
{
	if (reset_flags == STARTUP)
	{
		uartTransmit("Regular Start\r\n", 15);
	}
	else
	{
		if (reset_flags & IWDG1)
		{
			uartTransmit("Interner Watchdog Reset\n", 24);
		}

		if (reset_flags & WWDG1)
		{
			uartTransmit("Window Watchdog Reset\n", 22);
		}

		if (reset_flags & CPURST1)
		{
			uartTransmit("CPU Reset\n", 10);
		}
		if (reset_flags & BORST1)
		{
			uartTransmit("Brown Out Reset\n", 16);
		}

		if (reset_flags & PORST1)
		{
			uartTransmit("Power On Reset\n", 15);          //Power on Reset / Power down Reser
		}

		if (reset_flags & SFTRST1)
		{
			uartTransmit("Software Reset\n", 15);
		}

		if (reset_flags & PINRST1)
		{
			uartTransmit("PIN Reset\n", 10);          //NRST pin
		}

		if (reset_flags & RMVF1)
		{
			uartTransmit("RMVF\n", 5);                //NRST pin
		}
	}
}
