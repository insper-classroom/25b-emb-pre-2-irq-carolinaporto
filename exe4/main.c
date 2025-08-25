#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_RED = 28;
const int BTN_GREEN = 26;

const int LED_RED = 4;
const int LED_GREEN = 6;

volatile int red_flag = 0;
volatile int green_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    if (gpio == BTN_RED){
      red_flag = 1;
    }
  }
  if (events == 0x8) {
    if (gpio == BTN_GREEN){
      green_flag = 1;
    }
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_RED);
  gpio_set_dir(BTN_RED, GPIO_IN);
  gpio_pull_up(BTN_RED);

  gpio_init(BTN_GREEN);
  gpio_set_dir(BTN_GREEN, GPIO_IN);
  gpio_pull_up(BTN_GREEN);

  gpio_init(LED_RED); 
  gpio_set_dir(LED_RED, GPIO_OUT);

  gpio_init(LED_GREEN);
  gpio_set_dir(LED_GREEN, GPIO_OUT);

  gpio_set_irq_enabled_with_callback(BTN_RED, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(BTN_GREEN, GPIO_IRQ_EDGE_RISE, true);

  int led_red_state = 0;
  int led_green_state = 0;

  while (true) {
    if (red_flag){
      if (led_red_state){
        led_red_state = 0;
      } else {
        led_red_state = 1;
      }
      gpio_put(LED_RED, led_red_state);
      red_flag = 0;
    }

    if (green_flag){
      if (led_green_state){
        led_green_state = 0;
      } else {
        led_green_state = 1;
      }
      gpio_put(LED_GREEN, led_green_state);
      green_flag = 0;
    }
  }
}
