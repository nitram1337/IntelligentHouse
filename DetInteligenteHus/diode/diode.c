/*
 * diode.c
 *
 * Created: 18-12-2019 17:28:58
 *  Author: Martin Sonne
 */ 

#include "diode.h"
#include <avr/io.h>

void INIT_LOCK_DIODES(){
	DDRGreenDiode |= (1 << GreenDiode);
	DDRRedDiode |= (1<< RedDiode);
}

void OnlyTurnOnGreen(){
	PortGreenDiode |= (1 << GreenDiode);
	PortRedDiode &= ~(1 << RedDiode);
}

void OnlyTurnOnRed(){
	PortRedDiode |= (1 << RedDiode);
	PortGreenDiode &= ~(1 << GreenDiode);
}

void LoggedInDiode(int loggedIn){
	if (loggedIn == 1)
	{
		OnlyTurnOnGreen();
	}
	else
	{
		OnlyTurnOnRed();
	}
}