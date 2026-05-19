#ifndef ADC_RW_H
#define ADC_RW_H

#include "adc_registers.h"

/***********************/
/* READ and WRITE data */
/***********************/

int read_adc_register(struct adc_register* reg);

int write_adc_register(const struct adc_register reg);

//todo: manage errors in writing
/***********************/
/*        END          */
/* READ and WRITE data */
/***********************/


#endif // ADC_RW_H
