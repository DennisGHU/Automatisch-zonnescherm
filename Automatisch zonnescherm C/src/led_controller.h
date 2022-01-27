#define LED_GRE 8
#define LED_YEL 9
#define LED_RED 10

void led_controller_init();
void led_green_on(bool ans);
void led_yellow_on(bool ans);
void led_red_on(bool ans);

void led_controller_state();


void planeet_status();

int yellowstate;
void led_yellow_switch();