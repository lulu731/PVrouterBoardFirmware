#include "adc_registers.h"

const adc_register CAL_START  =
    {.adress = 0x20, .value = 0};

const adc_register PL_CONST_H =
    {.adress = 0X21, .value = 0};
const adc_register PL_CONST_L =
    {.adress = 0X22, .value = 0};

const adc_register MMODE =
    {.adress = 0x2b, .value = 0};

const adc_register CS1        =
    {.adress = 0x2C, .value = 0};

const adc_register SMALL_P_MOD =
    {.adress = 0x04, .value = 0};

const adc_register ADJ_START =
    {.adress = 0x30, .value = 0};

const adc_register P_OFFSET_L =
    {.adress = 0x37, .value = 0};
const adc_register Q_OFFSET_L =
    {.adress = 0x38, .value = 0};
const adc_register P_OFFSET_N =
    {.adress = 0x39, .value = 0};
const adc_register Q_OFFSET_N =
    {.adress = 0x3A, .value = 0};

const adc_register CS2 =
    {.adress = 0x3B, .value = 0};

const adc_register L_GAIN =
    {.adress = 0x23, .value = 0};
const adc_register N_GAIN =
    {.adress = 0x25, .value = 0};

const adc_register L_PHI =
    {.adress = 0x24, .value = 0};
const adc_register N_PHI =
    {.adress = 0x26, .value = 0};
const adc_register P_START_TH =
    {.adress = 0x27, .value = 0};
const adc_register P_NO_L_TH =
    {.adress = 0x28, .value = 0};
const adc_register Q_START_TH =
    {.adress = 0x29, .value = 0};
const adc_register Q_NO_L_TH =
    {.adress = 0x2A, .value = 0};

const adc_register U_GAIN =
    {.adress = 0x31, .value = 0};
const adc_register I_GAIN_L =
    {.adress = 0x32, .value = 0};
const adc_register I_GAIN_N =
    {.adress = 0x33,  .value = 0};

const adc_register U_OFFSET =
    {.adress = 0x34, .value = 0};
const adc_register I_OFFSET_L =
    {.adress = 0x35, .value = 0};
const adc_register I_OFFSET_N =
    {.adress = 0x36, .value = 0};


const adc_register U_RMS =
    {.adress = 0x47, .value = 0};
const adc_register I_RMS =
    {.adress = 0x48, .value = 0};
const adc_register I_RMS_2 =
    {.adress = 0x68, .value = 0};
const adc_register P_MEAN =
    {.adress = 0x4A, .value = 0};
const adc_register Q_MEAN =
    {.adress = 0x4B, .value = 0};
const adc_register P_MEAN_2 =
    {.adress = 0x6A, .value = 0};
const adc_register Q_MEAN_2 =
    {.adress = 0x6B, .value = 0};