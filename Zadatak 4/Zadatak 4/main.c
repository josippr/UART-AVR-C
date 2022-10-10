/*
 * Zadatak 4.c
 *
 * Created: 3.5.2020. 18:19:21
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
	//pri?ekaj dok ne primi podatke
	while (!(UCSR0A & (1<<RXC0)))
	;
	//vrati podatke iz buffera
	return UDR0;
}
int main(void)
{
	//Arduino naredba: Serial.begin()
	USART_Init(MYUBRR);
	char initMessage[40] = "Upisi znak u polje iznad i pritisni SEND";
	for (int i=0; i<40; i++)
	{
		UART_send(initMessage[i]);
	}
	UART_send('\n');
	while (1)
	{
		char c = USART_Receive();
		int val = c;
		char str[2];
		sprintf(str,"%d",val);
		
		if(str[0]!='1' || str[1]!='0')
		{
			char txt[14] = "Upisani znak: ";
			char txt2[20] = ", ASCII vrijednost: ";
			for (int i=0; i<14; i++)
			{
				UART_send(txt[i]);
			}
			UART_send(c);
			for (int i=0; i<20; i++)
			{
				UART_send(txt2[i]);
			}
			UART_send(str[0]);
			UART_send(str[1]);
			UART_send('\n');
		}
	}
}


