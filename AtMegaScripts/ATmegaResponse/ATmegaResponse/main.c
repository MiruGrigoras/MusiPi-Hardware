#define F_CPU 11059200UL

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#define BAUD 115200
#define MYUBRR 5

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


int main(void)
{
	char charToReceive, charToSend;
    USART_Init(MYUBRR);
    while (1) 
    {
		charToReceive = USART_Receive();
		if(charToReceive == '?')
			//send 1 if its from room1 to room2 and 2 if its viceversa
		else if(charToReceive == 'o')
				//keep the speakers Open
				
				charToSend = 'k';
			else if(charToReceive == 'c')
					//close the speakers
					charToSend = 'k';
			else charToSend = '!';
		USART_Transmit(charToSend);
    }
}

