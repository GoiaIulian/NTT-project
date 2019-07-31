/*
 * error.c
 *
 *  Created on: Jul 31, 2019
 *      Author: yulia
 */

#include "error.h"

void Error_Handler(void){}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line){}
#endif
