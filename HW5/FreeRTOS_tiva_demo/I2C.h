/*@FileName     : I2C.h
 *@Author       : Om Raheja
 *@Date         : 10th April 2019
 *@Course       : Advanced Embedded Software Development, Spring 2019
 */
#ifndef I2C_H_
#define I2C_H_

/* Standard C Library Headers */
#include "stdint.h"

/* Function Prototypes */
/*@Function Name    : I2CInit
 *@brief            : Initializes the I2C.
 *@param            : void
 *@return           : void
 * */
void I2CInit(void);

/* Function Prototypes */
/*@Function Name    : I2CReceive
 *@brief            : Receives temperature value from the temperature sensor.
 *@param1           : slave address
 *@param2           : Register address
 *@return           : uint32_t
 * */
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);


#endif /* I2C_H_ */
