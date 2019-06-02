#include "delay.h"
static volatile uint32_t counter = 0;

/**
 * System Tick Interrupt Service Routine 
 */
void SysTick_Handler(void)
{
    // nothing to do here yet
	counter++;
        
} /* SysTick_Handler */

void delay(uint32_t seconds)
{
	uint32_t buffor = counter;
	while(counter<(buffor+seconds));
} /* delay */
