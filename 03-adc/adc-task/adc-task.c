#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <stdio.h>

static const uint ADC_PIN = 26;
static const uint ADC_KANAL = 0;

void adc_task_init(void){
	adc_init();
	adc_gpio_init(ADC_PIN);
}

float get_voltage(void){
	adc_select_input(ADC_KANAL);
    uint16_t voltage_counts = adc_read();
    float voltage_V = (voltage_counts / 11) * 3.3;
    return voltage_V;
}