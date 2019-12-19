/*
 * servo.c
 *
 * Created: 17-12-2019 10:56:25
 *  Author: Martin Sonne
 */ 

#include <avr/io.h>
#include "servo.h"


void INIT_SERVO(){
	// Timer 3 16bit - Output: PE3/OC3A arduino: Pin5 - Servo Motor
	
	// Compare Output Mode: Fast PWM Mode: Clear OC3A on Compare Match, set OC3A at BOTTOM, non-inverting mode (Table 17-4)
	TCCR3A |= (1<<COM3A1);					// datasheet 17.11.2
	//Waveform Generation Mode: Mode 10 PWM, Phase Correct Top = ICR3: WGM31 = 1, WGM33 = 1 (Table 17-2)
	TCCR3A |= (1<<WGM31);
	TCCR3B |= (1<<WGM33);
	
	ICR3 = 20000;
	
	// Clock Select Bit: clk/8 prescaling: CS31 = 1 (Table 17-6), frekv. = 50Hz
	TCCR3B |= (1<<CS31);		// datasheet 17.11.6
	// Set output to OC3A = PE3 (arduino pin 5), see datasheet 13.4.15 and Arduino MEGA pin configuration: (pin 5)
	DDRE |= (1<<PE3);
}

void CloseLock(){
	OCR3A=500;
}

void OpenLock(){
	OCR3A=2000;
}