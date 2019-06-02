#include "SPIanalyzer.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "usart.h"

//PREPROCESOR
#define PresV 16000 						//one tic = 1ms
#define PeriodV 500
#define GPIO_SCLK GPIO_PIN_0
#define GPIO_MOSI GPIO_PIN_2
#define GPIO_MISO GPIO_PIN_4
#define GPIO_SS GPIO_PIN_6
#define RisingMode 0						//1- rising, 0-falling

//GLOBAL VARIABLE
uint8_t prevSCLK = 2;					//2 mean first SCLK
uint8_t MOSIbuffor[100];
uint8_t MISObuffor[100];
uint8_t count;
uint8_t mode=1;
static TIM_HandleTypeDef s_TimerInstance = { 
	.Instance = TIM2
};

//
//Function to GPIO
void SPIA_InitializeGPIO(){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// GPIO Ports Clock Enable
	__GPIOE_CLK_ENABLE();
	
	//Configure GPIO_SCLK
	GPIO_InitStruct.Pin = GPIO_SCLK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // digital Input
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
	
	//Configure GPIO_MOSI
	GPIO_InitStruct.Pin = GPIO_MOSI;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // digital Input
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
	
	//Configure GPIO_MISO
	GPIO_InitStruct.Pin = GPIO_MISO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // digital Input
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
	
	//Configure GPIO_SS
	GPIO_InitStruct.Pin = GPIO_SS;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // digital Input
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
}
//
//Function to initialize timer
void SPIA_InitializeTimer(){
	__TIM2_CLK_ENABLE();
	s_TimerInstance.Init.Prescaler = PresV;    												//one tic = 1ms
	s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
	s_TimerInstance.Init.Period = PeriodV;														//0.1s
	s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	s_TimerInstance.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&s_TimerInstance);
	HAL_TIM_Base_Start_IT(&s_TimerInstance);
}

//
//Function to initialize SPI analyzer (GPIO & Timer)
void SPIA_Initialize(){
	SPIA_InitializeGPIO();
	SPIA_InitializeTimer();
}
//
//Function to service SCLK changes
//return 1-rising edge, something happen, return 0-everything else, nothing happen
uint8_t SPIA_SCLK_Service(){
	uint8_t tempSCLK = HAL_GPIO_ReadPin(GPIOE, GPIO_SCLK);
	
	if (prevSCLK == 2){														//if no one prevSCLK
		prevSCLK = tempSCLK;
		return 0;
	}
	else if(prevSCLK == tempSCLK){								//no logic change
		return 0;
	}
	else if(prevSCLK == 1 && tempSCLK == 0){			//falling edge
		prevSCLK = tempSCLK;
		return (!RisingMode);
	}
	else if(prevSCLK == 0 && tempSCLK == 1){			//rising edge
		prevSCLK = tempSCLK;
		return RisingMode;
	}
	return 0;
}
//
//Function to service SS value
//return 1-this device is chosen, return 0-this device is not chosen
uint8_t SPIA_SS_Service(){
	uint8_t tempSS = HAL_GPIO_ReadPin(GPIOE, GPIO_SS);
	return (!tempSS);
}
//
//Interupt Handler
void TIM2_IRQHandler(){
	HAL_TIM_IRQHandler(&s_TimerInstance);
}
//
//Interupt Handler Support Function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (SPIA_SS_Service()){
		if (SPIA_SCLK_Service()){
			MOSIbuffor[count] = HAL_GPIO_ReadPin(GPIOE, GPIO_MOSI);
			MISObuffor[count] = HAL_GPIO_ReadPin(GPIOE, GPIO_MISO);
			count++;
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		}
		else
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	if(count == 100){
		mode = 2;
	}
}
//
//Function to sending data by uart
void SPIA_SendData(){
	char elem[2];
	/*for (int i=0; i<100; i++){
		sprintf(elem,"%d",MOSIbuffor[i]);
		USART_POLL_WriteString(elem);
	}
	for (int i=0; i<100; i++){
		sprintf(elem,"%d",MISObuffor[i]);
		USART_POLL_WriteString(elem);
	}*/
}
//
//Main function of library
void SPIA_function(){
	//Configuration
	//while(mode==0){
	//}
	
	//Collecting data
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	while(mode!=2);
	
	//Sending data
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	while(mode==2){
		SPIA_SendData();
	}
}
