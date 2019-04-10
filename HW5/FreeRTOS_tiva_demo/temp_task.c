/*@FileName : temp_task.c
 *@Author   : Om Raheja
 *@Date     : 10th April 2019
 *@brief    : Reads temperature from the TMP102 temperature sensor
 *          : using I2C. Passes this information in a structure
 *          : to the logger task.
 */

/* Standard C Library Headers */
#include <stdio.h>
#include <string.h>

/* User Defined Header Files */
#include "temp_task.h"
#include "helper.h"

/* Exported Global Variables */
extern QueueHandle_t xQueue;
extern TaskHandle_t xAlert;

/* Temperature Task */
void temperature_task(void *pvParameters)
{
    char str[30];
    QueueData_t temp_data;
    uint32_t Temperature;
    float TempData;
    uint32_t number_of_ticks;

    for(;;)
    {
        /* Read temperature value from the sensor */
        /* Slave Address : 0x48
         * Reg address   : 0x00 */
        Temperature = I2CReceive(0x48,0x00);

        TempData = (Temperature)*(0.0625);

        sprintf(str,"%d.%d",(uint8_t)(TempData),(uint8_t)(TempData*100));

        strcpy(temp_data.temp_value,str);

        strcpy(temp_data.task,"temp");

        /* Get number of ticks */
        number_of_ticks = xTaskGetTickCount();

        temp_data.timestamp = (number_of_ticks)*(portTICK_PERIOD_MS);

        /* Notifies when temperature crosses threshold */
        if(strcmp(str,"30") > 0)
        {
            xTaskNotifyGive(xAlert);
        }

        if(xQueueSend( xQueue,( void * ) &temp_data,(TickType_t )10) != pdPASS)
        {
            UARTprintf("Failed to post the message, even after 10 ticks");
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);

    }
}


/* Alert Task */
void alert_task(void *pvParameters)
{
    QueueData_t alert_data;
    for(;;)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        strcpy(alert_data.task,"Alert");
        if(xQueueSend( xQueue,( void * ) &alert_data,(TickType_t )10) != pdPASS)
        {
            UARTprintf("Failed to post the message, even after 10 ticks");
        }
    }
}
