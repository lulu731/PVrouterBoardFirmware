#ifndef APP_H
#define APP_H

void mount_littlefs_partition(void);
void init_ADC(void);
void launch_server(void);
void trigger_relay_when_power_below_threshold(const int threshold);

#endif // APP_H
