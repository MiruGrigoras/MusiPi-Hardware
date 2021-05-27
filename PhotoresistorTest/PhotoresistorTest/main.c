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

//https://www.electronicwings.com/avr-atmega/atmega1632-adc

int map(int receivedValue, int inMin, int inMax, int outMin, int outMax){
	return (receivedValue - inMin) * (outMax - outMin)/(inMax - inMin) + outMin;
}

void ADCInitialisation(){
	DDRA = 0;		//sets ADC port as input, 0 was for output
	
	//urmatoarele doua linii inlocuiesc ADCSRA = 10000110 = 0x86; 
	//ADEN = 1;		: ADC enabled
	//ADPS2:0 = 110;	: setting the division factor for the ADC clock frequency
	
	//REFS1:0 = 01;	ADMUX CU REFS1:0 = 01 : reference voltage is AVCC (pin 30)  
	
	//alternative fancy la ce am scris in ultimele 3 randuri de mai sus:
	
	ADCSRA &= (0<<ADPS0);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
	ADMUX &= (0<<REFS1);
	ADMUX |= (1<<REFS0);
}

uint16_t getLightLevel(uint8_t channel){
	//setez canalul de pe care sa citesc
	ADMUX |= (channel & 0x1f);		//compar channel-ul cu 00011111 ca sa ma asigur ca iau bitii corecti din channel
						
	ADCSRA |= (1<<ADSC);			//: ADC start conversion, alternativa la ADSC = 1;
	
	while(ADCSRA & (1<<ADSC));		//ADSC devine 0 la finalul conversiei, desi putem folosi si ca ADIF sa fie 1 (sa se intrerupa conversia)
	
	_delay_ms(10);
	return(ADC);
}

int main(void)
{
	printf("U ok?");
    ADCInitialisation();
    while (1) 
    {
		uint8_t channelLeft = 0x00;
		uint8_t channelRight = 0x01;
		
		uint16_t lightLevelLeft = getLightLevel(channelLeft);
		uint16_t lightLevelRight = getLightLevel(channelRight);
		
		printf("Left: %d   -   Right: %d\n", lightLevelLeft, lightLevelRight);
    }
	return 0;
}

