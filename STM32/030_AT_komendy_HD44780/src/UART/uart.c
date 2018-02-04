#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stdlib.h"
#include "uart.h"
#include "DMB/rejestry.h"
#include "stm32f0xx.h"


// bufory
volatile char UART_RxBuf[UART_RX_BUF_SIZE];
volatile char UART_TxBuf[UART_TX_BUF_SIZE];

volatile char uart_work_buff[UART_WORKBUFF_SIZE];

// indeksy odbiorcze:
volatile uint8_t UART_RxHead = 0;
volatile uint8_t UART_RxTail = 0;

volatile uint8_t UART_TxHead = 0;
volatile uint8_t UART_TxTail = 0;

volatile uint8_t otrzymane_linie = 0;

// Callback dla odebranej linii przez UART
uart_RX_Callback_type my_uart_callback_pointer;

void uart_register_rx_callback( uart_RX_Callback_type wsk )
{
	my_uart_callback_pointer = wsk;
}

void UART_INTERRUPT_HANDLER(void)
{
	//wysylanie - dla zwyklego uarta stosujmy to przerwanie
	if( ( UART_ISR(UARTx) & USART_ISR_TXE ) )
	{
		if (UART_TxHead != UART_TxTail)
		{
			//zwiekszamy index
			UART_TxTail++;
			if(UART_TxTail == UART_TX_BUF_SIZE)
			{
				UART_TxTail = 0;
			}

			UART_TDR(UARTx) = UART_TxBuf[UART_TxTail];
		}
		else
		{
			// wylaczamy przerwanie od pustego bufora nadawczego
			UART_CR1(UARTx) &= ~USART_CR1_TXEIE;
		}
	}
#ifdef RS485_ON
	//wysylanie - dla RS485 wykorzystujemy dodatkowo przerwanie TC
	if( ( UART_ISR(UARTx) & USART_ISR_TC ) )
	{
		// zgaszenie flagi
		UART_ICR(UARTx) = USART_ICR_TCCF;

		// jesli zakonczono wysylanie
		if (UART_TxHead == UART_TxTail)
		{
			// wylaczamy pin nadawania
			RS485_CONTROL_RECEIVE;
		}
	}
#endif

	if( ( UART_ISR(UARTx) & USART_ISR_RXNE ) ) //odbieranie
	{
		UART_RQR(UARTx) = USART_RQR_RXFRQ;
		register uint8_t tmp_head;
		register char data;

		data = UART_RDR(UARTx);

		// zwiekszenie indexu
		tmp_head = UART_RxHead + 1;

		if(tmp_head == UART_RX_BUF_SIZE)
		{
			tmp_head = 0;
		}

		if (tmp_head == UART_RxTail)
		{
			// zjadanie wlasnego ogona
		}
		else
		{
			switch(data)
			{
				case 0:
					break;
				case 10:
				{
					break;
				}
				case 13:
					otrzymane_linie++;
					UART_RxHead = tmp_head; 		// zapamiêtujemy nowy indeks
					UART_RxBuf[tmp_head] = 0; 	// wpisujemy odebrany bajt do bufora
					break;

				default:
				{
					UART_RxHead = tmp_head; 		// zapamiêtujemy nowy indeks
					UART_RxBuf[tmp_head] = data; 	// wpisujemy odebrany bajt do bufora
				}
			}
		}

	}
}

// funkcja sprawdzajaca wystapienie eventu - odebrania pelnej linii
void uart_event_check( void )
{
	if( otrzymane_linie )
	{
		uart_pobierz_linie(uart_work_buff);

		if( my_uart_callback_pointer )
		{
			my_uart_callback_pointer(uart_work_buff);
		}
	}
}

uint8_t uart_odebrano_linie( void )
{
	if( otrzymane_linie )
	{
		return 1;
	}
	return 0;
}

void uart_pobierz_linie( char *buf )
{
 	char c;

	if(otrzymane_linie)
	{
		while( (c = uart_pobierz_znak() ) )
		{
			*buf++ = c;
		}
		*buf = 0;
		otrzymane_linie--;
	}
}

void uart_wyslij_znak(char data)
{
#ifdef RS485_ON
	RS485_CONTROL_SEND;
#endif

	uint8_t tmp_head;

	// zwiekszenie indexu
	tmp_head = UART_TxHead + 1;

	if(tmp_head == UART_TX_BUF_SIZE)
	{
		tmp_head = 0;
	}

	// oczekiwanie na zwolnienie sie miejsca w buforze
	while (tmp_head == UART_TxTail) {
	}

	UART_TxBuf[tmp_head] = data;
	UART_TxHead = tmp_head;

	// uruchomienie przerwania od pustego rejestru TX
	UART_CR1(UARTx) |= USART_CR1_TXEIE;

}

void uart_wyslij_lancuch(char *s)
{
	register char c;
	while ((c = *s++))
		uart_wyslij_znak(c);
}

void uart_wyslij_lancuch_r(char *s)
{
	register char c;
	while ((c = *s++))
		uart_wyslij_znak(c);
	uart_wyslij_znak('\r');
}

void uart_wyslij_lancuch_rn(char *s)
{
	register char c;
	while ((c = *s++))
		uart_wyslij_znak(c);
	uart_wyslij_znak('\r');
	uart_wyslij_znak('\n');
}


uint8_t uart_odbior_sprawdz() {
	if (UART_RxHead == UART_RxTail) {
		return 0;
	}
	return 1;
}


uint8_t uart_pobierz_znak(void)
{
	// sprawdzamy czy indeksy s¹ równe
	if (UART_RxHead == UART_RxTail)
		return 0;

	// zwiekszenie indexu
	UART_RxTail = UART_RxTail + 1;

	if(UART_RxTail == UART_RX_BUF_SIZE)
	{
		UART_RxTail = 0;
	}
	// zwracamy bajt pobrany z bufora  jako rezultat funkcji
	return UART_RxBuf[UART_RxTail];
}

void uart_wyslij_liczbe(int16_t liczba)
{
	char buf[17];
	itoa(liczba, buf, 10);
	uart_wyslij_lancuch(buf);
}

void init_uart(uint32_t baudrate)
{
	UART_SYSCFG_EN;

	// wlaczenie zegara dla UARTu
	UART_CLK_EN;

	//wlaczenie zegara dla portu gpio:
	UART_PINS_CLK_EN;

	// ustawienie piny jako alternate
	UART_TX_ALTERNATE;
	UART_RX_ALTERNATE;

	// ustawiamy numer opcji alternatywnej
	UART_TX_ALTERNATE_NR;
	UART_RX_ALTERNATE_NR;

#ifdef RS485_ON
	RS485_CONTROL_OUT;
	RS485_CONTROL_RECEIVE;
#endif

	// dlugosc znaku (8bit) jest ustawione domyslnie

	// tak samo jak 1 bit stopu

	// ustawienie baudrate:
	UART_BRR(UARTx) = UART_CLK_SPEED / baudrate;

	// wlaczenie nadajnika:
	UART_CR1(UARTx)  |= (USART_CR1_TE);

	// wlaczenie odbiornika:
	UART_CR1(UARTx)  |= (USART_CR1_RE);

	// uruchomienie przerwania odbiorczego
	UART_CR1(UARTx)  |= USART_CR1_RXNEIE;

	// uruchomienie przerwania nadawczego
	UART_CR1(UARTx)  |= USART_CR1_TXEIE;

#ifdef RS485_ON
	// uruchomienie przerwania od odebranego znaku
	UART_CR1(UARTx)  |= USART_CR1_TCIE;
#endif

	// wlaczenie uartu:
	UART_CR1(UARTx) |= USART_CR1_UE;

	// wlaczenie przerwania w NVICu
	NVIC_EnableIRQ( UART_INTERRUPT_NUMBER );
}


#ifdef __cplusplus
}
#endif

