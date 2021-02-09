/*	Author: lab
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;
	
	if(frequency!=current_frequency){
		if(!frequency){TCCR3B&=0X08;}
		else{ TCCRB |= 0x03;}

		if(frequency<0.954){ OCR3A = 0xFFFF;}
	
		else if(frequency > 31250) {OCR3A = 0x0000;}

		else{ OCR3A = (short) (8000000 / (128 * frequency)) -1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWN_on(){
	TCCR3A = (1<< COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWN(0);
}

void PWN_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


enum States { start, init, c4, d4, e4} state;
unsigned char button;
void Tick(){ // transitions
	switch(state){
		case start:
			state = init;
			break;
		case init:
			if(button == 0X01){
				state = c4;
			}
			else if(button == 0x02){
				state = d4;
			}
			else if(button == 0X04){
				state = e4;
			}
			else{
				state = init;
			}
			break;
		case c4:
			if(button == 0x01){
				state = c4;
			}
			else{
				state = init;
			}
			break;
		case d4:
			if(button == 0x02){
				state = d4;
			}
			else{
				state = init;
			}
			break;
		case e4:
			if(button == 0x04){
				state = e4;
			}
			else{
				state = init;
			}
			break;
		default:
			case = start;
			break;
		}
	
	switch(state){ //actions
		case start:
			break;
		case init:
			set_PWN(0);
			break;
		case c4:
			set_PWN(261.63);
			break;
		case d4:
			set_PWN(293.66);
			break;
		case e4:
			set_PWN(329.63);
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xff;
	DDRB = 0xff; PORTB = 0x00;

	button = 0x00;
	state = start;
	PWN_on();
    /* Insert your solution below */
    while (1) {
	button = ~PINA;
	Tick();
    }
    return 1;
}
