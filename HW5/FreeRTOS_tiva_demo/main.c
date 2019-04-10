/*@FileName : main.c
 *@Author   : Om Raheja
 *@Date     : 10th April 2019
 *@brief    : The code creates 4 tasks. Each of the tasks perform independent tasks.
 *          : Temperature task measures temperature at a frequency of 1Hz.
 *@Reference: Andy Kobyljanec Github
 *          : https://community.nxp.com/docs/DOC-331922
 */

/* Standard Library Headers */
#include <I2C.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"

/* TivaWare includes */
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/i2c.h"

/* FreeRTOS includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* User defined Headers */
#include "temp_task.h"
#include "led_task.h"
#include "logger_task.h"
#include "semphr.h"

/* Global Variables */
SemaphoreHandle_t xMutex = NULL;
uint32_t output_clock_rate_hz;
TaskHandle_t xAlert = NULL;

/* Global Variables for Queue */
QueueHandle_t xQueue;

/* Main function */
int main(void)
{
    /* Initialize system clock to 120 MHz */
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);

    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    /* Initialize the GPIO pins for the Launchpad */
    PinoutSet(false, false);

    /* Set up the UART which is connected to the virtual COM port */
    UARTStdioConfig(0, 57600, SYSTEM_CLOCK);

    /* Initialize I2C */
    I2CInit();

    /* Create Mutex */
    xMutex = xSemaphoreCreateMutex();

    /* Create Message Queue */
    xQueue = xQueueCreate(30,sizeof(QueueData_t));

    /* Check if Queue has been created */
    if( xQueue == NULL )
    {
       UARTprintf("\r\nQueue was not created!");
    }

    /* Create Logger Task */
    xTaskCreate(logger_task, (const portCHAR *)"LOGGER TASK",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /* Create Temperature Task */
    xTaskCreate(temperature_task, (const portCHAR *)"TEMP TASK",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /* Create Led Task */
    xTaskCreate(led_task, (const portCHAR *)"LED TASK",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /* Create Alert Task */
    xTaskCreate(alert_task, (const portCHAR *)"ALERT TASK",
                    configMINIMAL_STACK_SIZE, NULL, 1, &xAlert);

    vTaskStartScheduler();
    return 0;
}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
