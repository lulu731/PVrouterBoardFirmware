#include "calibration_params.h"

uint16_t Mc, Un, Ib, Gl, Vl, Vu,
         Ugain, IgainL, IgainN,
         //PoffsetL, QoffsetL, PoffsetN, QoffsetN, Uoffset, IoffsetL, IoffsetN,
         //mmode register params
         Lgain, Ngain, LNsel, DisHPF, Amod,
         Rmod, Zxcon, Pthresh;

const char* string_keys_array[] = {
    "Mc", "Un", "Ib", "Gl", "Vl", "Vu",
    "Ugain", "IgainL", "IgainN",
    //"PoffsetL", "QoffsetL", "PoffsetN", "QoffsetN", "Uoffset", "IoffsetL", "IoffsetN",
    "Lgain", "Ngain", "LNsel", "DisHPF", "Amod",
    "Rmod", "Zxcon", "Pthresh"
};

uint16_t* keys_array[] = {
    &Mc, &Un, &Ib, &Gl, &Vl, &Vu,
    &Ugain, &IgainL, &IgainN,
    //&PoffsetL, &QoffsetL, &PoffsetN, &QoffsetN, &Uoffset, &IoffsetL, &IoffsetN,
    &Lgain, &Ngain, &LNsel, &DisHPF, &Amod,
    &Rmod, &Zxcon, &Pthresh
};

const int keys_count = sizeof(keys_array) / sizeof(keys_array[0]);