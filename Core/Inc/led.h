
#include <stdint.h>

#define LED_INTERVAL 100 //micro sec?

typedef enum _led{
    can,
    green,
    yellow,
    red
}led;



void led_on(led name);

//Turns off when there is an LED that stays on for a long time.
void led_process(void);
