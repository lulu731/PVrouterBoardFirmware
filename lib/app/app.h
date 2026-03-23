#ifndef APP_H
#define APP_H

#include <stdbool.h>

bool mount_littlefs_partition(void);
bool init_adc(void);
bool launch_server(void);

#endif // APP_H
