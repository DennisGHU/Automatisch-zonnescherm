#include <Arduino.h>
#include <scheduler.h>

#include "led_controller.h"
#include "debug/feedback.h"
#include "USART/USART.h"
#include "sensor/sensors.h"


void setup_program() {

  // Init scheduler
  SCH_Init_T1();
  //100 ongeveer 1 seconde
  
  // Feedback tasks
	SCH_Add_Task(alive_feedback_on, 0, 200);
	SCH_Add_Task(alive_feedback_off, 100, 200);

  // Led controller states
	SCH_Add_Task(led_controller_state, 100, 200);

  // Sensor task
	SCH_Add_Task(sensor_calc_gem, 100, 100); // Calc gem
	SCH_Add_Task(sensor_data_send, 500, 6000); // Send
	SCH_Add_Task(check_sonar, 100, 50); // Check sonar

  // Read
	SCH_Add_Task(USART_check_rx, 100, 1);
  // Start scheduler
  SCH_Start();
}

/*** TODO
 * KLAAR Rollen wanneer temperatuur grens haalt
 * KLAAR Rollen wanneer zon grens haalt
 * KLAAR Bij rollen juiste lichtjes branden
 * KLAAR maximale rol instellen
 * KLAAR minimale rol instellen
 * 
 *** DATA OM TE STUREN
 * KLAAR Status Opgerold, niet opgerold of bezig
 * KLAAR Waarde van de sensor met tag "temp:" of "ligh:"
 * 
 *** DATA OM TE ONTVANGEN VAN PYTHON
 * KLAAR Max waarde rollen
 * KLAAR Min Waarde rollen
 * Forceer uitrollen of inrollen
***/

int main() {

  // Init scripts
  feedback_init();
  USART_init();
  led_controller_init();

  // Init sensors
  init_analog();
  sonar_init();

  // Init scheduled scripts
  setup_program();

  // Loop
  while (1) {
    SCH_Dispatch_Tasks();
  }
  return 2;
}