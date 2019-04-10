/*@FileName     : temp_task.h
 *@Author       : Om Raheja
 *@Date         : 10th April 2019
 *@Course       : Advanced Embedded Software Development, Spring 2019
 */

#ifndef TEMP_TASK_H_
#define TEMP_TASK_H_

/* User Defined Header Files */
#include "helper.h"

/* Exported Global Variables */
extern SemaphoreHandle_t xMutex;

/* Function Prototypes */
/*@Function Name    : temperature_task
 *@brief            : This task measures the temperature from the TMP102
 *                  : temperature sensor via I2C and send it to the
 *                  : logger task using queues. It sends a structure along
 *                  : with time stamp.
 *
 *@param            : pvParameters
 *@return           : void
 * */
void temperature_task(void *pvParameters);

/*@Function Name    : alert_task
 *@brief            : This task gets notified when the temperature
 *                  : crosses a certain threshold.
 *@param            : pvParameters
 *@return           : void
 * */
void alert_task(void *pvParameters);

#endif /* TEMP_TASK_H_ */
