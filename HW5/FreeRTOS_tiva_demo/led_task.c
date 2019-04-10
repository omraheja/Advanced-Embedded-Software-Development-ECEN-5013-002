/*@FileName : led_task.c
 *@Author   : Om Raheja
 *@Date     : 10th April 2019
 *@brief    : This task toggles LEDs D1 and D2 on the TIVA
 *          : board at a frequency of 10Hz. This task also
 *          : passes a structure containing a time stamp,
 *          : toggle count and author name to the logger
 *          : task using a FreeRTOS IPC mechanism.
 */


/* User Defined Header Files */
#include "led_task.h"
#include "helper.h"


/* Exported Global Variables for Queue */
extern QueueHandle_t xQueue;


/* LED Task */
void led_task(void *pvParameters)
{
    QueueData_t led_data;
    uint32_t number_of_ticks;
    led_data.led_count = 0;
    while(1)
    {
        /* Turn On LEDs */
        D1_LED_ON();
        D2_LED_ON();

        /* Create Delay */
        vTaskDelay(50/portTICK_PERIOD_MS);

        /* Turn Off LEDs */
        D1_LED_OFF();
        D2_LED_OFF();

        /* Create Delay */
        vTaskDelay(50/portTICK_PERIOD_MS);

        /* Increment LED count */
        (led_data.led_count)++;

        /* Store Name to be passed in structure */
        strcpy(led_data.myname,"Om Raheja");

        strcpy(led_data.task,"led");

        /* Get ticks required for TimeStamp */
        number_of_ticks = xTaskGetTickCount();
        led_data.timestamp = (number_of_ticks)*(portTICK_PERIOD_MS);

        /* Send the structure via Queue to logger task */
        if(xQueueSend( xQueue,( void * ) &led_data,(TickType_t )10) != pdPASS)
        {
            UARTprintf("Failed to post the message, even after 10 ticks");
        }
    }
}
