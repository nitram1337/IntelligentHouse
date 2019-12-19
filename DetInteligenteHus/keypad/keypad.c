/*
 * keypad.c
 *
 * Created: 13-12-2019 12:27:44
 *  Author: Martin Sonne
 */ 
#define F_CPU 16E6
#include <avr/io.h>
#include <util/delay.h>
#include "keypad.h"
#include "../LCD/lcd.h"
#include "../servo/servo.h"
#include "../diode/diode.h"


void INIT_KEYPAD(){
	// Defines til output portene
	DDRK |= (1 << Col1) | (1 << Col2) | (1 << Col3) | (1 << Col4);		// Sætter portene K0 til K3 til output (Column lines)
	PORTK |= (1 << Col1) | (1 << Col2) | (1 << Col3) | (1 << Col4);		// Sætter 1 på portene K0 til K3 (Column lines)
	
	// Defines til input portene
	DDRK &= ~(1 << Row1 | 1 << Row2 | 1 << Row3 | 1 << Row4);			// Sætter portene K4 til K7, til input (Er default sat til 0 uanset)(row lines)
	PORTK |= (1 << Row1) | (1 << Row2) | (1 << Row3) | (1 << Row4);		// Enabler pull-up på portene K4 til K7, som er sat til at være input
	
}

// Scanner fra column 1 og så videre. Indtil den når til 4, så starter den på 1 igen næste gang
// Returnere den scannede column
int ColumnScan(){
	static columnCounter = 1;
	PORTK |= (1 << Col1) | (1 << Col2) | (1 << Col3)| (1 << Col4);
	
	switch (columnCounter)
	{
		case 1:
			PORTK &= ~(1 << Col1);
			columnCounter++;
			return 1;
		break;
		case 2:
			PORTK &= ~(1 << Col2);
			columnCounter++;
			return 2;
		break;
		case 3:
			PORTK &= ~(1 << Col3);
			columnCounter++;
			return 3;
		break;
		case 4:
			PORTK &= ~(1 << Col4);
			columnCounter = 1;
			return 4;
		break;
	}
}

// Returnere tallet på row'et, som er trykket. Hvis row'et er logisk 0. Ellers returnere den bare 0
int ReadRows()
{
	if (~PINK & (1 << Row4))
	{
		_delay_ms(10);
		if (~PINK & (1 << Row4))
		{
			return 4;
		}
	}
	if (~PINK & (1 << Row3))
	{
		_delay_ms(10);
		if (~PINK & (1 << Row3))
		{
			return 3;
		}
	}
	if (~PINK & (1 << Row2))
	{
		_delay_ms(10);
		if (~PINK & (1 << Row2))
		{
			return 2;
		}
	}
	if (~PINK & (1 << Row1))
	{
		_delay_ms(10);
		if (~PINK & (1 << Row1))
		{
			return 1;
		}
	}
	return 0;
}

// Udskriver det character, som er blevet trykket. Ved at finde det udfra hvad row og column er
void DecodeKeyboard(int row, int column)
{
	static char indtastetKode[4]; // Gemmer den fire cifret kode her, som brugeren indtaster på keypad'en
	static int codeCounter = 0;
	static int loggedIn = 0;
	
	// Bliver kun initialised 1 gang
	static char keyPad[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};
	
	// Run LCD Init once
	static int count = 1;
	if (count == 1)
	{
		lcd_init(LCD_DISP_ON_CURSOR_BLINK);
		lcd_puts("Enter Code:");
		count++;
	}
	
	// Der bliver returneret 0 i ReadRows, hvis der ikke er læst en character, derfor skal der tjekkes på 0

	if (row!=0)
	{
		switch (keyPad[row-1][column-1])
		{
			case 'A':
				if (loggedIn == 0)
				{
					// Hvis koden er rigtig
					if (codeCounter == 4 && indtastetKode[0] == '1' && indtastetKode[1] == '2' && indtastetKode[2] == '3' && indtastetKode[3] == '4')
					{
						loggedIn = 1;
						OpenLock();
						lcd_clrscr();
						lcd_puts("Velkommen!\n");
						lcd_puts("Laast op!");
						_delay_ms(2000);

						lcd_clrscr();
						lcd_puts("C:Se temperatur\nB:Logout");
					}
					else
					{				
						lcd_clrscr();
						lcd_puts("Forkert kode!\nB:Prov igen");
						codeCounter = 0;
					}
				}
			break;
			case 'B':
				// Hvis man allerede er logget ind
				if (loggedIn == 1)
				{
					CloseLock();
					lcd_clrscr();
					codeCounter = 0;
					loggedIn = 0;
					count = 1;
				}
				else
				{
					lcd_clrscr();
					count = 1;
					codeCounter = 0;
				}
			break;
			case 'C':
				if (loggedIn == 1)
				{
					lcd_clrscr();
					
					char buffer[2];
					/* convert integer into string */
					itoa( GetTemp() , buffer, 10);
					lcd_puts("Temp: ");
					lcd_puts(buffer);
					
					lcd_puts("\nB:Logout C:Update");
				}
			break;
			default:
			if (codeCounter < 4)
			{
				lcd_putc(keyPad[row-1][column-1]);
				indtastetKode[codeCounter] = keyPad[row-1][column-1];
				codeCounter++;
			}
			break;
		}
		
		LoggedInDiode(loggedIn);
	}
}