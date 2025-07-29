#ifndef ADC_REGISTERS_H
#define ADC_REGISTERS_H

#include <stdint.h>

typedef uint8_t adc_address;
typedef uint16_t adc_data;

struct adc_register
{
    const adc_address address;
    adc_data data;
};

#endif // ADC_REGISTERS_H
