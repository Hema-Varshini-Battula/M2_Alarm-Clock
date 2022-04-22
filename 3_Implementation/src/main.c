#define F_CPU    4000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned char hours = 0;
volatile unsigned char minutes = 0;
volatile unsigned char seconds = 0;


unsigned char DigitTo7SegEncoder(unsigned char digit, unsigned char common);


int main(void)
{
	DDRB = 0xFF;
	DDRC = 0x3F;
	PORTC = 0xFF;
	
	TCCR1B = (1<<CS12|1<<WGM12);
	OCR1A = 15625-1;
	TIMSK0 = 1<<OCIE1A;
	sei();
	
	
	while(1)
	{
		/* Set Minutes when PORTC Pin 6 Switch is Pressed*/
		if((PINC & 0x40) == 0 )
		{
			_delay_ms(200);
			if(minutes < 59)
			minutes++;
			else
			minutes = 0;
		}
		/* Set Hours when PORTC Pin 7 Switch is Pressed*/
		if((PINC & 0x80) == 0 )
		{
			_delay_ms(200);
			if(hours < 23)
			hours++;
			else
			hours = 0;
		}
		
		PORTB = DigitTo7SegEncoder(seconds%10,1);
		PORTC = ~0x01;
		PORTB = DigitTo7SegEncoder(seconds/10,1);
		PORTC = ~0x02;
		PORTB = DigitTo7SegEncoder(minutes%10,1);
		PORTC = ~0x04;
		PORTB = DigitTo7SegEncoder(minutes/10,1);
		PORTC = ~0x08;
		PORTB = DigitTo7SegEncoder(hours%10,1);
		PORTC = ~0x10;
		PORTB = DigitTo7SegEncoder(hours/10,1);
		PORTC = ~0x20;
		
	}
	return 0;
}


unsigned char DigitTo7SegEncoder(unsigned char digit, unsigned char common)
{
	unsigned char SegVal;
	
	switch(digit)
	{
		case 0:    if(common == 1)    SegVal = 0b00111111;
		else            SegVal = ~0b00111111;
		break;
		case 1:    if(common == 1)    SegVal = 0b00000110;
		else            SegVal = ~0b00000110;
		break;
		case 2:    if(common == 1)    SegVal = 0b01011011;
		else            SegVal = ~0b01011011;
		break;
		case 3:    if(common == 1)    SegVal = 0b01001111;
		else            SegVal = ~0b01001111;
		break;
		case 4:    if(common == 1)    SegVal = 0b01100110;
		else            SegVal = ~0b01100110;
		break;
		case 5:    if(common == 1)    SegVal = 0b01101101;
		else            SegVal = ~0b01101101;
		break;
		case 6:    if(common == 1)    SegVal = 0b01111101;
		else            SegVal = ~0b01111101;
		break;
		case 7:    if(common == 1)    SegVal = 0b00000111;
		else            SegVal = ~0b00000111;
		break;
		case 8:    if(common == 1)    SegVal = 0b01111111;
		else            SegVal = ~0b01111111;
		break;
		case 9:    if(common == 1)    SegVal = 0b01101111;
		else            SegVal = ~0b01101111;
	}
	return SegVal;
}


ISR(TIMER1_COMPA_vect)
{
	seconds++;
	
	if(seconds == 60)
	{
		seconds = 0;
		minutes++;
	}
	if(minutes == 60)
	{
		minutes = 0;
		hours++;
	}
	if(hours > 23)
	hours = 0;
}