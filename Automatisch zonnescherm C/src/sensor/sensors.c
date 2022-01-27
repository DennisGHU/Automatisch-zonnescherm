
/*******************************
 * Deze code is deels van
 * https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
 * https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
*******************************/

#include <Arduino.h>

#include "sensors.h"
#include "../USART/USART.h"
#include "../led_controller.h"
#include "../settings.h"


void init_analog(){
    // Analog to digital converter init
	ADMUX = (1<<REFS0)|(1<<ADLAR);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


uint8_t read_analog_to_digital(uint16_t channel)
{
    // Analog to digital converter
	ADMUX &= 0xF0;
	ADMUX |= channel;
	ADCSRA |= (1<<ADSC); //conv
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return ADCH;
}


uint8_t get_temp_inc()
{
    // Read voltage level and convert to celcius
    float raw_value = read_analog_to_digital(PIN_sensor_temp);
    float voltage = ((raw_value*5.0)/256.0);
    float tempC = (voltage-0.5) * 100;
    
    return (uint8_t)tempC;
}

uint8_t get_light()
{
    // Read voltage level and convert to range 0-100%
    float perc = read_analog_to_digital(PIN_sensor_light);
    perc *= 0.390625; // Range 0-100%
    return (uint8_t)perc;
}

void sensor_temp_send(int read){
    // Cast value and send using serial
    // uint8_t read = get_temp_inc();
    char str[10];
    sprintf(str, "%d", read);
    USART_println("temp:");
    USART_println(str);
    USART_println("|");
}

void sensor_light_send(int read){
    // Cast value and send using serial
    // uint8_t read = get_light();
    char str[10];
    sprintf(str, "%d", read);
    USART_println("ligh:");
    USART_println(str);
    USART_println("|");
}

int waarde_now = 0;
uint16_t gem_temp = 0;
uint8_t gem_temp_count = 0;

// Calc sensor data for light or temp
void sensor_calc_gem() {
    
    //USART_send_roll_val(gem_temp_count);
    //USART_send_roll_val(gem_temp);
    // gem over 40 seconds
    if (gem_temp_count>=40){
        return;
    }

     // Light sensor
    if (SENSOR_TYP==0){
        gem_temp += get_light();
        gem_temp_count++;
        return;
    }

     // Temp sensor
    gem_temp += get_temp_inc();
    gem_temp_count++;
}


// Send sensor data for light or temp
void sensor_data_send() {

    // Light sensor
    if (SENSOR_TYP==0){
        waarde_now = (int)(gem_temp/gem_temp_count);
        gem_temp=0;
        gem_temp_count=0;
        sensor_light_send(waarde_now);
        return;
    }

    // Temp sensor
    waarde_now = (uint8_t)(gem_temp/gem_temp_count);
    sensor_temp_send(waarde_now);
    gem_temp=0;
    gem_temp_count=0;
}


void sonar_init(){
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    // Clear trigger pin
    digitalWrite(TRIG_PIN, LOW);
}


void check_sonar()
{
    // Trigger for 10 microsec
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    // Calculate distance in centimer
    float duration = pulseIn(ECHO_PIN, HIGH, 0);
    duration *= 0.034;
    duration /= 2;
    
    new_sensor_distance((uint8_t)duration);

    if (SENSOR_TYP==0){ // light
        if (waarde_now > light_thrash_holder){
            // Uitrollen---------------------------------------
            if (roll_now < roll_max){
                //Yellow blinken en uitrollen rood
                planeet_status(2);
            } else {
                planeet_status(1);
            }

        } else if (waarde_now < light_thrash_holder){
            // Inrollen----------------------------------------
            if (roll_now > roll_min){
                //Yellow blinken en inrollen rood
                planeet_status(4);
            } else {
                planeet_status(3);
            }
        }
        return;
    }

    if (waarde_now > temp_thrash_holder){
            // Uitrollen---------------------------------------
            if (roll_now < roll_max){
                //Yellow blinken en uitrollen rood
                planeet_status(2);
            } else {
                planeet_status(1);
            }

        } else if (waarde_now < temp_thrash_holder){
            // Inrollen----------------------------------------
            if (roll_now > roll_min){
                //Yellow blinken en inrollen rood
                planeet_status(4);
            } else {
                planeet_status(3);
            }
        }

}

int roll_now = 0;

// int sensor_distance = 0; // CM
int sensor_distance_min = 0; // CM
int sensor_distance_max = 0; // CM

void new_sensor_distance(int dis){
    roll_now = dis;
}

void min_sensor_distance(int dis){
    sensor_distance_min = dis;
}

void max_sensor_distance(int dis){
    sensor_distance_max = dis;
}


// cm
int roll_max = 100;
int roll_min = 10;

int roll_go = 100;

void USART_send_roll_val(uint8_t read){
    // Cast value and send using serial
    // uint8_t read = get_temp_inc();
    char str[10];
    sprintf(str, "%d", read);
    USART_println(" ");
    USART_println(str);
}

// MAX roller
void roll_max_up(){
    roll_max++;
    USART_println("roll_max:");
    USART_send_roll_val(roll_max);
    USART_println("|");
}
void roll_max_down(){
    roll_max--;
    USART_println("roll_max:");
    USART_send_roll_val(roll_max);
    USART_println("|");
}

// MIN roller
void roll_min_up(){
    roll_min++;
    USART_println("roll_min:");
    USART_send_roll_val(roll_min);
    USART_println("|");
}
void roll_min_down(){
    roll_min--;
    USART_println("roll_min:");
    USART_send_roll_val(roll_min);
    USART_println("|");
}


int temp_thrash_holder = 0;

// TEMP SETTER
void temp_up(){
    temp_thrash_holder++;
    USART_println("temp_trash:");
    USART_send_roll_val(temp_thrash_holder);
    USART_println("|");
}
void temp_down(){
    temp_thrash_holder--;
    USART_println("temp_trash:");
    USART_send_roll_val(temp_thrash_holder);
    USART_println("|");
}



int light_thrash_holder = 0;

// light SETTER
void light_up(){
    light_thrash_holder++;
    USART_println("light_trash:");
    USART_send_roll_val(light_thrash_holder);
    USART_println("|");
}
void light_down(){
    light_thrash_holder--;
    USART_println("light_trash:");
    USART_send_roll_val(light_thrash_holder);
    USART_println("|");
}