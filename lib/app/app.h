#ifndef PVROUTER_APP_H
#define PVROUTER_APP_H

#include <stdbool.h>

bool mount_littlefs_partition(void);
bool init_adc(void);
bool launch_server(void);
void trigger_relay_when_power_below_threshold(const int threshold);

#endif // PVROUTER_APP_H
