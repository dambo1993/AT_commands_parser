/*
 * debug.h
 *
 *  Created on: 02.02.2017
 *      Author: Przemek
 */

#ifndef DEBUG_H_
#define DEBUG_H_

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						LEDY DEBUGUJACE - MAKRA                                              //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////
#define DEBUG_LED1_GPIO 	GPIOA
#define DEBUG_LED1_PIN 		9
#define DEBUG_LED1_OUT 		GPIO_MODER(DEBUG_LED1_GPIO) |= ( 1 << ( 2 * DEBUG_LED1_PIN));
#define DEBUG_LED1_ON 		GPIO_BSRR(DEBUG_LED1_GPIO) = (1 << DEBUG_LED1_PIN);
#define DEBUG_LED1_OFF 		GPIO_BRR(DEBUG_LED1_GPIO) = ( 1 << DEBUG_LED1_PIN );
#define DEBUG_LED1_TOG 		GPIO_ODR(DEBUG_LED1_GPIO) ^= ( 1 << DEBUG_LED1_PIN );

#define DEBUG_LED2_GPIO 	GPIOA
#define DEBUG_LED2_PIN 		1
#define DEBUG_LED2_OUT 		GPIO_MODER(DEBUG_LED2_GPIO) |= ( 1 << ( 2 * DEBUG_LED2_PIN));
#define DEBUG_LED2_ON 		GPIO_BSRR(DEBUG_LED2_GPIO) = (1 << DEBUG_LED2_PIN);
#define DEBUG_LED2_OFF 		GPIO_BRR(DEBUG_LED2_GPIO) = ( 1 << DEBUG_LED2_PIN );
#define DEBUG_LED2_TOG 		GPIO_ODR(DEBUG_LED2_GPIO) ^= ( 1 << DEBUG_LED2_PIN );

#define DEBUG_LED3_GPIO 	GPIOA
#define DEBUG_LED3_PIN 		2
#define DEBUG_LED3_OUT 		GPIO_MODER(DEBUG_LED3_GPIO) |= ( 1 << ( 2 * DEBUG_LED3_PIN));
#define DEBUG_LED3_ON 		GPIO_BSRR(DEBUG_LED3_GPIO) = (1 << DEBUG_LED3_PIN);
#define DEBUG_LED3_OFF 		GPIO_BRR(DEBUG_LED3_GPIO) = ( 1 << DEBUG_LED3_PIN );
#define DEBUG_LED3_TOG 		GPIO_ODR(DEBUG_LED3_GPIO) ^= ( 1 << DEBUG_LED3_PIN );

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						PRZYCISKI DEBUGUJACE - MAKRA                                         //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////
#define DEBUG_BUTTON1_GPIO 		GPIOB
#define DEBUG_BUTTON1_PIN 		1
#define DEBUG_BUTTON1_INIT 		GPIO_MODER(DEBUG_BUTTON1_GPIO) &= ~( 0x03 << ( 2 * DEBUG_BUTTON1_PIN));
#define DEBUG_BUTTON1_PULL_UP 	GPIO_PUPDR(DEBUG_BUTTON1_GPIO) |= ( 0x01 << ( 2 * DEBUG_BUTTON1_PIN));
#define DEBUG_BUTTON1_PULL_DOWN	GPIO_PUPDR(DEBUG_BUTTON1_GPIO) |= ( 0x02 << ( 2 * DEBUG_BUTTON1_PIN));
#define DEBUG_BUTTON1_IS_LOW 	!(GPIO_IDR(DEBUG_BUTTON1_GPIO) & (1 << DEBUG_BUTTON1_PIN))
#define DEBUG_BUTTON1_IS_HIGH 	!(GPIO_IDR(DEBUG_BUTTON1_GPIO) & (1 << DEBUG_BUTTON1_PIN))

#define DEBUG_BUTTON2_GPIO 		GPIOB
#define DEBUG_BUTTON2_PIN 		2
#define DEBUG_BUTTON2_INIT 		GPIO_MODER(DEBUG_BUTTON2_GPIO) &= ~( 0x03 << ( 2 * DEBUG_BUTTON2_PIN));
#define DEBUG_BUTTON2_PULL_UP 	GPIO_PUPDR(DEBUG_BUTTON2_GPIO) |= ( 0x01 << ( 2 * DEBUG_BUTTON2_PIN));
#define DEBUG_BUTTON2_PULL_DOWN	GPIO_PUPDR(DEBUG_BUTTON2_GPIO) |= ( 0x02 << ( 2 * DEBUG_BUTTON2_PIN));
#define DEBUG_BUTTON2_IS_LOW 	!(GPIO_IDR(DEBUG_BUTTON2_GPIO) & (1 << DEBUG_BUTTON2_PIN))
#define DEBUG_BUTTON2_IS_HIGH 	!(GPIO_IDR(DEBUG_BUTTON2_GPIO) & (1 << DEBUG_BUTTON2_PIN))

#define DEBUG_BUTTON3_GPIO 		GPIOB
#define DEBUG_BUTTON3_PIN 		3
#define DEBUG_BUTTON3_INIT 		GPIO_MODER(DEBUG_BUTTON3_GPIO) &= ~( 0x03 << ( 2 * DEBUG_BUTTON3_PIN));
#define DEBUG_BUTTON3_PULL_UP 	GPIO_PUPDR(DEBUG_BUTTON3_GPIO) |= ( 0x01 << ( 2 * DEBUG_BUTTON3_PIN));
#define DEBUG_BUTTON3_PULL_DOWN	GPIO_PUPDR(DEBUG_BUTTON3_GPIO) |= ( 0x02 << ( 2 * DEBUG_BUTTON3_PIN));
#define DEBUG_BUTTON3_IS_LOW 	!(GPIO_IDR(DEBUG_BUTTON3_GPIO) & (1 << DEBUG_BUTTON3_PIN))
#define DEBUG_BUTTON3_IS_HIGH 	!(GPIO_IDR(DEBUG_BUTTON3_GPIO) & (1 << DEBUG_BUTTON3_PIN))



#endif /* DEBUG_H_ */
