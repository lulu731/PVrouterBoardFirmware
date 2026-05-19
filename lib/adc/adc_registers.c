#include "adc_registers.h"

struct adc_register CAL_START  =
    {.address = 0x20, .data = 0};

struct adc_register PL_CONST_H =
    {.address = 0X21, .data = 0};
struct adc_register PL_CONST_L =
    {.address = 0X22, .data = 0};

struct adc_register MMODE =
    {.address = 0x2b, .data = 0};

struct adc_register CS1        =
    {.address = 0x2C, .data = 0};

struct adc_register SMALL_P_MOD =
    {.address = 0x04, .data = 0};

struct adc_register ADJ_START =
    {.address = 0x30, .data = 0};

struct adc_register P_OFFSET_L =
    {.address = 0x37, .data = 0};
struct adc_register Q_OFFSET_L =
    {.address = 0x38, .data = 0};
struct adc_register P_OFFSET_N =
    {.address = 0x39, .data = 0};
struct adc_register Q_OFFSET_N =
    {.address = 0x3A, .data = 0};

struct adc_register CS2 =
    {.address = 0x3B, .data = 0};

struct adc_register L_GAIN =
    {.address = 0x23, .data = 0};
struct adc_register N_GAIN =
    {.address = 0x25, .data = 0};

struct adc_register L_PHI =
    {.address = 0x24, .data = 0};
struct adc_register N_PHI =
    {.address = 0x26, .data = 0};

struct adc_register P_START_TH =
    {.address = 0x27, .data = 0};
struct adc_register P_NO_L_TH =
    {.address = 0x28, .data = 0};
struct adc_register Q_START_TH =
    {.address = 0x29, .data = 0};
struct adc_register Q_NO_L_TH =
    {.address = 0x2A, .data = 0};

struct adc_register U_GAIN =
    {.address = 0x31, .data = 0};
struct adc_register I_GAIN_L =
    {.address = 0x32, .data = 0};
struct adc_register I_GAIN_N =
    {.address = 0x33,  .data = 0};

struct adc_register U_OFFSET =
    {.address = 0x34, .data = 0};
struct adc_register I_OFFSET_L =
    {.address = 0x35, .data = 0};
struct adc_register I_OFFSET_N =
    {.address = 0x36, .data = 0};


struct adc_register U_RMS =
    {.address = 0x47, .data = 0};
struct adc_register I_RMS =
    {.address = 0x48, .data = 0};
struct adc_register I_RMS_2 =
    {.address = 0x68, .data = 0};
struct adc_register P_MEAN =
    {.address = 0x4A, .data = 0};
struct adc_register Q_MEAN =
    {.address = 0x4B, .data = 0};
struct adc_register P_MEAN_2 =
    {.address = 0x6A, .data = 0};
struct adc_register Q_MEAN_2 =
    {.address = 0x6B, .data = 0};

const adc_data CAL_NEEDED        = 0x5678;
const adc_data CAL_END           = 0x8765;

const adc_data SMALL_P_MOD_ENTER = 0xA987;
const adc_data SMALL_P_MOD_EXIT  = 0xA900;