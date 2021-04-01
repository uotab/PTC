//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.c
// Destination: 8150F020 Carte Master
// Description: Fonctions qui permette de cecomposer les commandes du PC de commande
//------------------------------------------------------
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_deplacement.h"
#include "PTC_geter_cmd.h"
#include "PTC_servoMoteurHorizontal.h"
#include "PTC_strOperateurs.h"
#include "UART0_RingBuffer_lib.h"

//------------------------------------------------------
//------------------------------------Variables Globales
extern int              epreuve_enable;
extern int              vitesse_par_default;

extern int              go_coordinates_x;
extern int              go_coordinates_y;
extern int              go_coordinates_angle;
extern char             state_go_coordinates;

extern unsigned char    temp_servo_H;
extern char             flag_print_arrive_servo_H;

//------------------------------------------------------
//------------------------------------------------------

void Cmd_epreuve_D(const char *Pchaine_courante) {
    char str_type_epreuve[2] = {0};

	get_param(Pchaine_courante, 1, str_type_epreuve);
	if (my_strlen(str_type_epreuve) > 0) {//il y a un parametre
		int type_epreuve = my_atoi(str_type_epreuve);
		if (type_epreuve > 0 && type_epreuve < 9) {
			AR_cmd_correcte();
			epreuve_enable = type_epreuve;
			serOutstring("I Le robot a demarre l'epreuve !\r\n");
		}
		else
			AR_cmd_incorrecte();
	}
	else {
		AR_cmd_correcte();
		epreuve_enable = 1;
		serOutstring("I Le robot a demarre l'epreuve !\r\n");
	}
}

void Cmd_epreuve_E(const char *Pchaine_courante) {
    char str_type_epreuve[2] = {0};

	get_param(Pchaine_courante, 1, str_type_epreuve);
	AR_cmd_correcte(); 
	epreuve_enable = 0;
}

void Cmd_epreuve_A(const char *Pchaine_courante) {
    char str_param[12]  = {0};
	int param           = 0  ;

	get_param(Pchaine_courante,1, str_param);
	if (my_strlen(str_param) > 0) {
		param = my_atoi(str_param);
		if (param <= 100 && param >= 5){
			AR_cmd_correcte();
			Avancer(str_param);
		}
		else {
			AR_cmd_incorrecte();
		}
	}
	else {
		char str_vitesse_par_default[3] = {0};

		AR_cmd_correcte();
		Avancer(my_itoa(vitesse_par_default,str_vitesse_par_default));
	}
}

void Cmd_epreuve_B(const char *Pchaine_courante) {
    char str_param[12]  = {0};
	int param           = 0  ;

	get_param(Pchaine_courante,1, str_param);
	if (my_strlen(str_param) > 0) {
		param = my_atoi(str_param);
		if (param <= 100 && param >= 5) {
			AR_cmd_correcte();
			Reculer(str_param);
		}
		else
			AR_cmd_incorrecte();
	}
	else {
		char str_vitesse_par_default[3] = {0};
		AR_cmd_correcte();
		Reculer(my_itoa(vitesse_par_default,str_vitesse_par_default));
	}
}

void Cmd_epreuve_RG(void) {
    AR_cmd_correcte();
	turn_left(90);
} 

void Cmd_epreuve_RD(void) {
    AR_cmd_correcte();
	turn_right(90);
}

void Cmd_epreuve_RC(const char *Pchaine_courante) {
	char str_param[12] = {0};

	get_param(Pchaine_courante,1, str_param);
	if (my_strcmp(str_param,"D")){
		AR_cmd_correcte();
		turn_right(180);
	}
	else if (my_strcmp(str_param,"G")){
		AR_cmd_correcte();
		turn_left(180);
	}
	else 
		AR_cmd_incorrecte();
}

void Cmd_epreuve_RA(const char *Pchaine_courante) {
    char str_param[8] = {0};
	char str_angle[4] = {0};
	char direction[2] = {0};

	get_param(Pchaine_courante,1,str_param);
	get_complex_param(str_param,direction,str_angle);
	if (my_strcmp(direction,"D")){
		AR_cmd_correcte();
		turn_right(my_atoi(str_angle));
	}
	else if (my_strcmp(direction,"G")){
		AR_cmd_correcte();
		turn_left(my_atoi(str_angle));
	}
	else
		AR_cmd_incorrecte();
}

void Cmd_epreuve_S(void) {
    AR_cmd_correcte();
	Stop();
}

void Cmd_epreuve_G(const char *Pchaine_courante) {
	int xval, yval, angle;
	char i;

	for (i = 0; i < 3; i++) {
		char str_param[32] 	   = {0};
		char str_name_param[2] = {0};
		char str_val_param[6]  = {0};

		get_param(Pchaine_courante, i+1, str_param);
		get_complex_param(str_param, str_name_param, str_val_param);
		if (my_strcmp(str_name_param,"X")){
			xval = 10 * my_atoi(str_val_param);
			if (xval < -9900 || xval > 9900) {
				AR_cmd_incorrecte();
				return; //on sort de la fonction
			}
		}
		else if (my_strcmp(str_name_param,"Y")){
			yval = 10 * my_atoi(str_val_param);
			if (yval < -9900 || yval > 9900) {
				AR_cmd_incorrecte();
				return; //on sort de la fonction
			}
		}
		else if (my_strcmp(str_name_param,"A")){
			angle = my_atoi(str_val_param);
			if (angle < -180 || angle > 180) {
				AR_cmd_incorrecte();
				return; //on sort de la fonction
			}
		}
		else 
			AR_cmd_correcte();		
	}
	state_go_coordinates = 1; //activation de la machine d'etat pour rejoindre les ccords
	go_coordinates_x     = xval;
	go_coordinates_y     = yval;
	go_coordinates_angle = angle;
}

void Cmd_epreuve_TV(const char *Pchaine_courante) {
    char str_param[4] = {0};

	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0){
		int param = my_atoi(str_param); 

		if (param > 6 && param < 101) {
			AR_cmd_correcte();
			vitesse_par_default = param;
		}
		else 
			AR_cmd_incorrecte();
	}
	else 
		AR_cmd_incorrecte();
}

void Cmd_epreuve_CS(const char *Pchaine_courante) {
    char str_param[2] = {0};

	get_param(Pchaine_courante,1,str_param);
	if (my_strcmp(str_param, "V")) { //cervo vertical
		AR_cmd_correcte();
		//TODO commande cervo vertical
	}

	else if (str_param[0] == 'A') { //cervo par default H
		char str_param_name[2]  = {0};
		char str_param_value[4] = {0};
		get_complex_param(str_param, str_param_name, str_param_value);
		if (my_strcmp(str_param_name, "A")) {
			char angle = (char)my_atoi(str_param_value);
			if (angle > -91 && angle < 91) {
				AR_cmd_correcte();
				CDE_Servo_H(angle);
			}
			else
				AR_cmd_incorrecte();
		}
		else
			AR_cmd_incorrecte();
	}

	else if (my_strcmp(str_param,"H")) {//cervo horizontal 
		char str_param2[8] = {0};
		get_param(Pchaine_courante, 2,str_param2);
		if (my_strlen(str_param2) > 0) {
			char str_param_name[2]  = {0};
			char str_param_value[4] = {0};
			get_complex_param(str_param2, str_param_name, str_param_value);
			if (my_strcmp(str_param_name, "A")) {
				char angle = (char)my_atoi(str_param_value);
				if (angle > -91 && angle < 91) {
					AR_cmd_correcte();
					temp_servo_H = (char)10*CDE_Servo_H(angle);
					flag_print_arrive_servo_H =1;
				}
				else
					AR_cmd_incorrecte();
			}
			else
				AR_cmd_incorrecte();
		}
		else {
			AR_cmd_correcte();
			temp_servo_H = (char)10*CDE_Servo_H(0);
			flag_print_arrive_servo_H =1;
		}
	}
}