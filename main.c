#include "stm32f4xx_hal.h"
#include "usart.h"
#include "cli.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "SPIanalyzer.h"

void __aeabi_assert(const char *expr, const char *file, int line) { 
    printf("ASSERT ERROR in file %s at line %d.\nExpression: %s\nGoing into infinite loop.\n", file, line, expr);
    while(1) {
        ;
    }
}
//
//FUNCTION TO INITIALIZE LED
void InitializeLED(){
	GPIO_InitTypeDef GPIO_InitStruct;
 
  // GPIO Ports Clock Enable
  __GPIOG_CLK_ENABLE();
 
  // Configure GPIO pin PG13
  GPIO_InitStruct.Pin   = GPIO_PIN_13;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;        // push-pull output
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;              // pull-down enabled
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;             // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  // Configure GPIO pin PG14
  GPIO_InitStruct.Pin   = GPIO_PIN_14;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;        // push-pull output
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;              // pull-down enabled
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;             // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

//
//MAIN FUNCTION
int main(void){
	HAL_Init();
	InitializeLED();
	SPIA_Initialize();
	
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
	while(1);
}
