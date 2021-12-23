  #define F_CPU 4000000UL        /* using default clock 4MHz*/
  #include <avr/io.h>
  #include <util/delay.h>
  #include <string.h>
  #include <avr/interrupt.h>
  #include "UART_1_AVR128DA64.h"
  #include "ADC_AVR128DA64.h"
  #define channel_3 0x03
  #define START_TOKEN 0x03
  #define END_TOKEN 0xFC
  #include <stdbool.h>
  int bool_flag=false;
//void syringe_dia(void);
  bool syringe_detected = false;
//unsigned long size=0, SYRINGE_Value=0;
void syringe_dia()
{
	float syringe_dia = 0.0;
	float sum=0.0;
	
	for(int i=1;i<=50;i++)
	{
		int adc= ADC0_read(channel_3);
		sum=sum+adc;
	}
	sum=sum/50.00;
	syringe_dia = 0.00676255*(sum) + 5.377882;
	USART1_sendString("Diameter size:");
	USART1_sendFloat(syringe_dia, 2);
	//_delay_ms(100);
}
ISR(PORTC_PORT_vect)
{
	if(PORTC.INTFLAGS & (1 << 7))
	{
		if (syringe_detected)
		{
			syringe_detected = false;
			USART1_sendString("syringe removed");
			  
		}
		else
		{
			syringe_detected = true;
			USART1_sendString("syringe connected\n");
			syringe_dia(); 
		}
		PORTC.INTFLAGS |= (1<<7);
	}
	//bool_flag=true;
	//USART1_sendString("syringe detected");
}
  
void syringe_check_init(void)
{
	if(PORTC.IN & PIN7_bm)
	{
		syringe_detected = false;
	}
	else if (!(PORTC.IN & PIN7_bm))
	{
		syringe_detected = true;
	}
	USART1_sendInt(syringe_detected);
}
 

 
int main(void)
{
	sei();
	USART1_init(9600);
	ADC0_init();
	syringe_check_init();
	PORTC.PIN7CTRL |= ((1 << 3)|(0x01<<0)); 
	while (1)
	{
		//syringe_dia(); 
    }
}