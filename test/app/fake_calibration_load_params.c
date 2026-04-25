#include "calibration_load_params.h"

int is_load_params_called = 0;

int load_calibration_params(void)
{
    is_load_params_called++;
    return 12;
}