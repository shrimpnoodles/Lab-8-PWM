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

enum States { start, init, startPoint, up, waitUp, down, waitDown, off, waitOff} state;
unsigned char button;
double freq;
void Tick(){ // transitions
	switch(state){
		case start:
			state = init;
			break;
		case init:
			state = startPoint;
			break;
		case startPoint:
			if(button == 0x02){
				state = up;
			}
			else if(button == 0x04){
				state = down;
			}
			else if(button == 0x01){
				state = off;
			}
			else{
				state = startPoint;
			}
			break;
		case up:
			state = waitUp;
			break;
		case waitUp:
			if(button == 0x00){
				state = startPoint;
			}
			else{
				state = waitUp;
			}
			break;
		case down:
			state = waitDown;
			break;
		case waitDown:
			if(button == 0x00){
				state = startPoint;
			}
			else{
				state = waitDown;
			}
			break;
		case off:
			if(button == 0x00){
				state = waitOff;
			}
			else{
				state = off;
			}
			break;
		case waitOff:
			if(button == 0x01){
				state = startPoint;
			}
			else{
				state = waitOff;
			}
			break;
		default:
			state = start;
			break;
		}
	
	switch(state){ //actions
		case start:
			break;
		case init:
			set_PWM(261.63);
			freq = 261.63;
			break;
		case startPoint:
			break;
		case up:
			if(freq == 261.63){
				set_PWM(293.66);
				freq = 293.66;
				break;
			}
			else if(freq == 293.66){
				set_PWM(329.63);
				freq = 329.63;
				break;
			}
			else if(freq == 329.63){
				set_PWM(349.23);
				freq = 349.23;
				break;
			}
			else if(freq ==349.23){
				set_PWM(392.00);
				freq = 392.00;
				break;
			}
			else if(freq == 392.00){
				set_PWM(440.00);
				freq = 440.00;
				break;
			}
			else if(freq == 440.00){
				set_PWM(493.88);
				freq = 493.88;
				break;
			}
			else if(freq == 493.88){
				set_PWM(523.25);
				freq = 523.25;
				break;
			}
			break;
		case waitUp:
			break;
		case down:
			 if(freq == 293.66){
				set_PWM(261.63);
				freq = 261.63;
			}
			else  if(freq == 329.63){
				set_PWM(293.66);
				freq = 293.66;
				break;
			}
			else if(freq == 349.23){
				set_PWM(329.63);
				freq = 329.63;
				break;
			}
			else if(freq ==392.00){
				set_PWM(349.23);
				freq = 349.23;
				break;
			}
			else if(freq == 440.00){
				set_PWM(392.00);
				freq = 392.00;
				break;
			}
			else if(freq == 493.88){
				set_PWM(440.00);
				freq = 440.00;
				break;
			}
			else if(freq == 523.25){
				set_PWM(493.88);
				freq = 493.88;
				break;
			}
			break;
		case waitDown:
			break;
		case off:
			set_PWM(0);
			break;
		case waitOff:
			break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xff;
	DDRB = 0xff; PORTB = 0x00;
	
	freq = 0.0;
	button = 0x00;
	state = start;
	PWM_on();
    /* Insert your solution below */
    while (1) {
	button = ~PINA;
	Tick();
    }
    return 1;
}

