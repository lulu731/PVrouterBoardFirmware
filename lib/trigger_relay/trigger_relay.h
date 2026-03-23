
#ifndef TRIGGER_RELAY_H
#define TRIGGER_RELAY_H

#define POWER_THRSHOLD 100

void create_trigger_relay(void);

void trigger_relay(void);

void power_isr(void);

void init_zx_gpio(void);

#endif // TRIGGER_RELAY_H
