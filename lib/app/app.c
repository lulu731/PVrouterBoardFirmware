#include "app.h"
#include "calibration.h"

#include <stddef.h>
#include <stdint.h>

static const int POWER_THRESHOLD = 100;

void init_ADC(void)
{
    exec_metering_calibration();
}
