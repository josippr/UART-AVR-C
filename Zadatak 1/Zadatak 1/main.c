/*
 * Zadatak 1.c
 *
 * Created: 1.5.2020. 18:15:37
 * Author : Josip Prpi? 4.H
 */ 

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>
//Ekvivalenta Arduino funkcije Serial.begin()
void UART_Begin(unsigned int ubrr)
{
	//Postavljanje BAUD rate-a
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	//Omogucavanje RX i TX
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//Namještanje postavki prijenosa - 8b data i 2 stop bita
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

//Postavljanje podataka u TX registar mikroupravlja?a
void UART_Send(unsigned char data)
{
	//?ekanje pražnjenja TX buffera
	while(!(UCSR0A & (1 << UDRE0)));
	//Punjenje TX registra
	UDR0 = data;
}
//Citanje podataka iz RX buffera
unsigned char UART_Receive(void)
{
	//?ekanje punjenja RX buffera
	while(!(UCSR0A & (1<<RXC0)));
	//Dohva?anje podataka iz buffera i vra?anje istih
	return UDR0;
}







int main(void)
{
	UART_Begin(MYUBRR);
	
	//Izgled poruke koja se prikazuje pri pokretanju programa
	char startupMsg[40] = "Unesi znak u polje iznad i pritisni SEND";
	//Slanje poruke
	for (int i=0; i<sizeof(startupMsg); i++)
	{
		UART_Send(startupMsg[i]);
	}
	UART_Send('\n');
	
	char buffer[64];
	int location=0;
	int bufferFull=0;
	//Pocetak izvodenja "loop funkcije"
	while (1)
	{
		//Prihvacanje znaka u buffer
		buffer[location] = UART_Receive();
		location++;
		if(location>63)
		{
			location=0;
			bufferFull=1;
		}
		
		char sizeString[2];
		//Ispis
		if(bufferFull)
		{
			sprintf(sizeString, "%d",63);
		}
		else
		{
			sprintf(sizeString, "%d",location);
		}
		
		char message[18] = "Serial.available()";
		for (int i=0; i<sizeof(message); i++)
		{
			UART_Send(message[i]);
		}
		UART_Send('\t');
		for (int i=0; i<sizeof(sizeString);i++)
		{
			UART_Send(sizeString[i]);
		}
		UART_Send('\n');
	}
}


