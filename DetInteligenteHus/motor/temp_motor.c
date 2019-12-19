/*
 * temp_motor.c
 *
 * Created: 18-12-2019 11:34:49
 *  Author: Martin Sonne
 */ 
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include "temp_motor.h"


void INIT_TEMP(){
	// TEMP Timer 0 - ADC0
	ADMUX = (1 << REFS0);	// 5V supply used for ADC reference, select ADC channel 0, datasheet 26.8.1
	DIDR0 = (1 << ADC0D);	// disable digital input on ADC0/PF0 arduino pin: A0, datasheet 26.8.1
	// enable ADC, start ADC, ADC clock = 16MHz / 128 = 125kHz, datasheet 26.8.3
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void INIT_MOTOR(){
	// Motor og RGB (R og G) Timer 1
	// Compare Output Mode: Fast PWM Mode: Clear OC1A, OC1B, OC1C on Compare Match, set OC1A, OC1B, OC1C at BOTTOM, non-inverting mode (Table 17-4)
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1)| (1<<COM1C1);					// datasheet 17.11.1
	//Waveform Generation Mode: Mode 5 Fast PWM: WGM10 = 1, WGM12 = 1 (Table 17-2)
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
	// Clock Select Bit: clk/64 prescaling: CS = 011 : CS01 = 1, CS00 = 1 (Table 17-6), frekv. = 980Hz
	TCCR1B |= (1<<CS11) | (1<<CS10);		// datasheet 17.11.5
	// Set output to OC0A = PB7, see datasheet 13.4.6 and Arduino MEGA pin configuration: (pin 13), same as LED
	DDRB |= (1<<PB7) | (1<<PB6) | (1<<PB5);
}

void INIT_TEMP_INTERRUPT_TIMER4(unsigned int ms)
{
	TCCR4B |= (1<<WGM42);						// Mode 4, CTC = WGM42, 17.11.7   Table 17-2
	TIMSK4 |= (1<<OCIE4A);						// Timer/Counter4, Output Compare A Match Interrupt Enable, 17.11.35
	OCR4A = (ms * (F_CPU / 1000) / 256) - 1;	// OCR4A = (Tdelay * F_CPU / N) - 1 => 61.5 gives 1ms => 62499 gives 1s
	TCCR4B |= (1<<CS42);						// Prescaler: 256, CS=100, Table 17-6. Timer starts!
}

volatile static unsigned int tempc;

ISR(TIMER4_COMPA_vect)
{
	float ADC_data;
	float tempk;
	
	float ADC_voltage;
	
	if (!(ADCSRA & (1 << ADSC)))
	{												// check if ADC conversion is complete
		ADC_data = log(10000.0 * (1024.0/ADC -1));
		tempk = 1/(0.001129148+(0.000234125+(0.0000000876741*ADC_data*ADC_data))*ADC_data);
			
		tempc = tempk-273.15;

		ADCSRA |= (1 << ADSC);						// start the next ADC conversion
		_delay_ms(100);
	}
			
	// Temp over 30 grader = Maks speed blæser og Grøn
	if(tempc > 30)
	{
	OCR1A = 255;	// Motor
	OCR1B = 0;		// Rød
	OCR1C = 255;	// Grøn
	}
	if(tempc < 30)
	{
	OCR1A = 0;		// Motor
	OCR1B = 255;	// Rød
	OCR1C = 0;		// Grøn
	}
}

int GetTemp(){
	return tempc;
}


