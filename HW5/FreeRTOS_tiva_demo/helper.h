/*@FileName     : helper.h
 *@Author       : Om Raheja
 *@Date         : 10th April 2019
 *@Course       : Advanced Embedded Software Development, Spring 2019
 */

#ifndef HELPER_H_
#define HELPER_H_

/* Standard C Library Headers */
#include <I2C.h>
#include <string.h>

/* TivaWare and FreeRTOS Includes  */
#include "FreeRTOS.h"
#include "semphr.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"
#include "FreeRTOS/include/task.h"
#include "driverlib/inc/hw_memmap.h"

/* Structure */
typedef struct
{
    char temp_value[30];
    char myname[10];
    char task[10];
    int led_count;
    uint32_t timestamp;

}QueueData_t;


#endif /* HELPER_H_ */
