#ifndef F_CPU
#define F_CPU 11059200UL
#endif

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#define BAUD 115200
#define MYUBRR 5

typedef enum {true, false} bool;
	
uint8_t lightChannelLeft = 0x00;
uint8_t lightChannelRight = 0x01;
uint16_t oldLightLevelLeft = 0;
uint16_t oldLightLevelRight = 0;
uint8_t oldDirection = 0; // 1 for left -> right; 2 for right -> left

void USART_Init( unsigned int myubrr ){
	//Set baud rate
	UCSRC = 0;
	UBRRH = (unsigned char) (myubrr>>8);
	UBRRL = (unsigned char) myubrr;
	
	//Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);
	
	/* Set frame format: 8 bits data, -no- 2stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive()
{
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);		/* Return the byte */
}

void ADC_Init(){
	DDRA = 0;		//sets ADC port as input, 1 was for output
	ADCSRA = 0;
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
}

uint16_t getLightLevel(uint8_t channel){
	uint8_t adc_hi;
	uint8_t adc_lo;
	uint16_t result = 0;
	//setez canalul de pe care sa citesc
	ADMUX = (1 << REFS0) | (channel & 0x1f);		//compar channel-ul cu 00011111 ca sa ma asigur ca iau bitii corecti din channel
	
	ADCSRA |= (1<<ADSC);			//: ADC start conversion, alternativa la ADSC = 1;
	
	while(ADCSRA & (1<<ADSC));		//ADSC devine 0 la finalul conversiei, desi putem folosi si ca ADIF sa fie 1 (sa se intrerupa conversia)
	
	//_delay_ms(10);
	adc_lo = ADCL;
	adc_hi = ADCH;
	result = ((uint16_t)adc_hi << 8) | adc_lo;
	return(result);
}

char getDirection()
{	
	uint8_t direction;
	bool changeLeft = false, changeRight = false;
	uint16_t lightLevelLeft = 0;
	uint16_t lightLevelRight = 0;
	
	lightLevelLeft = getLightLevel(lightChannelLeft);
	lightLevelRight = getLightLevel(lightChannelRight);
	
	if(abs(lightLevelLeft - oldLightLevelLeft) >= 150 && oldLightLevelLeft != 0)
		changeLeft = true;
	if(abs(lightLevelRight - oldLightLevelRight) >= 150 && oldLightLevelRight != 0)
		changeRight = true;
	
	if(changeLeft == true && changeRight == false){
		direction = 1;
		changeLeft = false;
	}
	else if(changeLeft == false && changeRight == true){
		direction = 2;
		changeRight = false;
	}
	else if(changeLeft == true && changeRight == true){
		direction = 0;
		changeLeft = changeRight = false;
	}
	
	oldLightLevelLeft = lightLevelLeft;
	oldLightLevelRight = lightLevelRight;
	
}

void Speakers_Init()
{
	DDRB = DDRB | (1<<2);
}

int main(void)
{
	char charToReceive, charToSend;
	uint8_t direction, notYetSentDirection = 0;
    USART_Init(MYUBRR);
	ADC_Init();
	Speakers_Init();
    while (1) 
    {
		charToReceive = USART_Receive();
		direction = getDirection();
		if(direction != oldDirection && direction != 0)
		{
			notYetSentDirection = direction;
			oldDirection = direction;
		}
		if(charToReceive == '?'){
			if(notYetSentDirection != 0)
				//send response to Raspi for things have changed
				{
					//send 1 if its from room1 to room2 and 2 if its viceversa
					charToSend = notYetSentDirection;
					notYetSentDirection = 0;
				}
		}
		else if(charToReceive == 'o')
			{
				//keep the speakers Open
				PORTB = PORTB | (1<<2);
				charToSend = 'k';
			}
			else if(charToReceive == 'c')
				{
					//close the speakers
					PORTB = PORTB & (~(1<<2));
					charToSend = 'k';
				}
			else charToSend = '!';
		USART_Transmit(charToSend);
    }
}

