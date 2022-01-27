#include <Arduino.h>
#include "feedback.h"

void feedback_init(){
  // Setup feedback pinmodes
  pinMode(LED_BUILTIN, OUTPUT); // Alive feedback
  pinMode(LED_BUILTIN, OUTPUT);
}


// This script triggers pin 13 (orange led) on and off every second

void alive_feedback_on() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void alive_feedback_off() {
  digitalWrite(LED_BUILTIN, LOW);
}
