/*
 * mlc_uart.c
 *
 *  Created on: 2015/07/20
 *      Author: yohji
 */
#include <chip.h>
#include "board.h"
#include "mlc_config.h"
#include "xprintf.h"

/*--------------------------------------------------------------------------------------------------
 * Macros
 ---------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------
 * Struct
 ---------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------
 * Lable
 ---------------------------------------------------------------------------------------------------*/
void uart_out(uint8_t out_data);

/*--------------------------------------------------------------------------------------------------
 * Globals
 ---------------------------------------------------------------------------------------------------*/
#if UART_COM
/* Transmit and receive ring buffers */
RINGBUFF_T txring, rxring;

/* Transmit and receive buffers */
uint8_t rxbuff[UART_RB_SIZE], txbuff[UART_RB_SIZE];
bool      gRecvCommandExecute = false;
#endif	/* UART_COM */

/*--------------------------------------------------------------------------------------------------
 * 割り込みハンドラ
 ---------------------------------------------------------------------------------------------------*/
#if UART_COM
/*****************************************************************************
** Function name:		UART_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART_IRQHandler(void)
{
	/* Handle transmit interrupt if enabled */
	if (LPC_USART->IER & UART_IER_THREINT) {
		Chip_UART_TXIntHandlerRB(LPC_USART, &txring);

		/* Disable transmit interrupt if the ring buffer is empty */
		if (RingBuffer_IsEmpty(&txring)) {
			Chip_UART_IntDisable(LPC_USART, UART_IER_THREINT);
		}
	}

	/* New data will be ignored if data not popped in time */
	while (Chip_UART_ReadLineStatus(LPC_USART) & UART_LSR_RDR) {
		uint8_t ch = Chip_UART_ReadByte(LPC_USART);
		/* check can display ascii code */
		if (ch >= 0x20 && ch <= 0x7E) {
			/* insert receive data */
			RingBuffer_Insert(&rxring, &ch);
		}
		/* check command end code(CR or LF) */
		if ((ch == 0x0D) || (ch == 0x0A)) {
			/* set command execute */
			gRecvCommandExecute = true;
		}
	}

}

/**
 *
 * UART通信
 *
 */
void uart_init(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT)); /* RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT)); /* TXD */

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);

	/* setup ring buffer */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_RB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);

	// uart_out
	xdev_out(uart_out);


	return ;
}
#endif	/* UART_COM */

