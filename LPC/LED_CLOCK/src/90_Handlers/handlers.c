/*
===============================================================================
 Name        : handlers.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
/*--------------------------------------------------------------------------------------------------
 * Includes
 ---------------------------------------------------------------------------------------------------*/
#include <chip.h>
#include "board.h"
#include "handlers.h"
#include "mlc_app.h"
#include "mlc_port.h"

/*--------------------------------------------------------------------------------------------------
 * Macros
 ---------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------
 * Struct
 ---------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------
 * Lable
 ---------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------
 * Globals
 ---------------------------------------------------------------------------------------------------*/
uint32_t msTicks = 0;			/* tick counter */
uint32_t period = 0;			/* capture counter */

/*--------------------------------------------------------------------------------------------------
 * Functions
 ---------------------------------------------------------------------------------------------------*/

/**
 * tick interrupt handler
 */
void SysTick_Handler(void)  {	/* SysTick interrupt Handler.	*/
	msTicks++;                  /* See startup file startup_LPC11xx.s for SysTick vector */

	/* 40msに1回コール */
	if (msTicks % 40 == 0) {
		sys_40ms_counter();
	}
}

/**
 * get tick counter
 * return: tick counter
 */
inline uint32_t get_tick_time(void) {
	return msTicks;
}


/**
 * @brief	I2C Interrupt Handler
 * @return	None
 */
void I2C_IRQHandler(void)
{
	i2c_state_handling(I2C0);
}

/**
 * 16bit timer interrupt
 */
void TIMER16_0_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending( LPC_TIMER16_0, 0 )) {        //capture interrupt?
		LPC_TIMER16_0->TC = 0;                                  //reset timer
		// period = Chip_TIMER_ReadCapture( LPC_TIMER16_0, 0 );    //save counter CR0
		Chip_TIMER_ClearCapture( LPC_TIMER16_0, 0 );            //reset capture interrupt

	}
	/* コンペアマッチチェック */
	if (Chip_TIMER_MatchPending(LPC_TIMER16_0, 0)) {
		/* コンペアマッチ状態消去 */
		Chip_TIMER_ClearMatch( LPC_TIMER16_0, 0 );

    	// Board_LED_Toggle(1);
		MLC_LedControlMain(0);
	}
	return ;
}

/**
 * IRQ割り込み
 * (RTCからの1秒割り込み)
 */
void PIOINT2_IRQHandler(void)
{
	/* Clear interrupt */
	Chip_GPIO_ClearInts(LPC_GPIO, 2, (1 << 8));

	/* 1periodec */
	MLC_1sCounter();

	return ;
}
/* End of File */
