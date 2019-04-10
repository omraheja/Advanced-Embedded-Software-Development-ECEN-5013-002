/*@FileName : logger_task.c
 *@Author   : Om Raheja
 *@Date     : 10th April 2019
 *@brief    : This task receives messages from Temperature Task,
 *          : Led Task and Alert Task and Logs the values/messages
 *          : to UART so that logs can be viewed on the HOST machine.
 */

/* User defined Header files */
#include "logger_task.h"
#include "helper.h"

/* Variables for Queue */
extern QueueHandle_t xQueue;

/* Logger task */
void logger_task(void *pvParameters)
{
    while(1)
    {
        QueueData_t logger_data;

        /* Receive the data from message queue */
        if(xQueueReceive( xQueue, &(logger_data), ( TickType_t ) 10 ))
        {
            if(!strcmp(logger_data.task,"temp"))
            {
                UARTprintf("\r\n[TEMPERATURE]:[TIMESTAMP]: %d\tTEMP DATA: %s",logger_data.timestamp,logger_data.temp_value);
            }

            if(!strcmp(logger_data.task,"led"))
            {
                UARTprintf("\r\n[LED]        :[TIMESTAMP]: %d\tLED COUNT = %d\tNAME:%s",logger_data.timestamp,logger_data.led_count,logger_data.myname);
            }

            if(!strcmp(logger_data.task,"Alert"))
            {
                UARTprintf("\r\n[ALERT]      :TEMPERATURE OUT OF RANGE");
            }
        }
    }
}
