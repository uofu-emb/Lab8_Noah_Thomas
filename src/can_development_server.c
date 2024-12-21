#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>

#include "FreeRTOSConfig.h"

#include "FreeRTOS.h"

#include <queue.h>
#include <task.h>

static struct can2040 cbus;
static QueueHandle_t messageQueue;
static TaskHandle_t messageProcessorTH, messageSenderTH;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    BaseType_t xHigherPriorityTaskWoken;
    // Add msg to queue
    xQueueSendFromISR(messageQueue,msg,&xHigherPriorityTaskWoken);
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
    QueueHandle_t mesQueue = (QueueHandle_t)args;
    struct can2040_msg msg;
    while(1)
    {
        xQueueReceive(mesQueue, &msg, portMAX_DELAY);
        printf("ID: %u\n", msg.id);
        printf("Data 0: %u\n", msg.data32[0]);
        printf("Data 1: %u\n", msg.data32[1]);
    }
    // check if something on queue
    // if something, print it
    // else go back waiting
}

void task2(void* args)
{
    struct can2040_msg transmitMsg;

    transmitMsg.id = 0x0000000F;
    transmitMsg.dlc = 0x0008;
    transmitMsg.data32[0] = 0x11223344;
    transmitMsg.data32[1] = 0x55667788;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        // printf("Trying to transmit\n");
        int returnValue = can2040_transmit(&cbus, &transmitMsg);
        // printf("Return value %i\n", returnValue);
    }
    // check if something on queue
    // if something, print it
    // else go back waiting
}

int main(void)
{
    // initialization
    stdio_init_all();

    messageQueue = xQueueCreate( 15, sizeof(struct can2040_msg) );

    canbus_setup();

    xTaskCreate(task1, "messageProcessor", configMINIMAL_STACK_SIZE, messageQueue, tskIDLE_PRIORITY+1, &messageProcessorTH);
    xTaskCreate(task2, "messageSender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &messageSenderTH);

    vTaskStartScheduler();

    // Create task listen for something on the queue
    // start scheduler
    return 0;
}