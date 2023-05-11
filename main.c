/*
 * Lab6.c
 *
 * Created: 6/14/2022 8:00:42 PM
 * Author : Abdalla
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "avr/iom328pb.h"

const float perentages [4][3] = 
{
	{0.9,0.8,0.7},
	{0.6,0.5,0.4},
	{0.3,0.2,0.1},
	{0,1.5,0}
};

int main(void)
{
	//enable global interrupts
	sei();
	
	//initiate timer1 to generate an interrupt every 3ms to scan keypad
	TCCR1B |= 1<<WGM12;						// CTC mode
	OCR1A = 48000u;							// Count to 3ms
	TIMSK1 |= 1<<OCIE1A;					// enable interrupt for compare match A
	
	// initiate timer0 to generate PWM with a fix 2ms period and varying duty cycle
	TCCR0A |= (1<<WGM00) | (1<<WGM01);		// PWM mode
	TCCR0B |= 1<<WGM02;						// PWM mode
	OCR0A = 124;							// 2ms period of the PWM signal
	OCR0B = 125;							// intialize duty cycle to 0%
	TIMSK0 |= (1<<OCIE0A) | (1<<OCIE0B);	// enable interrupt for compare match A&B
	
	//initialize IO pins for the keypad and LED
	DDRD |= 0xf0;
	PORTD |= 0xf0;
	DDRB |= 0x20;
	PORTB |= 0x0f;	
	
	// enable timers
	TCCR1B |= 1<<CS10;						// 1 prescaler
	TCCR0B |= 1<<CS02;						// 256 prescaler

    while (1) {}
		
}

ISR(TIMER1_COMPA_vect)
{
	for (int i=0; i<4; i++)
	{
		PORTD &= ~(1<<(PORTD4+i));		// turn off one of the row pins
		
		for (int j=0; j<3; j++)
		{
			if (i==3 && (j==0 || j==2))	// skip * and #
			{
				continue;
			}
			
			if (!(PINB & (1<<(PINB0+j))))
			{
				OCR0B = (int) round((124 * perentages[i][j]));	// adjust duty cycle
			}
		}
		
		PORTD |= 1<<(PORTD4+i);
	}
}

ISR (TIMER0_COMPA_vect)
{
	PORTB &= ~(1<<PORTB5);
}

ISR (TIMER0_COMPB_vect)
{
	PORTB |= (1<<PORTB5);
}