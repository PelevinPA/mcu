#include "pico/stdlib.h"
#include "stdio.h" 
#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"
#include "hardware/gpio.h" 
const uint LED_PIN = 25;


int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while(1){
        char symbol = getchar();
        printf("Символ: %c", symbol);
        printf("ASCII код: %d\n", symbol);
        switch(symbol)
        {
        case 'e':
        	gpio_put(LED_PIN, 1);
        	printf("led enable done\n");
        	break;

        default:
        	break;
        }
        switch(symbol)
        {
        case 'd':
        	gpio_put(LED_PIN, 0);
        	printf("led is off\n");
        	break;

        default:
        	break;
        }
        switch(symbol)
        {
        case 'v':
        	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
        	break;

        default:
        	break;
        }
    }
}
