#include "adc-task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Пины и каналы
#define ADC_VOLTAGE_GPIO   26   // GPIO26 соответствует каналу 0
#define ADC_VOLTAGE_CHANNEL 0
#define ADC_TEMP_CHANNEL    4    // встроенный датчик температуры

// Период телеметрии (мкс)
#define ADC_TASK_MEAS_PERIOD_US 100000  // 100 мс

// Состояние телеметрии
static adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
static uint64_t next_measure_us = 0;

// Инициализация АЦП
void adc_task_init() {
    adc_init();
    adc_gpio_init(ADC_VOLTAGE_GPIO);
    adc_set_temp_sensor_enabled(true);   // включаем датчик температуры
}

// Однократное измерение напряжения (В)
float adc_task_get_voltage() {
    adc_select_input(ADC_VOLTAGE_CHANNEL);
    uint16_t raw = adc_read();            // 12 бит (0..4095)
    const float conversion = 3.3f / 4095.0f;
    return raw * conversion;
}

// Однократное измерение температуры (°C)
float adc_task_get_temperature() {
    adc_select_input(ADC_TEMP_CHANNEL);
    uint16_t raw = adc_read();
    const float conversion = 3.3f / 4095.0f;
    float voltage = raw * conversion;
    // Формула из datasheet RP2040
    float temp_c = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temp_c;
}

// Установка состояния телеметрии
void adc_task_set_state(adc_task_state_t state) {
    adc_state = state;
    // Сбрасываем таймер при переходе в RUN, чтобы первое измерение произошло сразу
    if (state == ADC_TASK_STATE_RUN) {
        next_measure_us = time_us_64();
    }
}

// Обработчик телеметрии (вызывается в главном цикле)
void adc_task_handle() {
    if (adc_state != ADC_TASK_STATE_RUN) {
        return;
    }

    uint64_t now = time_us_64();
    if (now >= next_measure_us) {
        // Измеряем напряжение и температуру
        float voltage = adc_task_get_voltage();
        float temp = adc_task_get_temperature();

        // Выводим в формате "voltage temp\n" (как требуется для Python)
        printf("%f %f\n", voltage, temp);

        // Планируем следующее измерение
        next_measure_us = now + ADC_TASK_MEAS_PERIOD_US;
    }
}