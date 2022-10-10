/*
 * Zadatak 5.c
 *
 * Created: 5.5.2020. 22:21:21
 * Author : Josip Prpi? 4.H
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//deklaracije funkcija
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

char String[]="";
unsigned char data;
int main(void){
	DDRD |= 0b00000100;
	PORTD=0x00;
	USART_init();
	while(1){
		data=USART_receive();
		USART_send(data);
		if (data=='1')
		{
			PORTD|=(1<<PORTD2);
			_delay_ms(300);
		}
		else
		{
			PORTD&=~(1<<PORTD2);
		}
	}
	return 0;
}

// Deklaracija dodatnih funckija

void USART_init(void){

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); // Baud Rate Register upper 4 bits
	UBRR0L = (uint8_t)(BAUD_PRESCALLER); // Baud Rate Register lower 4 bits
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00); //Set frame
}
unsigned char USART_receive(void){

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
void USART_send( unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;

}

void USART_putstring(char* StringPtr){

	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}

}



