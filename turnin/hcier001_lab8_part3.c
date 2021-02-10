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
#include "timer.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;
	
	if(frequency!=current_frequency){
		if(!frequency){TCCR3B&=0X08;}
		else{ TCCR3B |= 0x03;}

		if(frequency<0.954){ OCR3A = 0xFFFF;}
	
		else if(frequency > 31250) {OCR3A = 0x0000;}

		else{ OCR3A = (short) (8000000 / (128 * frequency)) -1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = (1<< COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States { start, init, n1, n2, n3, n4, n5, wait, push, release} state;
unsigned char button;
double freq;

void Tick(){
	switch (state){
		case start:
			state = init;
			break;
		case init:
			if(button ==0x01){
				state = n1;
			}
			else{
				state = init;
			}
			break;
		case n1:
			state = n2;
			break;
		case n2:
			state =n3;
			break;
		case n3:
			state = n4;
			break;
		case n4:
			state = n5;
			break;
		case n5:
			state = wait;
			break;
		case wait:
			if(button == 0x01){
				state = push;
			}
			else {
				state = wait;
			}
			break;
		case push:
			if(button == 0x00){
				state = release;
			}
			else {
				state = push;
			}
			break;
		case release:
			state = init;
			break;
		default:
			state = start;
			break;
		}
	switch(state){
		case start:
			break;
		case init:
			break;
		case n1:
			set_PWM(329.63);
			break;
		case n2:
			set_PWM(349.23);
			break;
		case n3:
			set_PWM(392.00);
			break;
		case n4:
			set_PWM(293.66);
			break;
		case n5:
			set_PWM(523.25);
			break;
		case wait:
			set_PWM(0.0);
			break;
		case push:
			break;
		case release:
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
	PWM_on();
	TimerSet(100);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	button = ~PINA;
	Tick();
	while(!TimerFlag){}
	TimerFlag=0;
    }
    return 1;
}

