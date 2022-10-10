/*
 * Zadatak 7.c
 *
 * Created: 10.5.2020. 14:27:49
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

char String[]="";
unsigned char data;
uint8_t brightness;

int main(void)
{
	DDRD |= 0b00111100;
	PORTD=0x00;
	
	USART_init(); //inicijalizacija serijske komunikacije
	
	while (1)
	{
		data=USART_receive();
		USART_send(data);
		
		brightness=(data%100 - '0')*100 + (data%10 - '0')*10 + (data -'0');
		
		OCR2B = brightness;

		_delay_ms(10);
		

	}
}

void pwm_init() //funkcija za promjenu svjetine
{
	// initialize timer0 in PWM mode
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	TCCR2B = _BV(WGM22) | _BV(CS20);
}

//funkcije potrebne za UART komunikaciju

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



