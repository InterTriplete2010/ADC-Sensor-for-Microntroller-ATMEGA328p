#ifndef F_CPU
#define F_CPU 16000000UL	//16 MHz; this is the system clock of ATMEGA328p
#endif

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#ifndef BAUD
#define BAUD 9600
#endif

//#define UBRR0 103	//The UBRR is the Baud Rate register. This is the value for single speed (U2Xn = 0);
#ifndef MYUBRR
#define MYUBRR ((F_CPU/(BAUD*16UL))-1) //The value 16 has been used because we are operating in single speed;
#endif

unsigned int threshold_ADC = 100;	//Thershold for the photoresistor

//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//
//Define the USART
void uart_init(unsigned int ubrr) {
	
	//Set the baud rate. This is a 12 bit register 
	UBRR0H = (unsigned char) (ubrr >> 8);	//It contains the 4 most significant bits
	UBRR0L = (unsigned char)ubrr;	//It contains the 8 least significant bits

	//Enable the transmitter
	UCSR0B |= (1 << TXEN0);
	
	//Set the number of bits to 1
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	

}

//Function for the transmission of the data
void USART_Transmit (uint8_t data)
{
	
	//Check if the transmit buffer is empty and ready to receive new data. This happens when the UCSR01 and the UDRE0 are empty (UDRE0 is set to 1 in the UCSR0A register).
	while(!(UCSR0A & (1<<UDRE0)));
	
	//Put the data into the buffer and send it out;
	UDR0 = data;
	
}

//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//
//Initialize the ADC converter
void init_adc()
{
	//Set the scalar value to 128. Given the fact that the clock is 16 MHz and the scalar value is 128 (ADPS2,1,0 = 111) => 16000000/128 = 125000 Hz.
	//This is the ADC frequency. ADEN enables the ADC converter
	//ADATE = ADC auto trigger enable; ADIE = ADC Interrupt Enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADIE) | (0 << ADATE);
	
	//Internal 1.1V voltage reference with external capacitor at AREF pin
	ADMUX |= (1 << REFS0) | (1 << REFS1);
	
	
}


//Read the data and digitize them
void adc_read(int channel)
{
	//Select the analog channel to be read
	ADMUX |= channel;
	
	//Enable the ADC conversion; ADEN = ADC enable
	ADCSRA |= (1 << ADEN) | (1 << ADSC);
	
	while(ADCSRA & (1 << ADSC));
	
}
//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//
//Interrupt of the ADC
ISR(ADC_vect)
{

	USART_Transmit(ADC);

	if(ADC < threshold_ADC)
	{
		
		PORTD |= (1 << PORTD5);
		PORTB &= ~(1 << PORTB1);
		
	}
	
	else
	{
		
		PORTD &= ~(1 << PORTD5);
		PORTB |= (1 << PORTB1);
		
	}
	
	adc_read(1);
	
}
//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//


int main(void)
{
	DDRB |= (1 << DDB1);	//Set port PB1 as output
	DDRD |= (1 << DDD5);	//Set the port PD5 as output
	
	uart_init(MYUBRR);	//Initialize the USART
	
	init_adc();	//Initialize the ADC converter
	adc_read(1);
	
	sei();	//Enable interrupts
	
	while (1);
	

}
