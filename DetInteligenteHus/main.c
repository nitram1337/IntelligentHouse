/*
 * DetInteligenteHus.c
 *
 * Created: 16-12-2019 12:58:47
 * Author : Martin Sonne
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "keypad/keypad.h"
#include "servo/servo.h"
#include "motor/temp_motor.h"
#include "diode/diode.h"
#include <avr/interrupt.h>

#include <util/delay.h>

#define delay _delay_ms(50);

int main(void)
{
	int column;
	int row;
	INIT_LOCK_DIODES();
	INIT_KEYPAD();
	INIT_SERVO();
	INIT_TEMP();
	INIT_MOTOR();
	INIT_TEMP_INTERRUPT_TIMER4(1000);
	
	sei();
	
	while (1)
	{
		column = ColumnScan();
		delay;
		row = ReadRows();
		DecodeKeyboard(row, column);
	}
}

