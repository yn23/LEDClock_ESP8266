/*
 * @brief LPCXpresso 11C24 board file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __BOARD_H_
#define __BOARD_H_

#include <00_chip/chip.h>

/* board_api.h is included at the bottom of this file after DEBUG setup */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BOARD_NXP_XPRESSO_11C24 NXP LPC11C24 LPCXpresso board support software API functions
 * @ingroup LPCOPEN_11XX_BOARD_XPRESSO_11C24
 * The board support software API functions provide some simple abstracted
 * functions used across multiple LPCOpen board examples. See @ref BOARD_COMMON_API
 * for the functions defined by this board support layer.<br>
 * @{
 */

/** @defgroup BOARD_NXP_XPRESSO_11C24_OPTIONS BOARD: NXP LPC11C24 LPCXpresso board build options
 * This board has options that configure its operation at build-time.<br>
 */

/** Define DEBUG_ENABLE to enable IO via the DEBUGSTR, DEBUGOUT, and
    DEBUGIN macros. If not defined, DEBUG* functions will be optimized
	out of the code at build time.
 */
// #define DEBUG_ENABLE

/** Define DEBUG_SEMIHOSTING along with DEBUG_ENABLE to enable IO support
    via semihosting. You may need to use a C library that supports
	semihosting with this option.
 */
// #define DEBUG_SEMIHOSTING

/** Board UART used for debug output and input using the DEBUG* macros. This
    is also the port used for Board_UARTPutChar, Board_UARTGetChar, and
	Board_UARTPutSTR functions.
 */
#if defined(DEBUG_ENABLE)
#define DEBUG_UART LPC_USART
#else
#define UART_COM	1
#endif

/**
 * @}
 */

/* Board name */
// #define BOARD_NXP_XPRESSO_11C24
#define BOARD_AITENDO_LPC_1114

/**
 * @}
 */
void Board_SystemInit(void);


/* aitendo boardの場合 */
#if defined(BOARD_AITENDO_LPC_1114)

void Board_LED_Init(void);
void Board_LED_Set(uint8_t LEDNumber, bool On);
bool Board_LED_Test(uint8_t LEDNumber);
void Board_LED_Toggle(uint8_t LEDNumber);

#if defined(DEBUG_ENABLE)
#if defined(DEBUG_SEMIHOSTING)
#define DEBUGINIT()
#else
#define DEBUGINIT() Board_Debug_Init()
#endif /* defined(DEBUG_SEMIHOSTING) */

#else
#define DEBUGINIT()
#endif /* defined(DEBUG_ENABLE) */



#endif /* BOARD_AITENDO_LPC_1114 */

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */
