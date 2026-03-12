#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "adc-task/adc-task.h"

#define DEVICE_NAME "RP2040 LED Controller"
#define DEVICE_VRSN "1.0.0"

// Callbacks for commands
void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
    printf("LED turned ON\n");
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED turned OFF\n");
}

void led_blink_callback(const char* args)
{
    led_task_state_set(LED_STATE_BLINK);
    printf("LED blinking\n");
}
void led_blink_set_period_ms_callback(const char* args)
{
    unsigned int period_ms = 0;
    // Пытаемся извлечь число из строки аргументов
    int ret = sscanf(args, "%u", &period_ms);
    if (ret != 1 || period_ms == 0) {
        printf("Error: invalid period. Usage: set_period <milliseconds>\n");
        return;
    }
    led_task_set_blink_period_ms(period_ms);
    printf("Blink period set to %u ms\n", period_ms);
}
void get_adc_callback(const char* args)
{
    float voltage_V = get_voltage();
    printf("%f\n", voltage_V);
}

// Device API commands array
api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "turn LED on"},
    {"off", led_off_callback, "turn LED off"},
    {"blink", led_blink_callback, "make LED blink"},
    {"set_period", led_blink_set_period_ms_callback, "set blink period in milliseconds"},
    {"get_adc", get_adc_callback, "get voltage"},
    {NULL, NULL, NULL}
};

int main()
{
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();
    adc_task_init();

    while (true)
    {
        char* command = stdio_task_handle();
        if (command != NULL)
        {
            protocol_task_handle(command);
        }
        led_task_handle();
        sleep_us(100); // небольшая задержка для снижения нагрузки
    }
    return 0;
}