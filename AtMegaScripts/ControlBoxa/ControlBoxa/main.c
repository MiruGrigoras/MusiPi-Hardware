/*
 * ControlBoxa.c
 *
 * Created: 10/06/2021 22:35:16
 * Author : mgrigora
 */ 

#ifndef F_CPU
#define F_CPU 11059200UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


int main(void)
{
    DDRB = DDRB | (1<<2);
    while (1) 
    {
		PORTB = PORTB | (1<<2);		/* Making PD3 high. This will make LED ON */
		_delay_ms(1000);
		PORTB = PORTB & (~(1<<2));	/* Making PD3 low. This will make LED OFF */
		_delay_ms(1000);
    }
	
	return 0;
}

