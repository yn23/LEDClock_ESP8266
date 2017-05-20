/********************************************************/
// FILE: mlc_config.h
// DESCRIPTION: Matrix LED Clock Draw
// Copyright (CC) 2015.02.07 Y.Narisawa
/********************************************************/
// This work is licensed under the Creative Commons
// Attribution 4.0 Unported License.
// To view a copy of this license, visit
// http://creativecommons.org/licenses/by/4.0/
// Good Luck
/********************************************************/
/**
 * @file	mlc_app.h
 * @brief	Matrix LED Clock 設定ファイル
 * @author	Y.Narisawa
 * @date 	2015.02.07
 */
#ifndef MLC_DRAW_CLOCK_H_
#define MLC_DRAW_CLOCK_H_

#include <chip.h>

#define CLOCK_SECOND	60	/* 最大値：秒 */
#define CLOCK_MIN		60	/* 最大値：分 */

#define DATE_LENGTH		6	/* 日付 */
#define TIME_LENGTH		5	/* 時間 */
#define TMP_LENGTH		4	/* 温度 */


/* MatrixLedClockPostion */
typedef struct _StMlC_Pos {
	uint8_t				endPosX;		/* this pin is using (YES = ture/ No = false) */
	uint8_t				endPoxY;		/* padding */
	uint16_t			colorVal;		/* padding */
} StMlC_Position;

void MLC_DrawLineControl_S(uint32_t count);
void MLC_DrawLineControl_M(uint32_t count);
void MLC_DrawLineControl_H(uint32_t count);

void MLC_DrawDate(uint8_t* date, int length);
void MLC_DrawTime(bool is_am, int32_t hour, int32_t min, int32_t sec);
void MLC_DrawTemp(int32_t temp, bool is_plus);

void MLC_String(StMlC_Position* position, uint8_t* date, int length);
void MLC_DrawChar(StMlC_Position* position, uint8_t ch);

#endif /* MLC_DRAW_CLOCK_H_ */
