/* USER CODE BEGIN Header */
/**
 * Jan 16, 2021
 * Albert Zhong
 * Simple code to transmit a character using UART without the need for HAL.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "stm32g071xx.h"


void UART3_Config(void);
void GPIOC_Config(void);
void UART3_SendChar (uint8_t c);

/*GPIOC_Config: Setup GPIO Port C to use button input on PC13. */
void GPIOC_Config(void)
{
	RCC->IOPENR |= (1 << 2); //enable Port C
	GPIOC->MODER &= ~(3 << 26);	//set PC13 as input
}

/*UART3_Config: Setup USART3_TX on PD8*/
void UART3_Config(void)
{

	RCC->APBENR1 |= (1 << 18);	//enable clock for USART3
	RCC->IOPENR |= (1 << 3);	//enable clock for GPIO PORT D. (Using PD8 for Transmit)

	GPIOD->OSPEEDR |= (1 << 16);	//set output speed to LOW on PD8

	GPIOD->MODER |= (1 << 17); //alternate function on PD8
	GPIOD->MODER &= ~(1 << 16);

	GPIOD->AFR[1] |= 0x0; // set AFO in AFRH register to use USART3_TX for PD8

	USART3->CR1 &= ~(1 << 0); //disable USART3 to allow changes to registers

	USART3->CR1 &= ~(1 << 28);	//program M bits, set word length to 8 bits
	USART3->CR1 &= ~(1 << 12);
	USART3->CR1 &= ~(1 << 15);	//oversampling by 16
	USART3->BRR |= 0x683;	//set the baud rate to 9600. PCLK default = 16MHz -> (16000000/9600) = 0x683. See reference manual on calculation.
	USART3->CR2 &= ~(3 << 12) ;	// program number of stop bits (1)

	USART3->CR1 |= (1 << 0);	//enable USART3
	USART3->CR1 |= (1 << 3); //Enable Transmission for USART3

}

/*UART3_SendChar: Load input into transmit register, and wait until transmission is complete*/
void UART3_SendChar (uint8_t c)
{
	USART3->TDR = c; // load the data into USART3_TDR, also clears TC bit in USART3_ISR.
	while (!(USART3->ISR & (1<<6)));  // Wait until transmission complete (TC = 1)
}


int main(void)
{

  //SystemClock_Config();
  GPIOC_Config();
  UART3_Config();

  while (1)
  {
	  if(!((GPIOC -> IDR) & 0x2000))
	  	  {
	  		  UART3_SendChar('C');
	  	  }
  }
}


