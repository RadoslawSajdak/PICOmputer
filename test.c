
#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

enum
{
    LED_PIN         = 25,
    BUTTON_PIN      = 20,
    ENCODER_B_PIN   = 19,
    ENCODER_A_PIN   = 18,
    DEBOUNCE_MS     = 200,

};

void blink(void)
{
    gpio_put(LED_PIN, 1);
}

void encoder_irq(uint gpio, uint32_t events)
{
    static uint32_t time = 0;
    if ((to_ms_since_boot(get_absolute_time())-time) > DEBOUNCE_MS) 
    {
        time = to_ms_since_boot(get_absolute_time());

        switch (gpio)
        {
        case BUTTON_PIN:
            blink();
            break;

        case ENCODER_A_PIN:
            if (gpio_get(ENCODER_B_PIN))
                puts("->\n");
            else
                puts("<-\n");
            break;

        default:
            break;
        }
    }
}

int main()
{

    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, encoder_irq);

    gpio_init(ENCODER_A_PIN);
    gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
    gpio_set_irq_enabled(ENCODER_A_PIN, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    blink();
    while (1) 
    {
       sleep_ms(500);
       gpio_put(LED_PIN, 0);
    }
}