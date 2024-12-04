#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#include "task.h"

static struct can2040 cbus;
static TaskHandle_t messageSenderTH;


static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Put your code here....
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

void task1(void* args)
{

}

int main(void)
{
    // initialization
    stdio_init_all();

    canbus_setup();

    struct can2040_msg transmitMsg;

    transmitMsg.id = 0x000F;
    transmitMsg.dlc = 0x0008;
    transmitMsg.data32[0] = 0x11223344;
    transmitMsg.data32[1] = 0x55667788;

    // gpio_init(1);
    // gpio_set_dir(1,true);
    // gpio_put(1, true);
    
    bool output = true;
    while(1)
    {
        sleep_ms(1000);
        printf("Trying to transmit\n");
        // output = ~output;
        // printf("output %u\n", output);
        // gpio_put(5, output);
        int returnValue = can2040_transmit(&cbus, &transmitMsg);
        printf("Return value %i\n", returnValue);
    }

    return 0;
}