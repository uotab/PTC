//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: config_globale.c
// Destination: 8150F020 Carte Master
// Description: Fonctions d'initialisation
//------------------------------------------------------

///Pour le debug sans le robot 
//#define WAIT_RX_ROBOT

#include "config_globale.h"
#include "UART1_RingBuffer_lib.h"
#include "UART0_RingBuffer_lib.h"
#include "SPI_RingBuffer_Master.h"
#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK 22118400 //approximate SYSCLK frequency in Hz
   #define BAUDRATE  19200L          // Baud rate of UART in bps
                                   // Le caractère 'L' force l'évaluation de BAUDRATE en entier long
   #define SCK              200000    // Frequence de la liaison SPI
   #define SPACE_TRAME      25       //espacement tres trames (en nb de trame exemple si SPACE_TRAME = 100, il y a un trame toutes les X periode de trame)
   #define T_TRAME_SPI      (8*(1/SCK))
   #define T_RECURRENCE_SPI (SPACE_TRAME*T_TRAME_SPI)
   #define T_T3             (T_RECURRENCE_SPI*SYSCLK)
#endif


extern char Flag_RX1;

sbit INT6 = P3^6;

void Reset_Sources_Init(){
  //Desactivation du Watchdog
	 WDTCN = 0xDE;
	 WDTCN = 0XAD;
}
//test com

void Port_IO_Init() {
    //GND                                        //B-0
    //3.3V                                       //A-0

    // P0.0  -  Tx, 				 Pull-push, Digital //C-12
    // P0.1  -  Rx, 				 Open-Drain, Digital//B-12
    // P0.2  -  SCK       ,  Push-Pull , Digital//A-12
    // P0.3  -  MISO      ,  Open-Drain, Digital//C-11
    // P0.4  -  MOSI      ,  Push-Pull , Digital//B-11
    // P0.5  -  NSS       ,  Open-Drain, Digital//A-11
    // P0.6  -  Tx1,         Pull-push, Digital  //C-10
    // P0.7  -  Rx1,         Open-Drain, Digital//B-10

    // P1.0  -  Servo H   ,  Puss_pull, Digital //C-04
    // P1.1  -  T2EX      ,  Open-Drain, Digital //B-04//telemetrie
    // P1.2  -  Unassigned,  Open-Drain, Digital//A-04
    // P1.3  -  Unassigned,  Open-Drain, Digital//C-03
    // P1.4  -  Unassigned,  Open-Drain, Digital //B-03
    // P1.5  -  Unassigned,  Open-Drain, Digital//A-03
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    // P2.0  -  SS        ,  Pull-push , Digital //B-7
    // P2.1  -  Unassigned,  Open-Drain, Digital
    // P2.2  -  Unassigned,  Open-Drain, Digital
    // P2.3  -  Unassigned,  Open-Drain, Digital
    // P2.4  -  Unassigned,  Open-Drain, Digital
    // P2.5  -  Unassigned,  Open-Drain, Digital
    // P2.6  -  Unassigned,  Open-Drain, Digital
    // P2.7  -  Unassigned,  Open-Drain, Digital

    // P3.0  -  Unassigned,  Open-Drain, Digital
    // P3.1  -  Triger AV ,  Push-Pull, Digital
    // P3.2  -  Triger AR ,  Push-Pull , Digital
    // P3.3  -  Commande capt Timer 2,  Push-Pull , Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  INT6 (echo),  Open-Drain, Digital Input INT6
    // P3.7  -  Unassigned,  Open-Drain, Digital Input INT7
		
	// P4.0 to P7.7   Unassigned,  Open-Drain, Digital

     P0MDOUT |= (1<<2); //SCK
     P0MDOUT |= (1<<4); //MOSI
     P2MDOUT |= (1<<0); //SS

		P0MDOUT |= (1<<0); //P0.0
		P0MDOUT |= (1<<6); //P0.6
		P1MDOUT |= (1<<0); //P1.0 servo
    XBR1 |= 0x40; // Validation crossbar T2EX
    XBR2 |= 0x40; //enable le crossbar
    XBR0 |= (1<<3); // route le signal CEX0 sur un port pin (servo) 
    P3MDOUT |= 0x06; //Configuration P3.1 et P3.2 en push-pull
    P3 |= 0x40; // Configuration de P3.6 en input
	  INT6 = 1; // Configuration de P3.6 en input
	  EIE2 |= 0x10; // Autorisation interruption INT6
	  // Sensibilite de /INT6 initialement mise a front montant
	  P3IF |= 0x04;
	  P3IF &= 0xBF;
    //SPI
    XBR0 |= (1<<1); //SCK -> P0.0 | MISO -> P0.1 | MOSI -> P0.2 | NSS -> P0.3
}


//-----------------------------------------------------------------------------
// Config oscillateur - SYSCLK = 22,1184MHz - Oscillateur externe � quartz 
//-----------------------------------------------------------------------------
/*void Oscillator_Init_Osc_Quartz(){
		int i = 0;
    OSCXCN    = 0x67;  // Config de l'horloge externe - Quartz > 6,7 MHz
    for (i = 0; i < 3000; i++){} // attente stabilisation Fosc quartz
    while ((OSCXCN & 0x80) == 0); // validation stabilite du quartz
    OSCICN    = 0x0C;  // Commutation sur oscillateur externe 
	                     // L'oscillateur n'est pas stopp
}*/

void Oscillator_Init() {
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN    = 0x0C;
}


#define Preload_Timer0 (SYSCLK/(BAUDRATE*16))
#if Preload_Timer0 > 255 
#error "Valeur Preload Timer0 HORS SPECIFICATIONS"
#endif 
void cfg_Clock_UART(void) {
  CKCON |= 0x10;      // T1M: Timer 1 use the system clock.
  TMOD |= 0x20;       //  Timer1 auto reload
	TMOD &= 0x2f;			  // Timer1 configure en timer 8 bit avec auto-reload	
	TF1 = 0;				  // Flag Timer efface

  TH1 = 0xB8;
	//TH1 = -(Preload_Timer0);
	ET1 = 0;				   // Interruption Timer 1 devalidee
	TR1 = 1;				   // Timer1 demarre
}

void cfg_UART0_mode1(void) {
  init_Serial_Buffer();//init des buffers
	    //Config Crossbar
		XBR2 |= (1<<6); //Crossbar enable
    XBR0 |= (1<<2); //Tx -> P0.0 & Rx -> P0.1
		RCLK0 = 0;     // Source clock Timer 1
		TCLK0 = 0;
		PCON  |= 0x80; //SMOD0: UART0 Baud Rate Doubler Disabled.
		PCON &= 0xBF;  // SSTAT0=0
		SCON0 = 0x70;   // Mode 1 - Check Stop bit - Reception validee
		TI0 = 1;        // Transmission: octet transmis (pret e recevoir un char
					          // pour transmettre			
    ES0 = 1;        // interruption UART0 autorisee	
}

void cfg_UART1_mode1(void){
  init_Serial_Buffer_uart1();//buffers
	//crossbar
  XBR2  |= (1<<6); //Crossbar enable
	XBR2  |= (1<<2);//Tx1 = P0.6  Rx = P0.7
	
	PCON  |= (1<<4);
	//config uart
	SCON1 |= (1<<6);//Mode 1 8bit de data asynchrone
	SCON1 |= (1<<5);//Bit de Stop enable
  SCON1 |= (1<<4);//reception active

 // SCON1 |= (1<<1); //TI1 Transmission: octet transmis (pret a recevoir un char
					          // pour transmettre	
  EIE2  |= (1<<6); //ES1 interruption UART1 autorisee	
}

void Init_SPI() {
  init_Serial_Buffer_SPI();
    //Config de l'horloge
    SPI0CFG &= ~(0xC0); //Polarite et etat horloge
    SPI0CKR = SYSCLK / (2 * SCK) - 1; //fixe la frequence de SCK

    //Actication de master
    MSTEN = 1;

    //nb de bit de shift
    SPI0CFG |= 0x07; //8bits

    //Actication de SPI
    SPIEN = 1;

    //Activation de l'interruption 
    EIE1 |= (1<<0);
    
    //TODO Voir si on peut placer SS a 1 en mode push-pull pour eviter d'avoir a le faire en hardware
}

void Init_Timer3(void) { //Utiliser pour la SPI
    //TMR3RL  = 0xFFFF - T_T3; //valeur de reload
		TMR3RL  = 0xFFFF - 12118; //valeur de reload
    //TMR3RL  = 0xFFFF - 22118; //valeur de reload
    TMR3CN |= (1<<2); //Timer 3 enable
    TMR3CN |= (1<<1); //SYSCLK/1

    EIE2   |= (1<<0); //ISR enable
}

void Init_Timer4() {
// autoreload avec 43417 sans division par 12 pour avoir 1ms
//Mode 1
T4CON |= 0x04;
EIE2 |= (1<<2);
//interrupt 16

//reload value
RCAP4 = 0xA99A;// = 43418

}
//-----------------------------------------------------------------------------
// Initialisation des interuption
//-----------------------------------------------------------------------------
void Init_interrupt(void){
    ES0   = 1;      //UART0
		EIE2 |= (1<<6); //UART1
   // EA    = 1;      //Interuption General
    EIE2 |= 0x10; // Interruption INT6
    EIP2 |= 0x10; // Priorite haute pour INT6
    ET2 = 1; // Interruption overflow Timer2
}

void Init_Timer2(void){
    RCLK0 = 0;
    TCLK0 = 0;
    CPRL2 = 1;
    TR2 = 1;
    EXEN2 = 1;
    T2CON |= 0x08; // Autorise les captures sur le timer2
}

//PCA pour le servomoteur 

void Init_PCA()
{
	PCA0MD = 0x00; //sysclk divided by 12
	PCA0CN |= 0x41; // on choisit le module 0 ( bit 0 mit à 1) et enable PCA counter timer
	PCA0CPM0 = 0xC2; // mode PWM 16 bits et on enable le PWM0

	// Registres de rechargement 
	PCA0CPH0 = 0; //On initialise.Pour déterminer le duty high
	PCA0CPL0 = 0;
	
}

//-----------------------------------------------------------------------------
// Initialisation globale du Microcontroleur - 
//-----------------------------------------------------------------------------
void Init_Device(void) {
    Reset_Sources_Init();
    Port_IO_Init();
    Oscillator_Init();
    Init_Timer4();
		Init_Timer3();
    Init_Timer2();
    Init_PCA();
    cfg_Clock_UART();
    cfg_UART0_mode1();
    cfg_UART1_mode1();
    Init_SPI();
    Init_interrupt();
    EA = 1;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////Initialisation du robot

void Init_Robot(){
  Robot_restore();
  //TODO Config l'encoder
}

void Robot_restore(){
  serOutstring_uart1("restore\r");
  #ifdef WAIT_RX_ROBOT
    while (serInchar_uart1()==0);
    Delay(10);
  #endif
}

