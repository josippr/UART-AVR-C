/*
 * Zadatak 10.c
 *
 * Created: 11.5.2020. 18:31:05
 * Author : Josip Prpi? 4.H
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//Deklaracija funkcija
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
int led_state=0;
int main(void){
	DDRD |= 0b00000100;  //pinMode(2, OUTPUT);
	DDRD &= 0b01111111;  //pinMode(7, INPUT_PULLUP), Postavljanje registra DDR u 0 za      pin 7;
	PORTD |= 0b11000000; // Postavljanje registra PORTD u 1 za pin 7
	USART_init();
	while(1){
		if ((PIND & 0b10000000) >> 7 == 0){
			PORTD |= 0b00000100;
			led_state=1; //ako je varijabla led_state u 1, zna?i da je LED dioda upaljena
		}
		if(led_state==1){
			if ((PIND & 0b10000000) >> 7 == 0)
			{
				PORTD &= 0b01111111;
				led_state=0; //ako je varijabla led_state u 0, zna?i da je LED dioda ugašena
			}
		}
	}
	return 0;
}

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


