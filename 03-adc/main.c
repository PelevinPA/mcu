#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "adc-task/adc-task.h"           // <-- новый модуль

#define DEVICE_NAME "RP2040 ADC Meter"
#define DEVICE_VRSN "1.0.0"

// Прототипы callback-функций (все старые плюс новые)
void version_callback(const char* args);
void led_on_callback(const char* args);
void led_off_callback(const char* args);
void led_blink_callback(const char* args);
void help_callback(const char* args);
void led_blink_set_period_ms_callback(const char* args);
void mem_callback(const char* args);
void wmem_callback(const char* args);

// Новые callback'и для АЦП
void get_adc_callback(const char* args);
void get_temp_callback(const char* args);
void tm_start_callback(const char* args);
void tm_stop_callback(const char* args);

// Массив команд API
static api_t device_api[] = {
    {"version", version_callback, "get device name and firmware version"},
    {"on",      led_on_callback,  "turn LED on"},
    {"off",     led_off_callback, "turn LED off"},
    {"blink",   led_blink_callback, "make LED blink"},
    {"help",    help_callback,    "show this help"},
    {"set_period", led_blink_set_period_ms_callback, "set blink period in milliseconds"},
    {"mem",     mem_callback,     "read memory at address (hex): mem <addr>"},
    {"wmem",    wmem_callback,    "write memory at address (hex): wmem <addr> <value>"},
    {"get_adc", get_adc_callback, "measure voltage on GPIO26 (V)"},
    {"get_temp", get_temp_callback, "measure RP2040 temperature (C)"},
    {"tm_start", tm_start_callback, "start telemetry (continuous V and temp)"},
    {"tm_stop",  tm_stop_callback,  "stop telemetry"},
    {NULL, NULL, NULL}
};

// ---------- Callback-функции (старые, без изменений) ----------
void version_callback(const char* args) {
    (void)args;
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args) {
    (void)args;
    led_task_state_set(LED_STATE_ON);
    printf("LED ON\n");
}

void led_off_callback(const char* args) {
    (void)args;
    led_task_state_set(LED_STATE_OFF);
    printf("LED OFF\n");
}

void led_blink_callback(const char* args) {
    (void)args;
    led_task_state_set(LED_STATE_BLINK);
    printf("LED BLINK\n");
}

void help_callback(const char* args) {
    (void)args;
    printf("Available commands:\n");
    for (int i = 0; device_api[i].command_name != NULL; i++) {
        printf("  %s - %s\n", device_api[i].command_name, device_api[i].command_help);
    }
}

void led_blink_set_period_ms_callback(const char* args) {
    uint32_t period_ms = 0;
    if (sscanf(args, "%u", &period_ms) == 1 && period_ms > 0) {
        led_task_set_blink_period_ms(period_ms);
        printf("Blink period set to %u ms\n", period_ms);
    } else {
        printf("Error: invalid period. Usage: set_period <milliseconds>\n");
    }
}

void mem_callback(const char* args) {
    uint32_t addr;
    if (sscanf(args, "%x", &addr) == 1) {
        uint32_t value = *(volatile uint32_t*)addr;
        printf("Value at 0x%08x: 0x%08x\n", addr, value);
    } else {
        printf("Error: invalid address. Usage: mem <hex-addr>\n");
    }
}

void wmem_callback(const char* args) {
    uint32_t addr, value;
    if (sscanf(args, "%x %x", &addr, &value) == 2) {
        *(volatile uint32_t*)addr = value;
        printf("Written 0x%08x to address 0x%08x\n", value, addr);
    } else {
        printf("Error: invalid arguments. Usage: wmem <hex-addr> <hex-value>\n");
    }
}

// ---------- Новые callback'и для АЦП ----------
void get_adc_callback(const char* args) {
    (void)args;
    float voltage = adc_task_get_voltage();
    printf("%f\n", voltage);   // только число, для Python
}

void get_temp_callback(const char* args) {
    (void)args;
    float temp = adc_task_get_temperature();
    printf("%f\n", temp);       // только число
}

void tm_start_callback(const char* args) {
    (void)args;
    adc_task_set_state(ADC_TASK_STATE_RUN);
    printf("Telemetry started\n");
}

void tm_stop_callback(const char* args) {
    (void)args;
    adc_task_set_state(ADC_TASK_STATE_IDLE);
    printf("Telemetry stopped\n");
}

// ---------- main ----------
int main() {
    stdio_init_all();
    stdio_task_init();
    led_task_init();
    adc_task_init();                     // <-- инициализация АЦП
    protocol_task_init(device_api);

    while (1) {
        led_task_handler();
        adc_task_handle();                // <-- обработчик телеметрии
        char* cmd = stdio_task_handle();
        if (cmd) {
            protocol_task_handle(cmd);
        }
        // Нет задержек – неблокирующий цикл
    }
    return 0;
}