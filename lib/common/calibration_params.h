#ifndef CALIBRATION_PARAMS_H
#define CALIBRATION_PARAMS_H

#include <stdint.h>

extern uint16_t Mc, Un, Ib, Gl, Vl, Vu,
                //mmode register params
                Lgain, Ngain, LNsel, DisHPF, Amod,
                Rmod, Zxcon, Pthresh;


extern const char* string_keys_array[];
extern uint16_t* keys_array[];

extern const int keys_count;

#endif // CALIBRATION_PARAMS_H
