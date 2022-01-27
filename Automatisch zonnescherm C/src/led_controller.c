#include <Arduino.h>
#include "led_controller.h"
#include "sensor/sensors.h"

void led_controller_init(){

    // Set pins as output
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YEL, OUTPUT);
    pinMode(LED_GRE, OUTPUT);

    // Set voltages to low
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YEL, LOW);
    digitalWrite(LED_GRE, LOW);
}

void led_green_on(bool ans){
    // Turn led on or off
    if (ans == true){
        digitalWrite(LED_GRE, HIGH);
    } else {
        digitalWrite(LED_GRE, LOW);
    }
}

void led_yellow_on(bool ans){
    // Turn led on or off
    if (ans == true){
        digitalWrite(LED_YEL, HIGH);
    } else {
        digitalWrite(LED_YEL, LOW);
    }
}

int yellowstate = 1;

void led_yellow_switch(){
    yellowstate++;
    if (yellowstate >= 3) {
        yellowstate = 1;
    }
    if (yellowstate <= 1){
        led_yellow_on(true);
    } else {
        led_yellow_on(false);
    }
    
}


void led_red_on(bool ans){
    // Turn led on or off
    if (ans == true){
        digitalWrite(LED_RED, HIGH);
    } else {
        digitalWrite(LED_RED, LOW);
    }
}

void led_controller_state() {
    // Debug troepie force all leds
    // led_green_on(false);
    // led_yellow_on(false);
    // led_red_on(false);
}


void planeet_status(int inrollen){

    switch (inrollen)
    {
    case 1:
        //uitgerold
        led_green_on(true);
        led_yellow_on(false);
        led_red_on(false);
        break;

    case 2:
        //uitrollen bezig
        led_green_on(true);
        led_yellow_switch();
        led_red_on(false);
        break;
        
    case 3:
        //ingerold
        led_green_on(false);
        led_yellow_on(false);
        led_red_on(true);
        break;
        
    case 4:
        //ingerold bezig
        led_green_on(false);
        led_yellow_switch();
        led_red_on(true);
        break;
    
    default:
        break;
    }

}