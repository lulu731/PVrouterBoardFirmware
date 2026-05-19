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

// RMS measurement registers
extern struct adc_register U_RMS;
extern struct adc_register I_RMS;
extern struct adc_register I_RMS_2;

#endif // ADC_REGISTERS_H
