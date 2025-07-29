#ifndef ADC_REGISTERS_H
#define ADC_REGISTERS_H

#include <stdint.h>

typedef uint8_t adc_address;
typedef uint16_t register_value;

struct adc_register
{
    const adc_address address;
    register_value value;
};

#endif // ADC_REGISTERS_H
