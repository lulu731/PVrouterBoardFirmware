#include "app.h"
#include "calibration.h"
#include "calibration_load_params.h"

#include <stddef.h>
#include <stdint.h>

//static const int POWER_THRESHOLD = 100;

void init_ADC(void)
{
    load_calibration_params();
    exec_metering_calibration();
}
