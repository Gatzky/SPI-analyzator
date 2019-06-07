#ifndef SPIanalyzer_H_
#define SPIanalyzer_H_

//Function to initialize SPI analyzer (GPIO & Timer)
void SPIA_Initialize(void);
//main function of library
void SPIA_function(void);

void SPIA_SendData(void);

#endif //SPIanalyzer_H_
