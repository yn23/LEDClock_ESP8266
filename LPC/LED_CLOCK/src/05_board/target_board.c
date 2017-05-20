/*
 * target_board.c
 *
 *  Created on: 2015/05/03
 *      Author: yohji
 */
#include "board.h"
#include "xprintf.h"
#include "mlc_cmn.h"

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT)); /* RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT)); /* TXD */

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);

	// uart_out
	xdev_out(uart_out);
#endif
}

/* Initializes board LED(s) */
void Board_LED_Init(void)
{
	/* LED1: Set the PI1_5 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO, 1, 2, true);
	/* LED2: Set the PIO_7 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 0, true);
}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber == 0)
		Chip_GPIO_WritePortBit(LPC_GPIO, 1, 2, !On);
	else if (LEDNumber == 1)
		Chip_GPIO_WritePortBit(LPC_GPIO, 3, 0, On);
	}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t LEDNumber)
{
	bool res = false;

	if (LEDNumber == 0)
		res = !Chip_GPIO_ReadPortBit(LPC_GPIO, 1, 2);
	else if (LEDNumber == 1)
		res = Chip_GPIO_ReadPortBit(LPC_GPIO, 3, 0);

	return res;

}

void Board_LED_Toggle(uint8_t LEDNumber)
{
	Board_LED_Set( LEDNumber, !Board_LED_Test( LEDNumber ) );
}

