/*@FileName     : logger_task.h
 *@Author       : Om Raheja
 *@Date         : 10th April 2019
 *@Course       : Advanced Embedded Software Development, Spring 2019
 */


#ifndef LOGGER_TASK_H_
#define LOGGER_TASK_H_

/* User Defined Header Files */
#include "helper.h"

/* Exported Global Variables */
extern SemaphoreHandle_t xMutex;

/* Function Prototypes */
/*@Function Name    : logger_task
 *@brief            : This task receives messages from Temperature Task,
 *                    Led Task and Alert Task and Logs the values to UART
 *                    so that logs can be viewed on the HOST machine.
 *@param            : pvParameters
 *@return           : void
 * */
void logger_task(void *pvParameters);

#endif /* LOGGER_TASK_H_ */
