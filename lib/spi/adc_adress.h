#ifndef ADC_ADRESS_H
#define ADC_ADRESS_H

#include <stdint.h>

typedef uint8_t adc_address;

extern const adc_address CAL_START;

extern const adc_address PL_CONST_H;
extern const adc_address PL_CONST_L;

extern const adc_address MMODE;

extern const adc_address CS1;

extern const adc_address SMALL_P_MOD;
extern const adc_address P_OFFSET_L;
extern const adc_address Q_OFFSET_L;
extern const adc_address P_OFFSET_N;
extern const adc_address Q_OFFSET_N;
extern const adc_address CS2;

extern const adc_address L_GAIN;
extern const adc_address N_GAIN;
extern const adc_address L_PHI;
extern const adc_address N_PHI;

extern const adc_address P_START_TH;
extern const adc_address P_NO_L_TH;
extern const adc_address Q_START_TH;
extern const adc_address Q_NO_L_TH;


extern const adc_address ADJ_START;

extern const adc_address U_GAIN;
extern const adc_address I_GAIN_L;
extern const adc_address I_GAIN_N;

extern const adc_address U_OFFSET;
extern const adc_address I_OFFSET_L;
extern const adc_address I_OFFSET_N;

extern const adc_address U_RMS;
extern const adc_address I_RMS;
extern const adc_address I_RMS_2;
extern const adc_address P_MEAN;
extern const adc_address Q_MEAN;
extern const adc_address P_MEAN_2;
extern const adc_address Q_MEAN_2;

#endif // ADC_ADRESS_H
