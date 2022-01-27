#define PIN_sensor_light 0
#define PIN_sensor_temp 1

#define ECHO_PIN 11
#define TRIG_PIN 12

void init_analog();
void sonar_init();

uint8_t read_analog_to_digital(uint16_t channel);
uint8_t get_temp_inc();
uint8_t get_light();

void sensor_temp_send(int read);
void sensor_light_send(int read);


uint16_t gem_temp;
uint8_t gem_temp_count;

void sensor_calc_gem();
void sensor_data_send();

void check_sonar();

// cm
int roll_max;
int roll_min;

int roll_now;
int roll_go;

void USART_send_roll_val(uint8_t read);
// up
void roll_max_up();
void roll_max_down();

// down
void roll_min_up();
void roll_min_down();


int waarde_now;

int temp_thrash_holder;

// TEMP SETTER
void temp_up();
void temp_down();


int light_thrash_holder;

// light SETTER
void light_up();
void light_down();




int sensor_distance; // CM
int sensor_distance_min; // CM
int sensor_distance_max; // CM

void new_sensor_distance(int dis);
void min_sensor_distance(int dis);
void max_sensor_distance(int dis);