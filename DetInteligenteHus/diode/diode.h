/*
 * diode.h
 *
 * Created: 18-12-2019 17:28:48
 *  Author: Martin Sonne
 */ 


#ifndef DIODE_H_
#define DIODE_H_

#pragma region Defines
	#define GreenDiode		PJ1
	#define RedDiode		PJ0
	#define PortGreenDiode	PORTJ
	#define PortRedDiode	PORTJ
	#define DDRGreenDiode	DDRJ
	#define DDRRedDiode		DDRJ
#pragma endregion Defines

#pragma region FunctionPrototyping
	void INIT_LOCK_DIODES();
	void OnlyTurnOnGreen();
	void OnlyTurnOnRed();
	void LoggedInDiode(int loggedIn);
#pragma endregion FunctionPrototyping

#endif /* DIODE_H_ */