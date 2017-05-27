/********************************************************/
// FILE: mlc_app.c
// DESCRIPTION: Matrix LED Control
// Copyright (CC) 2015.01.18 Y.Narisawa
/********************************************************/
// This work is licensed under the Creative Commons
// Attribution 4.0 Unported License.
// To view a copy of this license, visit
// http://creativecommons.org/licenses/by/4.0/
// Good Luck
/********************************************************/
/**
 * @file	mlc_app.c
 * @brief	Matrix LED Clock Aplication Main
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */

#include <string.h>
#include "global_setting.h"
#include "mlc_app.h"
#include "mlc_draw_clock.h"
#include "mlc_config.h"
#include "mlc_port.h"
#include "mlc_cmn.h"

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
/* ---- color setting ---- */
int32_t s_colorDepthCnt = 0;
/* ---- led righting time ---- */
int32_t s_ledOnTime = MLC_LED_OUTPUT_MAX;

/* ---- now time ---- */
int32_t s_counter = 0;
int32_t s_counter_M = 0;
int32_t s_counter_H = 0;
bool    isAM = TRUE;

/* ---- now date ---- */
char s_nowDate[DATE_LENGTH];

/* ---- now weather ---- */
eAplWeather s_weather = APLWeather_Fine;

/* ---- now temperature ---- */
bool isPlus = TRUE;
int32_t s_temperature = 0;

/* ---- now information ---- */
char s_information[UART_RB_SIZE];

uint32_t chatteringCnt = 0;		/* chattering counter */
uint32_t longPressCnt = 0;
bool longPressMode = FALSE;

/* ---- application mode ---- */
//eAplMode s_AplMode = APLMODE_DigitalClock;
eAplMode s_AplMode = APLMODE_AnalogClock;
/* ---- command recv flg ---- */
extern bool gRecvCommandExecute;

/*--------------------------------------------------------------------------------------------------
 * Globals (commands)
 ---------------------------------------------------------------------------------------------------*/
const char SET_TIME_CMD[] = "SET_TIME";
const char SET_DATE_CMD[] = "SET_DATE";
const char SET_MODE_CMD[] = "SET_MODE";
const char INFO_WTH_CMD[] = "INFO_WTH";
const char INFO_TMP_CMD[] = "INFO_TMP";
const char INFO_STR_CMD[] = "INFO_STR";


/**
 * 40msCounter
 */
void sys_40ms_counter() {

	/* 輝度変更 */
	if (FALSE == Chip_GPIO_GetPinState(LPC_GPIO, 3, 2)) {
		chatteringCnt++;
		/* 40ms * 4回でButton downと判断 */
		if (chatteringCnt >= 4) {
			chatteringCnt = 0;

			MLC_LedOnTimeUp();
		}
	}

	/* Mボタン */
	if (FALSE == Chip_GPIO_GetPinState(LPC_GPIO, 1, 11)) {
		chatteringCnt++;
		/* 40ms * 3回でButton downと判断 */
		if (FALSE == longPressMode) {
			if (chatteringCnt >= 4) {
				chatteringCnt = 0;
				if (s_AplMode == APLMODE_AnalogClock) {
					s_AplMode = APLMODE_DigitalClock;
				}
				else {
					s_AplMode = APLMODE_AnalogClock;
				}
			}
		}
		else {
			MLC_1mCounter();
		}
	}
	return ;
}

/**
 * 時計更新機能
 */
void MLC_1sCounter() {

	// 秒更新
	s_counter++;

	/* 1分経過(長針進める) */
	if ((s_counter % CLOCK_SECOND) == 0) {
		s_counter = 0;
		s_counter_M++;
		/* 1時間経過 */
		if ((s_counter_M % CLOCK_SECOND) == 0) {
			s_counter_H++;
			s_counter_M = 0;
			/* 12時間経過 */
			if ((s_counter_H % CLOCK_SECOND) == 0) {
				s_counter_H = 0;
				isAM = !isAM;
			}
		}
		/* 10分経過(短針進める) */
		else if ((s_counter_M % (CLOCK_MIN / 5)) == 0) {
			s_counter_H++;
		}
	}

	/* Mボタン */
	if (FALSE == Chip_GPIO_GetPinState(LPC_GPIO, 1, 11)) {
		longPressCnt++;
		if (longPressCnt >= 3) {
			longPressMode = TRUE;
		}
	}
	else {
		longPressCnt = 0;
		longPressMode = FALSE;
	}

//	xprintf("H=%d_M=%d\r\n", s_counter_H, s_counter_M);
	return ;
}

/*
 * 「分」変更機能
 */
void MLC_1mCounter() {
	s_counter_M++;
	/* 10分経過(短針進める) */
	if ((s_counter_M % (CLOCK_MIN / 5)) == 0) {
		s_counter_H++;
	}
}

/**
 * 明るさ変更
 */
void MLC_LedOnTimeUp() {
	s_ledOnTime++;

	/* 明るさ変更 */
	if (MLC_LED_OUTPUT_MAX <= s_ledOnTime) {
		s_ledOnTime = 0;
	}
}

/**
 * @fn
 * MatrixLedの初期化処理
 * @breaf MatrixLedの初期化処理
 * @param exinf タスク起動時のパラメータ
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_MainAppInit( void )
{
	/* MatrixLedのペリフェラル関係の初期化処理を行う */
	MLC_InitPort();

	/* set tick interval */
    (void)SysTick_Config(SystemCoreClock / TICK_INTERVAL);      /* Configure SysTick to generate an interrupt every millisecond */

    /* set 16bit timer */
    cmt_timer16_init(0);

    /* RTCの電源安定待ち */
    MLC_DelayMs(1000);

    /* RTC初期化 */
    i2c_init();

    /* uart初期化 */
    uart_init();

	s_ledOnTime = MLC_LED_OUTPUT_MAX;

	return ;
}

/**
 * @file	mlc_app.c
 * @brief	Matrix LED Clock アプリケーションメイン
 * @author	Y.Narisawa
 * @date 	2015.02.01
 */
void MLC_LedControlMain( uint32_t exinf ) {
	int32_t i;
	int32_t led_on_time = s_ledOnTime;
	uint16_t*	pFrmBuf = NULL;

	/* 描画用のフレームバッファのアドレスを取得する */
	pFrmBuf = MLC_GetFrmBufAdr();

	/* 上側のMatrix LEDの点灯処理を行う */
	for (i = 0; i < 8; i++) {
		MLC_DE1_LowHigh(PORT_HIGH);	// Low

		MLC_SetAddress(i);
		MLC_DrawLineTop(i, s_colorDepthCnt, pFrmBuf);

		MLC_DE1_LowHigh(PORT_LOW);
		MLC_Delay10Cycle(led_on_time);
	}
	MLC_DE1_LowHigh(PORT_HIGH);	// Low

	/* 下側のMatrix LEDの点灯処理を行う */
	for (i = 16; i < 24; i++) {
		MLC_DE2_LowHigh(PORT_HIGH);

		MLC_SetAddress(i);
		MLC_DrawLineBottom(i, s_colorDepthCnt, pFrmBuf);

		MLC_DE2_LowHigh(PORT_LOW);
		MLC_Delay10Cycle(led_on_time);
	}
	MLC_DE2_LowHigh(PORT_HIGH);

	/* colorDepth制御用カウンタ更新 */
	s_colorDepthCnt++;
	if (MLC_MAX_COLOR_DEPTH == s_colorDepthCnt) {
		s_colorDepthCnt = 0;
	}
	return;
}

/**
 * @file	mlc_app.c
 * @brief	Matrix LED Clock アプリケーションメイン
 * @author	Y.Narisawa
 * @date 	2015.02.01
 */
void MLC_AnalogClockMain(  ) {

	int8_t bufNum;

//	syslog_0(1, "MLC_MainApp call!!\n");
	/* 線を描画 */
	MLC_DrawLineControl_S(s_counter % CLOCK_SECOND);
	MLC_DrawLineControl_M(s_counter_M % CLOCK_SECOND);
	MLC_DrawLineControl_H(s_counter_H % CLOCK_SECOND);
	bufNum = MLC_GetFrmBufNum();
	if (0 == bufNum) {
		MLC_SetFrmBufNum(1);
		MLC_initFrmBufNum(0);
	}
	else {
		MLC_SetFrmBufNum(0);
		MLC_initFrmBufNum(1);
	}
	return;
}

/**
 * @file	mlc_app.c
 * @brief	Matrix LED Clock アプリケーションメイン
 * @author	Y.Narisawa
 * @date 	2015.02.01
 */
void MLC_DigitalClockMain(  ) {

	int8_t bufNum;
	/* 日時を描画 */
	MLC_DrawDate(s_nowDate, sizeof(s_nowDate));
	MLC_DrawTime(isAM, s_counter_H, s_counter_M, s_counter);
	/* 気温を描画 */
	MLC_DrawTemp(s_temperature, isPlus);
	/* マークを描画 */
	MLC_DrawMark(s_weather);

	/* 線を描画 */
	bufNum = MLC_GetFrmBufNum();
	if (0 == bufNum) {
		MLC_SetFrmBufNum(1);
		MLC_blankFrmBufNum(0);
	}
	else {
		MLC_SetFrmBufNum(0);
		MLC_blankFrmBufNum(1);
	}
	return;
}


/**
 * @file	mlc_app.c
 * @brief	get mode information of Matrix LED Clock from UART
 * @author	Y.Narisawa
 * @date 	2017.04.20
 */
eAplMode MLC_PrepareModeCheck( void ) {


	char tempBuf[UART_RB_SIZE];

	if (gRecvCommandExecute) {
		int ret = MLC_GetCommand(tempBuf, UART_RB_SIZE);
		if (0 < ret) {
			MLC_ParseCommand(tempBuf, ret);
		}
		gRecvCommandExecute = false;
	}
	return s_AplMode;
}

/**
 * @file	mlc_app.c
 * @brief	get command from uart
 * @author	Y.Narisawa
 * @date 	2017.04.20
 */
int32_t MLC_GetCommand( char* buffer, int size ) {
	int32_t		ret_code = 0;

	if (0 < RingBuffer_GetCount(&rxring)) {
		ret_code = RingBuffer_PopMult(&rxring, buffer, size);
	}
	return ret_code;
}


void MLC_ParseCommand( char* buffer, int size ) {
	int32_t work = 0;
	char* tempPtr = NULL;
	char  resultBuf[] = "OK\n";

    if (0 == memcmp(buffer, SET_TIME_CMD, strlen(SET_TIME_CMD))) {
    	tempPtr = buffer + strlen(SET_TIME_CMD);
    	// calculate hour
    	work = (int)(tempPtr[0] - '0') * 10;
    	work += (int)(tempPtr[1] - '0');
    	if (work >= 24) {
    		work = 23;
    	}
    	if (work >= 12) {
    		isAM = false;
    	}
    	work = (work * 5);
    	if (work >= 60) {
    		work -= 60;
    	}
    	s_counter_H = work;

    	// calculate min
    	work = (int)(tempPtr[2] - '0') * 10;
    	work += (int)(tempPtr[3] - '0');
    	if (work >= 60) {
    		work = 59;
    	}
    	s_counter_M = work;
    	s_counter_H = s_counter_H + (s_counter_M / 12);
    	DEBUG_PRINT("H:%d M:%d\n", s_counter_H, s_counter_M);
    }
    else if (0 == memcmp(buffer, SET_DATE_CMD, strlen(SET_DATE_CMD))) {
    	tempPtr = buffer + strlen(SET_DATE_CMD);

    	// set date
    	memset(s_nowDate, 0x00, sizeof(s_nowDate));
    	memcpy(s_nowDate, tempPtr, 6);

    	DEBUG_PRINT("date:%s\n", s_nowDate);
    }
    else if (0 == memcmp(buffer, SET_MODE_CMD, strlen(SET_MODE_CMD))) {
    	tempPtr = buffer + strlen(SET_MODE_CMD);
    	if ('A' == *tempPtr || 'a' == *tempPtr) {
    		s_AplMode = APLMODE_AnalogClock;
    	}
    	else {
    		s_AplMode = APLMODE_DigitalClock;
    	}
    	DEBUG_PRINT("Mode:%d\n", s_AplMode);
    }
    else if (0 == memcmp(buffer, INFO_WTH_CMD, strlen(INFO_WTH_CMD))) {
    	tempPtr = buffer + strlen(INFO_WTH_CMD);
    	if ((tempPtr[0] >= '0') && (tempPtr[0] <= '9')) {
			s_weather = *tempPtr - '0';
    	}
    	else if ((tempPtr[0] >= 'a') && (tempPtr[0] <= 'f')) {
        	s_weather = *tempPtr - 'a';
        	s_weather+=10;
    	}
    	else if ((tempPtr[0] >= 'A') && (tempPtr[0] <= 'F')) {
        	s_weather = *tempPtr - 'A';
        	s_weather+=10;
    	}
    	DEBUG_PRINT("weather:%d\n", s_weather);
    }
    else if (0 == memcmp(buffer, INFO_TMP_CMD, strlen(INFO_TMP_CMD))) {
    	tempPtr = buffer + strlen(INFO_TMP_CMD);
    	if ('-' == tempPtr[0]) {
    		isPlus = FALSE;
    	}
    	// calculate temperature
    	work = (int)(tempPtr[1] - '0') * 10;
    	work += (int)(tempPtr[2] - '0');
    	s_temperature = work;
    	DEBUG_PRINT("s_temperature:%d %d\n", isPlus, s_temperature);
    }
    else if (0 == memcmp(buffer, INFO_STR_CMD, strlen(INFO_STR_CMD))) {
    	tempPtr = buffer + strlen(INFO_STR_CMD);
    	memset(s_information, 0x00, sizeof(s_information));
    	memcpy(s_information, tempPtr, (size - strlen(INFO_STR_CMD)));
    	DEBUG_PRINT("info:%s\n", s_information);
    }
    else {
      /* nothing to do */
    	memcpy(resultBuf, "NG\n", sizeof("NG"));
//      DEBUG_PRINT("CommandNG:%s\n", recvCommandBuffer);
    }
    xprintf(resultBuf);
	return ;
}

