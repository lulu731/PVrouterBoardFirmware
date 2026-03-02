
#ifndef TRIGGER_RELAY_H
#define TRIGGER_RELAY_H

#define POWER_THRSHOLD 100

// TODO: Define pin_zx number according to hardware
#define GPIO_ZX 4

void create_trigger_relay(void);

void trigger_relay(void);

void power_isr(void);

void init_zx_gpio(void);

#endif // TRIGGER_RELAY_H
