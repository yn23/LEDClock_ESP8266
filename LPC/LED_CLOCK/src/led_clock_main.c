/*
===============================================================================
 Name        : led_clock_main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
/*--------------------------------------------------------------------------------------------------
 * Includes
 ---------------------------------------------------------------------------------------------------*/
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "global_setting.h"
#include "handlers.h"
#include "mlc_cmn.h"
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
int main(void);

/* ---------- sample code ------------- */
void led_task( void );
void MLC_MainApp( uint32_t exinf );

/**
 * application main function
 */
int main(void) {

#if defined (__USE_LPCOPEN)
#if !defined(NO_BOARD_LIB)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    // Board_LED_Init();
#endif
#endif
    /* 初期化処理 */
    MLC_MainAppInit();

    /* 電源安定待ち */
    MLC_DelayMs(1000);

    /* GPIO割り込み設定 */
    set_gpio_interrupt();

    /* Base mode */
	eAplMode mode = APLMODE_AnalogClock;

    /* 無限ループ */
    while(1) {
    	mode = MLC_PrepareModeCheck();
    	switch (mode) {
		case APLMODE_AnalogClock:
			MLC_AnalogClockMain();
			break;
		case APLMODE_DigitalClock:
			MLC_DigitalClockMain();
		default:
			break;
    	}
    }
}
