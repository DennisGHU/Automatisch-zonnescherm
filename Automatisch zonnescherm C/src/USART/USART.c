#include <Arduino.h>
#include "USART.h"
#include "sensor/sensors.h"
#include "led_controller.h"
#include "../debug/feedback.h"
#include "../settings.h"

/*******************************
 * Deze code is deels uit
 * Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*******************************/

void USART_init(){
    /*Set baud rate */
    UBRR0H = (int)115200;
    UBRR0L = 51;
	UCSR0A = 0; // Disable U2X is bad mmkay
    /*Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 1stop bit */
    // UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    UCSR0C=_BV(UCSZ01)|_BV(UCSZ00);
}

void USART_transmit(unsigned char data){
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void USART_println(char* string)
{
    // Print line char by char
	for (size_t i = 0; i < strlen(string); i++)
	{
		USART_transmit(string[i]);
	}
}

char USART_read()
{
	/* Wait for data to be received */
    //while (!(UCSR0A & (1<<RXC0)));
    
    if (!(UCSR0A & (1<<RXC0))){
        // UDR0;
        return 'x';
    }
    //loop_until_bit_is_set(UCSR0A, RXC0);
    /* Get and return received data from buffer */

    return UDR0;
}

void USART_send_val(uint8_t read){
    // Cast value and send using serial
    // uint8_t read = get_temp_inc();
    char str[10];
    sprintf(str, "%d", read);
    USART_println(" ");
    USART_println(str);
}

void USART_check_rx(){

    // f-z 102-122 10 end char 0 tot 9

    unsigned char res = USART_read();
    if (!strcmp(res, 'x')){
        //uint8_t tmp = UDR0;
        return;
    }

    //USART_send_roll_val(res);

    // Rol max Up
    if ((int)res == 114){ // r
        roll_max_up();
        return;
    }

    // Rol max down
    if ((int)res == 106){ // j
        roll_max_down();
        return;
    }

    // Rol min Up
    if ((int)res == 121){ // y
        roll_min_up();
        return;
    }

    // Rol max down
    if ((int)res == 103){ // g
        roll_min_down();
        return;
    }

    
    // Temp of light up
    if ((int)res == 102){ // f
    //led_green_on(true);
        if (SENSOR_TYP==0){
            light_up();
            return;
        }
        temp_up();
        return;
    }    
    // Temp of light down
    if ((int)res == 111){ // o
        if (SENSOR_TYP==0){
            light_down();
            return;
        }
        temp_down();
        return;
    }

    
}