/*
 * Zadatak 2.c
 *
 * Created: 11.5.2020. 18:17:30
 * Author : Josip Prpi? 4.H
 */ 

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>
#include<util/delay.h>
void USART_Init (unsigned int ubrr)
{
	//postavljanje baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//omogu?avanje RX i TX
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Namještanje postavki prijenosa - 8b data i 2 stop bita
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void UART_send(unsigned char data)
{
	// ?ekaj dok se buffer ne isprazni
	while(!(UCSR0A & (1 << UDRE0)));

	// spremi podatke
	UDR0 = data;
}
unsigned char USART_Receive(void)
{
	//?ekaj dok ne primi podatke
	while (!(UCSR0A & (1<<RXC0)))
	;
	//vrati podatke iz buffera
	return UDR0;
}
int main(void)
{
	//Arduino naredba: Serial.begin()
	USART_Init(MYUBRR);
	while (1)
	{
		char message[16]= "Parsed Integer: ";
		char c = USART_Receive();
		
		for(int i=0; i<16;i++){
			UART_send(message[i]);
		}
		if(c >= '0' && c<='9') UART_send(c);
		else UART_send('0');
		
		UART_send('\n');
		_delay_ms(5000);
	}
}


