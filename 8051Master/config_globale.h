//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: config_globale.c
// Destination: 8150F020 Carte Master
// Description: Fonctions d'initialisation
//------------------------------------------------------

#define CONFIG_GLOBALE

#ifndef C8051F020_SFR16
	#include "c8051F020_SFR16.h"
#endif
#ifndef C8051F020
	#include "c8051F020.h"
#endif
#ifndef	CONFIG_GLOBALE
//	#include "config_globale.h"
#endif
#ifndef PTC_ACCUSEDERECEPTION
	#include "PTC_accuseDeReception.h"
#endif
#ifndef PTC_CONVERTION
	#include "PTC_convertion.h"
#endif
#ifndef PTC_GETERCMD
	#include "PTC_geter_cmd.h"
#endif
#ifndef PTC_STROPERATEURS
	#include "PTC_strOperateurs.h"
#endif
#ifndef PTC_TIMER
	#include "PTC_timer.h"
#endif
#ifndef PTC_UART
	#include "PTC_UART.h"
#endif


//Place le watchdog a DEAD (off)
//@param rien
//@return rien
void Reset_Sources_Init(void);

//initialise les ports
//@param rien
//@return rien
void Port_IO_Init(void);

//initialise sysclock avec le quartz externe
//@param rien
//@return rien
void Oscillator_Init(void);

//initialise le Timer1 en baurate generator (19200baud/s)
void cfg_Clock_UART(void);

//initialise l'UART0 avec le Timer 1 en BaudRate generator (19200baud/s)
void cfg_UART0_mode1(void);

//initialise l'UART1 avec le Timer 1 en BaudRate generator (19200baud/s)
void cfg_UART1_mode1(void);

//initialise le Timer2 en mode capture
void Init_Timer2(void);

//initialise le timer 3
void Init_Timer3(void);

//initialise le timer 4
void Init_Timer4(void);

//initialise la liaison SPI 
//Fonction pas encore devellope
//@param rien
//@return rien
void Init_SPI(void);

//initialise les interuption
//@param rien
//@return rien
void Init_interrupt(void);

//initialise le Timer PCA pour les PWM
void Init_PCA(void);

//initialise le device en appelant toutes les fonctions d'initialisation
//@param rien
//@return rien
void Init_Device(void);


//initialise le robot en appelant toutes les fonctions d'initialisation
//@param rien
//@return rien
void Init_Robot(void);

//restore les parametres du robot par default
//@param rien
//@return rien
void Robot_restore(void);











