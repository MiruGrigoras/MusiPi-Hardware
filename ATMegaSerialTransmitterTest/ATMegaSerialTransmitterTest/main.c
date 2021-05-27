#define F_CPU 11059200UL

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>



#define BAUD 115200
#define MYUBRR F_CPU/16/BAUD-1
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

int main(void)
{
	char c = 'a';
	USART_Init(MYUBRR);
    /* Replace with your application code */
    while (1) 
    {
		USART_Transmit(c);
    }
}

