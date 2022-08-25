//----------------------------------------------------------------------
// Titel	:	CAN_config.c
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	Aug 25, 2022
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	STM32F767ZI
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Einfuegen der eigenen Include Dateien
//----------------------------------------------------------------------
#include "CAN_Bus.h"
//----------------------------------------------------------------------

// Konfiguriere CAN Nachrichten
//----------------------------------------------------------------------
void CAN_config(void)
{
	CAN_Output_Paket_Liste[0] = CAN_Nachricht(0x108, 1, 7, 17);
	CAN_Output_Paket_Liste[1] = CAN_Nachricht(0x283, 1, 20, 13);
	CAN_Output_Paket_Liste[2] = CAN_Nachricht(0x372, 2, 25, 14);
	CAN_Output_Paket_Liste[3] = CAN_Nachricht(0x182, 1, 25, 7);
	CAN_Output_Paket_Liste[4] = CAN_Nachricht(0x728, 5, 100, 32);
	CAN_Output_Paket_Liste[5] = CAN_Nachricht(0x635, 8, 200, 65);
	CAN_Output_Paket_Liste[6] = CAN_Nachricht(0x263, 1, 150, 24);
	CAN_Output_Paket_Liste[7] = CAN_Nachricht(0x364, 1, 200, 53);
	CAN_Output_Paket_Liste[8] = CAN_Nachricht(0x634, 2, 100, 64);
	CAN_Output_Paket_Liste[9] = CAN_Nachricht(0x736, 1, 500, 74);
	CAN_Output_Paket_Liste[10] = CAN_Nachricht(0x516, 5, 100, 32);
	CAN_Output_Paket_Liste[11] = CAN_Nachricht(0x453, 8, 200, 35);
	CAN_Output_Paket_Liste[12] = CAN_Nachricht(0x392, 1, 120, 67);
	CAN_Output_Paket_Liste[13] = CAN_Nachricht(0x483, 1, 150, 39);
	CAN_Output_Paket_Liste[14] = CAN_Nachricht(0x504, 2, 130, 45);
	CAN_Output_Paket_Liste[15] = CAN_Nachricht(0x694, 1, 140, 68);
	CAN_Output_Paket_Liste[16] = CAN_Nachricht(0x029, 5, 100, 18);
	CAN_Output_Paket_Liste[17] = CAN_Nachricht(0x102, 8, 200, 6);
	CAN_Output_Paket_Liste[18] = CAN_Nachricht(0x206, 1, 500, 38);
	CAN_Output_Paket_Liste[19] = CAN_Nachricht(0x371, 1, 400, 98);
	CAN_Output_Paket_Liste[20] = CAN_Nachricht(0x193, 2, 100, 87);
	CAN_Output_Paket_Liste[21] = CAN_Nachricht(0x274, 1, 120, 83);
	CAN_Output_Paket_Liste[22] = CAN_Nachricht(0x089, 5, 100, 93);
	CAN_Output_Paket_Liste[23] = CAN_Nachricht(0x036, 8, 200, 59);
}
//----------------------------------------------------------------------
