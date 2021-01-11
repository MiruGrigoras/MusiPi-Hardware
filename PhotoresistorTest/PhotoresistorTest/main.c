/*
 * PhotoresistorTest.c
 *
 * Created: 02/01/2021 15:45:27
 * Author : MiruGrigoras
 */ 

#ifndef F_CPU
#define F_CPU 11059200UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int map(int receivedValue, int inMin, int inMax, int outMin, int outMax){
	return (receivedValue - inMin) * (outMax - outMin)/(inMax - inMin) + outMin;
}

void ADCInitialisation(){
	//ADEN = 1: ADC enabled
	//ADPS: setting the division factor for the ADC clock frequency
	//ADMUX CU REFS1:0 = 01 : reference voltage is AVCC (pin 30)  
	
	ADCSRA &= (0<<ADPS0);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
	ADMUX = (1<<REFS0);
}

uint16_t getLightLevel(){
	//ADSC = 1: ADC start conversion
	
	_delay_ms(10);
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC));
	
	_delay_ms(10);
	return(ADC);
}

int main(void)
{
	printf("U ok?");
    ADCInitialisation();
    while (1) 
    {
		uint16_t lightLevel = getLightLevel();
    }
	return 0;
}

