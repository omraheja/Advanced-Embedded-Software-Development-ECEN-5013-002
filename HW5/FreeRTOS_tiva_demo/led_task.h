/*@FileName     : led_task.h
 *@Author       : Om Raheja
 *@Date         : 10th April 2019
 *@Course       : Advanced Embedded Software Development, Spring 2019
 */

#ifndef LED_TASK_H_
#define LED_TASK_H_

/* User Defined Header Files */
#include "helper.h"

/* Define and Macros */
#define D1_LED_ON()  GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
#define D2_LED_ON()  GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, CLP_D2_PIN);

#define D1_LED_OFF() GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
#define D2_LED_OFF() GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, 0);


/* Exported Global Variables */
extern SemaphoreHandle_t xMutex;

/* Function Prototypes */
/*@Function Name    : led_task
 *@brief            : This task toggles LEDs D1 and D2 on the TIVA
 *                  : board at a frequency of 10Hz. This task also
 *                  : passes a structure containing a time stamp,
 *                  : toggle count and author name to the logger
 *                  : task using a FreeRTOS IPC mechanism.
 *@param            : pvParameters
 *@return           : void
 * */
void led_task(void *pvParameters);



#endif /* LED_TASK_H_ */
