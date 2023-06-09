//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_timer.h
// Destination: 8150F020 Carte Master
// Description: Fonctions du temps
//------------------------------------------------------




//Retourne le temps en ms depuis de debut
//@param rien
//@return le temps en ms
unsigned long int get_time_ms(void);

//Incremente le temps
//Fonction appeler par interuption toutes les ms
//@param rien
//@return rien
void Time_increment(void);


//Attend un delay de time_ms
//@param time_ms temp en ms
//@return rien
void Delay(const unsigned long time_ms);

//Attend un delay de 1ms
//@param rien
//@return rien
void Delay_1ms(void);

//Attend un delay de 1us !!Tres peu precis!!
//@param rien
//@return rien
void Delay_1us(void);

//Attend un delay de 10us
//@param rien
//@return rien
void Delay_10us(void);