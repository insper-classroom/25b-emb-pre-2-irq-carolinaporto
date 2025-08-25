#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN = 28;

volatile int fall_flag = 0;
volatile int rise_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    fall_flag = 1;
  }
  if (events == 0x8) {
    rise_flag = 1;
  }
}

int main() {
    stdio_init_all();
    
    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_up(BTN);
    
    gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    
    uint32_t fall_ms;
    
    while (true) {
        if (fall_flag){
            fall_ms = to_ms_since_boot(get_absolute_time());
            fall_flag = 0;
        }
        else if (rise_flag){
            uint32_t rise_ms = to_ms_since_boot(get_absolute_time());
            
            uint32_t tempo = (rise_ms - fall_ms);

            if (tempo > 800){
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
            rise_flag = 0;
        }
    }
}
