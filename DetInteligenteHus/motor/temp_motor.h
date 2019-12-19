/*
 * temp_motor.h
 *
 * Created: 18-12-2019 11:35:00
 *  Author: Martin Sonne
 */ 


#ifndef TEMP_MOTOR_H_
#define TEMP_MOTOR_H_

void INIT_TEMP();
void INIT_MOTOR();
void INIT_TEMP_INTERRUPT_TIMER4(unsigned int ms);
int GetTemp();



#endif /* TEMP_MOTOR_H_ */