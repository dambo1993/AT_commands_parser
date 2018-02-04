/*
 * RCC.c
 *
 *  Created on: 14.01.2017
 *      Author: Przemek
 */


#include "stm32f0xx.h"
#include "RCC.h"

void RCC_init(void)
{
	uint32_t StartUpCounter = 0, HSEStatus = 0;

	/* uruchomienie HSE */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);

	/* oczekiwanie na gotowosc HSE*/
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	//sprawdzanie gotowosci HSE
	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		//mozemy zmienic taktowanie na HSE

		//spowolnienie pamieci flash i wlaczenie buforowania (max komunikacja z FLASH to 24MHz)
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

		//preskaler dla AHB  - domyslnie 1
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

		//preskaler dla APB - domyslnie 1
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

//		//wybranie HSE na zegar systemowy
//		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW)); //zerowanie rejestru
//		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE; //wybor HSE
//
//		//oczekiwanie, az HSE bedzie zrodlem taktowania
//		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_HSE)
//		{
//
//		}

		//ustawienia PLL
		/* PLL mnoznik = HSE * 6 = 48 MHz */
		// zerowanie rejestrow (jakby co)
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE_PREDIV| RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV1 | RCC_CFGR_PLLMUL6);

		/* wlaczenie PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* oczekiwanie na gotowosc PLL */
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
		{

		}

		/* Wybranie PLL jako zrodla taktowania */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

		/* oczekiwanie, az PLL stanie sie zrodlem taktowania */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
		{

		}
	}
	else
	{
//		nie gotowe (mozemy tu dac jakis reset czy cos typu miganie dioda bledu)

		// petla nieskonczona
		while(1)
		{

		}
	}
}
