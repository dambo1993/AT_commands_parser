/*
 * rejestry.h
 *
 *  Created on: 02.02.2017
 *      Author: Przemek
 */

#ifndef REJESTRY_H_
#define REJESTRY_H_

// 	REJESTRY DLA PERYFERII, KTORE MAJA KILKA INSTANCJI W MIKROKONTROLERZE

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY GPIO DLA F0xxxx                                             //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_MODER(x) (x->MODER)
#define GPIO_OTYPER(x) (x->OTYPER)
#define GPIO_OSPEED(x) (x->OSPEED)
#define GPIO_PUPDR(x) (x->PUPDR)
#define GPIO_IDR(x) (x->IDR)
#define GPIO_ODR(x) (x->ODR)
#define GPIO_BSRR(x) (x->BSRR)
#define GPIO_LCKR(x) (x->LCKR)
#define GPIO_AFRL(x) (x->AFR[0])
#define GPIO_AFRH(x) (x->AFR[1])
#define GPIO_BRR(x) (x->BRR)

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY ADC DLA F0xxxx                                              //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////

#define ADC_ISR(x)		(x->ISR)
#define ADC_IER(x)		(x->IER)
#define ADC_CR(x)		(x->CR)
#define ADC_CFGR1(x)	(x->CFGR1)
#define ADC_CFGR2(x)	(x->CFGR2)
#define ADC_SMPR(x)		(x->SMPR)
#define ADC_TR(x)		(x->TR)
#define ADC_CHSELR(x)	(x->CHSELR)
#define ADC_DR(x)		(x->DR)
#define ADC_CCR(x)		(x->CCR)

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY TIM DLA F0xxxx                                              //
//						WSZYSTKIE MOZLIWE													 //
///////////////////////////////////////////////////////////////////////////////////////////////

#define TIM_CR1(x)			(x->CR1)
#define TIM_CR2(x)			(x->CR2)
#define TIM_SMCR(x)			(x->SMCR)
#define TIM_DIER(x)			(x->DIER)
#define TIM_SR(x)			(x->SR)
#define TIM_EGR(x)			(x->EGR)
#define TIM_CCMR1(x)		(x->CCMR1)
#define TIM_CCMR2(x)		(x->CCMR2)
#define TIM_CCER(x)			(x->CCER)
#define TIM_CNT(x)			(x->CNT)
#define TIM_PSC(x)			(x->PSC)
#define TIM_ARR(x)			(x->ARR)
#define TIM_RCR(x)			(x->RCR)
#define TIM_CCR1(x)			(x->CCR1)
#define TIM_CCR2(x)			(x->CCR2)
#define TIM_CCR3(x)			(x->CCR3)
#define TIM_CCR4(x)			(x->CCR4)
#define TIM_BDTR(x)			(x->BDTR)
#define TIM_DCR(x)			(x->DCR)
#define TIM_DMAR(x)			(x->DMAR)
#define TIM_OR(x)			(x->OR)

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY UART DLA F0xxxx                                             //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////
#define UART_CR1(x) 	(x->CR1)
#define UART_CR2(x) 	(x->CR2)
#define UART_CR3(x) 	(x->CR3)
#define UART_BRR(x) 	(x->BRR)
#define UART_GTPR(x) 	(x->GTPR)
#define UART_RTOR(x) 	(x->RTOR)
#define UART_RQR(x) 	(x->RQR)
#define UART_ISR(x) 	(x->ISR)
#define UART_ICR(x) 	(x->ICR)
#define UART_RDR(x) 	(x->RDR)
#define UART_TDR(x) 	(x->TDR)

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY SPI DLA F0xxxx                                              //
//						                 													 //
///////////////////////////////////////////////////////////////////////////////////////////////

#define SPI_CR1(x)			(x->CR1)
#define SPI_CR2(x)			(x->CR2)
#define SPI_SR(x)			(x->SR)
#define SPI_DR(x)			(x->DR)
#define SPI_CRCPR(x)		(x->CRCPR)
#define SPI_RXCRCR(x)		(x->RXCRCR)
#define SPI_TXCRCR(x)		(x->TXCRCR)

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY I2C DLA F0xxxx                                              //
//						                 													 //
///////////////////////////////////////////////////////////////////////////////////////////////

#define I2C_CR1(x)			(x->CR1)
#define I2C_CR2(x)			(x->CR2)
#define I2C_OAR1(x)			(x->OAR1)
#define I2C_OAR2(x)			(x->OAR2)
#define I2C_TIMINGR(x)		(x->TIMINGR)
#define I2C_TIMEOUT(x)		(x->TIMEOUT)
#define I2C_ISR(x)			(x->ISR)
#define I2C_ICR(x)			(x->ICR)
#define I2C_PECR(x)			(x->PECR)
#define I2C_RXDR(x)			(x->RXDR)
#define I2C_TXDR(x)			(x->TXDR)




#endif /* REJESTRY_H_ */
