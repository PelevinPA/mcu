#pragma once

#include <stdint.h>
#include <stdbool.h>

// Состояния для телеметрии
typedef enum {
    ADC_TASK_STATE_IDLE = 0,
    ADC_TASK_STATE_RUN = 1,
} adc_task_state_t;

// Инициализация АЦП (GPIO26, канал 0, датчик температуры)
void adc_task_init();

// Однократное измерение напряжения на GPIO26 (В)
float adc_task_get_voltage();

// Однократное измерение температуры кристалла (°C)
float adc_task_get_temperature();

// Установка состояния телеметрии
void adc_task_set_state(adc_task_state_t state);

// Обработчик телеметрии (вызывать в основном цикле)
void adc_task_handle();