/********************************************************/
// FILE: mlc_draw_clock.c
// DESCRIPTION: Matrix LED Control
// Copyright (CC) 2015.02.07 Y.Narisawa
/********************************************************/
// This work is licensed under the Creative Commons
// Attribution 4.0 Unported License.
// To view a copy of this license, visit
// http://creativecommons.org/licenses/by/4.0/
// Good Luck
/********************************************************/
/**
 * @file	mlc_app.c
 * @brief	Matrix LED Clock needle Draw
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */

#include "string.h"
#include "font.h"
#include "mlc_app.h"
#include "mlc_draw_clock.h"
#include "mlc_port.h"
#include "mlc_img.h"

#ifdef DEBUG_PC
uint8_t programCounter2 = 0;
#endif /* DEBUG_PC */


uint8_t s_lastDate[DATE_LENGTH];
int32_t s_lastWriteTime = 0;
int32_t s_lastWriteTmp = 0;

/**
 * 時計「秒」描画用ポジションテーブル
 */
const StMlC_Position clock_S_potion[CLOCK_SECOND] = {
	/* 0～10 */
#if 0
	{ 15,  0, 0x0F00, },
	{ 17,  0, 0x0F00, },
	{ 19,  0, 0x0F00, },

	{ 21,  0, 0x00F0, },
	{ 22,  0, 0x00F0, },
	{ 24,  0, 0x00F0, },

	{ 28,  0, 0x000F, },
	{ 31,  0, 0x000F, },
	{ 31,  2, 0x000F, },

	{ 31,  4, 0x0FF0, },
	{ 31,  8, 0x0FF0, },
	/* 11～20 */
	{ 31, 9,  0x0FF0, },

	{ 31, 11, 0x00FF, },
	{ 31, 13, 0x00FF, },
	{ 31, 14, 0x00FF, },

	{ 31, 16, 0x0F0F, },
	{ 31, 18, 0x0F0F, },
	{ 31, 19, 0x0F0F, },

	{ 31, 21, 0x02F2, },
	{ 31, 22, 0x02F2, },
	{ 31, 24, 0x02F2, },

	/* 21～30 */
	{ 31, 27, 0x044F, },
	{ 31, 29, 0x044F, },
	{ 31, 31, 0x044F, },

	{ 28, 31, 0x04F0, },
	{ 23, 31, 0x04F0, },
	{ 21, 31, 0x04F0, },

	{ 19, 31, 0x0555, },
	{ 17, 31, 0x0555, },
	{ 16, 31, 0x0555, },

	{ 15, 31, 0x0124, },
	/* 31～40 */
	{ 13, 31, 0x0124, },
	{ 12, 31, 0x0124, },

	{ 10, 31, 0x0FF6, },
	{  9, 31, 0x0FF6, },
	{  7, 31, 0x0FF6, },

	{  3, 31, 0x0C0C, },
	{  0, 31, 0x0C0C, },
	{  0, 29, 0x0C0C, },

	{  0, 27, 0x06B2, },
	{  0, 24, 0x06B2, },
	/* 41～50 */
	{  0, 22, 0x06B2, },

	{  0, 21, 0x0777, },
	{  0, 19, 0x0777, },
	{  0, 18, 0x0777, },

	{  0, 16, 0x08C4, },
	{  0, 14, 0x08C4, },
	{  0, 13, 0x08C4, },

	{  0, 11, 0x07F7, },
	{  0, 10, 0x07F7, },
	{  0,  8, 0x07F7, },
	/* 51～60 */
	{  0,  4, 0x0A25, },
	{  0,  2, 0x0A25, },
	{  0,  0, 0x0A25, },

	{  3,  0, 0x012A, },
	{  6,  0, 0x012A, },
	{  8,  0, 0x012A, },

	{ 10,  0, 0x0690, },
	{ 11,  0, 0x0690, },
	{ 13,  0, 0x0690, },
#else
	/* 0～9 */
	{ 16, 0, 0x0690, },
	{ 18, 0, 0x0690, },
	{ 19, 0, 0x0690, },
	{ 21, 0, 0x0690, },
	{ 22, 0, 0x0690, },

	{ 24, 0, 0x012A, },
	{ 26, 1, 0x012A, },
	{ 28, 2, 0x012A, },
	{ 29, 3, 0x012A, },
	{ 30, 6, 0x012A, },

	/* 10～19 */
	{ 31, 8, 0x0A25, },
	{ 31, 10, 0x0A25, },
	{ 31, 11, 0x0A25, },
	{ 31, 13, 0x0A25, },
	{ 31, 14, 0x0A25, },

	{ 31, 16, 0x07F7, },
	{ 31, 18, 0x07F7, },
	{ 31, 19, 0x07F7, },
	{ 31, 21, 0x07F7, },
	{ 31, 22, 0x07F7, },

	/* 20～29 */
	{ 31, 24, 0x08C4, },
	{ 30, 26, 0x08C4, },
	{ 29, 28, 0x08C4, },
	{ 28, 29, 0x08C4, },
	{ 26, 30, 0x08C4, },

	{ 23, 31, 0x0777, },
	{ 21, 31, 0x0777, },
	{ 20, 31, 0x0777, },
	{ 19, 31, 0x0777, },
	{ 17, 31, 0x0777, },

	/* 30～39 */
	{ 16, 31, 0x06B2, },
	{ 14, 31, 0x06B2, },
	{ 12, 31, 0x06B2, },
	{ 11, 31, 0x06B2, },
	{ 9, 31, 0x06B2, },

	{ 8, 31, 0x0C0C, },
	{ 6, 30, 0x0C0C, },
	{ 4, 29, 0x0C0C, },
	{ 2, 28, 0x0C0C, },
	{ 1, 26, 0x0C0C, },

	/* 40～49 */
	{ 0, 24, 0x0FF6, },
	{ 0, 22, 0x0FF6, },
	{ 0, 21, 0x0FF6, },
	{ 0, 19, 0x0FF6, },
	{ 0, 18, 0x0FF6, },

	{ 0, 16, 0x0124, },
	{ 0, 14, 0x0124, },
	{ 0, 13, 0x0124, },
	{ 0, 11, 0x0124, },
	{ 0, 10, 0x0124, },

	/* 50～59 */
	{ 0, 8, 0x0555, },
	{ 1, 6, 0x0555, },
	{ 2, 4, 0x0555, },
	{ 3, 3, 0x0555, },
	{ 5, 1, 0x0555, },

	{ 7, 0, 0x04F0, },
	{ 9, 0, 0x04F0, },
	{ 10, 0, 0x04F0, },
	{ 12, 0, 0x04F0, },
	{ 14, 0, 0x04F0, },
#endif
};

/**
 * 時計「分」描画用ポジションテーブル
 */
const StMlC_Position clock_M_potion[CLOCK_SECOND] = {
#if 0
	/* 0～10 */
	{ 15,  5, 0x0F00, },
	{ 17,  5, 0x0F00, },
	{ 19,  5, 0x0F00, },

	{ 20,  5, 0x00F0, },
	{ 22,  5, 0x00F0, },
	{ 23,  5, 0x00F0, },

	{ 25,  5, 0x000F, },
	{ 27,  5, 0x000F, },
	{ 28,  6, 0x000F, },

	{ 28,  7, 0x0FF0, },
	{ 28,  8, 0x0FF0, },
	/* 11～20 */
	{ 28, 10,  0x0FF0, },

	{ 28, 12, 0x00FF, },
	{ 28, 13, 0x00FF, },
	{ 28, 14, 0x00FF, },

	{ 28, 16, 0x0F0F, },
	{ 28, 18, 0x0F0F, },
	{ 28, 19, 0x0F0F, },

	{ 28, 21, 0x02F2, },
	{ 28, 23, 0x02F2, },
	{ 28, 24, 0x02F2, },

	/* 21～30 */
	{ 28, 25, 0x044F, },
	{ 27, 26, 0x044F, },
	{ 26, 26, 0x044F, },

	{ 25, 26, 0x04F0, },
	{ 23, 26, 0x04F0, },
	{ 21, 26, 0x04F0, },

	{ 20, 26, 0x0555, },
	{ 19, 26, 0x0555, },
	{ 17, 26, 0x0555, },

	{ 15, 26, 0x0124, },
	/* 31～40 */
	{ 13, 26, 0x0124, },
	{ 12, 26, 0x0124, },

	{ 11, 26, 0x0FF6, },
	{  9, 26, 0x0FF6, },
	{  8, 26, 0x0FF6, },

	{  7, 26, 0x0C0C, },
	{  6, 26, 0x0C0C, },
	{  5, 25, 0x0C0C, },

	{  5, 24, 0x06B2, },
	{  5, 23, 0x06B2, },
	/* 41～50 */
	{  5, 22, 0x06B2, },

	{  5, 21, 0x0777, },
	{  5, 19, 0x0777, },
	{  5, 18, 0x0777, },

	{  5, 16, 0x08C4, },
	{  5, 14, 0x08C4, },
	{  5, 13, 0x08C4, },

	{  5, 12, 0x07F7, },
	{  5, 10, 0x07F7, },
	{  5,  8, 0x07F7, },
	/* 51～60 */
	{  5,  7, 0x0A25, },
	{  5,  6, 0x0A25, },
	{  6,  6, 0x0A25, },

	{  6,  5, 0x012A, },
	{  7,  5, 0x012A, },
	{  8,  5, 0x012A, },

	{ 10,  5, 0x0690, },
	{ 12,  5, 0x0690, },
	{ 13,  5, 0x0690, },
#else
	/* 0～9 */
	{ 16, 5, 0x0F00, },
	{ 17, 5, 0x0F00, },
	{ 19, 5, 0x0F00, },
	{ 20, 5, 0x0F00, },
	{ 22, 5, 0x0F00, },

	{ 23, 5, 0x00F0, },
	{ 24, 5, 0x00F0, },
	{ 25, 5, 0x00F0, },
	{ 26, 6, 0x00F0, },
	{ 27, 7, 0x00F0, },

	/* 10～19 */
	{ 28, 8, 0x000F, },
	{ 28, 10, 0x000F, },
	{ 28, 12, 0x000F, },
	{ 28, 13, 0x000F, },
	{ 28, 14, 0x000F, },

	{ 28, 16, 0x0FF0, },
	{ 28, 18, 0x0FF0, },
	{ 28, 19, 0x0FF0, },
	{ 28, 21, 0x0FF0, },
	{ 28, 22, 0x0FF0, },

	/* 20～29 */
	{ 28, 23, 0x00FF, },
	{ 27, 24, 0x00FF, },
	{ 26, 25, 0x00FF, },
	{ 25, 26, 0x00FF, },
	{ 24, 26, 0x00FF, },

	{ 23, 26, 0x0F0F, },
	{ 21, 26, 0x0F0F, },
	{ 20, 26, 0x0F0F, },
	{ 19, 26, 0x0F0F, },
	{ 17, 26, 0x0F0F, },

	/* 30～39 */
	{ 16, 26, 0x02F2, },
	{ 14, 26, 0x02F2, },
	{ 12, 26, 0x02F2, },
	{ 11, 26, 0x02F2, },
	{ 9, 26, 0x02F2, },

	{ 8, 26, 0x044F, },
	{ 7, 26, 0x044F, },
	{ 6, 25, 0x044F, },
	{ 5, 24, 0x044F, },
	{ 4, 23, 0x044F, },

	/* 40～49 */
	{ 4, 22, 0x04F0, },
	{ 4, 20, 0x04F0, },
	{ 4, 19, 0x04F0, },
	{ 4, 18, 0x04F0, },
	{ 4, 17, 0x04F0, },

	{ 4, 16, 0x0F00, },
	{ 4, 14, 0x0F00, },
	{ 4, 13, 0x0F00, },
	{ 4, 12, 0x0F00, },
	{ 5, 10, 0x0F00, },

	/* 50～59 */
	{ 5, 9, 0x0F00, },
	{ 5, 8, 0x0555, },
	{ 5, 7, 0x0555, },
	{ 6, 6, 0x0555, },
	{ 7, 5, 0x0555, },

	{ 8, 5, 0x0124, },
	{ 10, 5, 0x0124, },
	{ 12, 5, 0x0124, },
	{ 13, 5, 0x0124, },
	{ 14, 5, 0x0124, },
#endif
};

/* 指定座標塗りつぶし関数 */
// static void MLC_DrawPoints(int8_t x, int8_t y, uint16_t color_val);
/* 針描画座標算出関数(using Bresenham algorithm) */
static void MLC_DrawLineByBresenham(int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint16_t color_val, bool erase_flg);

char tempBuffer[64];
/**
 * @fn
 * 針描画座標算出関数(using Bresenham algorithm)
 * @breaf 針描画座標を算出し塗りつぶしを行う機能
 * @param x1		開始座標(横)
 * @param y1		開始座標(縦)
 * @param x2		終了座標(横)
 * @param y2		終了座標(縦)
 * @param color_val	塗りつぶしの色
 * @param erase_flg	true:背景で塗りつぶす / false:指定した色で塗りつぶす
 * @return なし
 * @sa なし
 * @detail なし
 */
static void MLC_DrawLineByBresenham(
	int8_t x1,
	int8_t y1,
	int8_t x2,
	int8_t y2,
	uint16_t color_val,
	bool erase_flg
)
{
	int8_t dx,dy,sx,sy,a,a1,e,i;

    if(x1 <= x2) {
        dx = x2 - x1, sx = 1;
    }
    else {
        dx = x1 - x2, sx = -1;
    }
    if(y1 <= y2) {
        dy = y2 - y1, sy = 1;
    }
    else {
        dy = y1 - y2, sy = -1;
    }

    if(dx >= dy)
    {
        a  = 2 * dy;
        a1 = a - 2 * dx;
        e  = a - dx;

        for(i = 0; i < 64; i++)
        {
        	if (erase_flg) {
#ifdef DEBUG_PC
        		programCounter2 = 1;
#endif /* DEBUG_PC */
				MLC_DrawPoints(x1, y1, globalClock[y1][x1]);
#ifdef DEBUG_PC
        		programCounter2 = 2;
#endif /* DEBUG_PC */
        	}
        	else {
//        		xsprintf(tempBuffer, "x1=%d y1=%d", x1, y1);
#ifdef DEBUG_PC
        		programCounter2 = 3;
#endif /* DEBUG_PC */
        		MLC_DrawPoints(x1, y1, color_val);
#ifdef DEBUG_PC
        		programCounter2 = 4;
#endif /* DEBUG_PC */
        	}
            if(x1 == x2) break;

            if(e >= 0) y1 += sy, e += a1;
            else e += a;

            x1 += sx;
        }
    }
    else
    {
        a  = 2 * dx;
        a1 = a - 2 * dy;
        e  = a - dy;

        for(i = 0; i < 64; i++)
        {
        	if (erase_flg) {
#ifdef DEBUG_PC
        		programCounter2 = 5;
#endif /* DEBUG_PC */
        		MLC_DrawPoints(x1, y1, globalClock[y1][x1]);
#ifdef DEBUG_PC
        		programCounter2 = 6;
#endif /* DEBUG_PC */
        	}
        	else {
#ifdef DEBUG_PC
        		programCounter2 = 7;
#endif /* DEBUG_PC */
        		MLC_DrawPoints(x1, y1, color_val);
#ifdef DEBUG_PC
        		programCounter2 = 8;
#endif /* DEBUG_PC */
        	}

            if(y1 == y2) break;

            if(e >= 0) x1 += sx, e += a1;
            else e += a;

            y1 += sy;
        }
    }
}

/**
 * @fn
 * 時計の針「秒」描画関数
 * @breaf 時計の針「秒」の描画を行う機能
 * @param count 秒数(0～3600)
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_DrawLineControl_M(uint32_t count) {
	int8_t		start_x = 16;
	int8_t		start_y = 16;
	int8_t		end_x = 0;
	int8_t		end_y = 0;
	uint16_t	color_val = 0x0FFF;

//	syslog_1(1, "drawline count_s=%d", count);
	if (0 == count) {
		end_x = clock_S_potion[CLOCK_SECOND - 1].endPosX;
		end_y = clock_S_potion[CLOCK_SECOND - 1].endPoxY;
	}
	else {
		end_x = clock_S_potion[count - 1].endPosX;
		end_y = clock_S_potion[count - 1].endPoxY;
	}
	/* 前回の針の位置を消去する */
	MLC_DrawLineByBresenham(start_x, start_y, end_x, end_y, 0x0000, true);

	/* 秒針描画 */
	end_x = clock_S_potion[count].endPosX;
	end_y = clock_S_potion[count].endPoxY;
	color_val = clock_S_potion[count].colorVal;

	/* 今回の針の位置を描画する */
	MLC_DrawLineByBresenham(start_x, start_y, end_x, end_y, color_val, false);
}

/**
 * @fn
 * 時計の針「分」描画関数
 * @breaf 時計の針「分」の描画を行う機能
 * @param count 	秒数(0～60)
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_DrawLineControl_H(uint32_t count) {
	int8_t		start_x = 16;
	int8_t		start_y = 16;
	int8_t		end_x = 0;
	int8_t		end_y = 0;
	uint16_t	color_val = 0x0FFF;

//	syslog_1(1, "drawline count_m=%d", count);
	if (0 == count) {
		end_x = clock_M_potion[CLOCK_SECOND - 1].endPosX;
		end_y = clock_M_potion[CLOCK_SECOND - 1].endPoxY;
	}
	else {
		end_x = clock_M_potion[count - 1].endPosX;
		end_y = clock_M_potion[count - 1].endPoxY;
	}
	/* 前回の針の位置を消去する */
	MLC_DrawLineByBresenham(start_x, start_y, end_x, end_y, 0x0000, false);

	/* 分を描画 */
	end_x = clock_M_potion[count].endPosX;
	end_y = clock_M_potion[count].endPoxY;
	color_val = clock_M_potion[count].colorVal;

	/* 今回の針の位置を描画する */
	MLC_DrawLineByBresenham(start_x, start_y, end_x, end_y, color_val, false);
}


/**
 * @fn
 * 時計の針「分」描画関数
 * @breaf 時計の針「分」の描画を行う機能
 * @param count 	秒数(0～60)
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_DrawLineControl_S(uint32_t count) {
	int32_t		end_x = 0;
	int32_t		end_y = 0;
	int32_t		i = 0;
	uint16_t	color_val = 0x0FFF;

//	syslog_1(1, "drawline count_s=%d", count);
#if 0
	if (0 == count) {
		end_x = clock_S_potion[CLOCK_SECOND - 2].endPosX;
		end_y = clock_S_potion[CLOCK_SECOND - 2].endPoxY;
	}
	else if (1 == count) {
		end_x = clock_S_potion[CLOCK_SECOND - 1].endPosX;
		end_y = clock_S_potion[CLOCK_SECOND - 1].endPoxY;
	}
	else {
		end_x = clock_S_potion[count - 2].endPosX;
		end_y = clock_S_potion[count - 2].endPoxY;
	}
	color_val = globalClock[end_y][end_x];
	/* 前回の針の位置を消去する */
	MLC_DrawPoints(end_x, end_y, color_val);

	/* 分を描画 */
	end_x = clock_S_potion[count].endPosX;
	end_y = clock_S_potion[count].endPoxY;
	color_val = 0x0FFF;

	/* 今回の針の位置を描画する */
	MLC_DrawPoints(end_x, end_y, color_val);
#else
	if ((0 == count) || (1 == count)) {
		for (i = 0; i < CLOCK_SECOND; i++) {
			/* ポジションを取得 */
			end_x = clock_S_potion[i].endPosX;
			end_y = clock_S_potion[i].endPoxY;
			/* 背景色取得 */
			color_val = globalClock[end_y][end_x];
			/* 前回の針の位置を消去する */
			MLC_DrawPoints(end_x, end_y, color_val);
		}
	}
	for (i = 0; i < count; i++) {
		/* ポジションを取得 */
		end_x = clock_S_potion[i].endPosX;
		end_y = clock_S_potion[i].endPoxY;
		/* 背景色取得 */
		color_val = globalClock[end_y][end_x];
		if (0x0000 == color_val) {
			/* 秒の色設定 */
			color_val = 0x0FFF;
		}
		/* 今回の位置まで描画 */
		MLC_DrawPoints(end_x, end_y, color_val);
	}
#endif
}


/* ------- digital clock functions ------- */
void MLC_DrawDate(uint8_t* date, int length) {
	StMlC_Position	datePos[DATE_LENGTH] = {
		{1,  0, 0x0F00},		// year[left]
		{6,  0, 0x00CC},		// year[right]
		{11, 0, 0x00F0},		// month[left]
		{16, 0, 0x0FF0},		// month[right]
		{21, 0, 0x0FFF},		// date[left]
		{26, 0, 0x0A25},		// date[right]
	};

	if (date) {
		if (length == DATE_LENGTH) {
			MLC_String(datePos, date, length);
		}
	}
}

void MLC_DrawTime(bool is_am, int32_t hour, int32_t min, int32_t sec) {
	uint8_t	tempTime[TIME_LENGTH];
	StMlC_Position	timePos[TIME_LENGTH] = {
		{6,  8, 0x0A25},		// year[right]
		{11, 8, 0x0FF0},		// month[left]
		{16, 8, 0x000F},		// month[right]
		{21, 8, 0x00F0},		// date[left]
		{26, 8, 0x00CC},		// date[right]
	};
	hour = hour / 6;
	if (false == is_am) {
		hour += 12;
	}
	tempTime[0] = (hour / 10) + 0x30;
	tempTime[1] = (hour % 10) + 0x30;
	if (0 == (sec % 2)) {
		tempTime[2] = ':';
	}
	else {
		tempTime[2] = 0x00;
	}
	min = min % CLOCK_SECOND;
	tempTime[3] = (min / 10) + 0x30;
	tempTime[4] = (min % 10) + 0x30;
	s_lastWriteTime = sec;
	MLC_String(timePos, tempTime, TIME_LENGTH);
}

void MLC_String(StMlC_Position* position, uint8_t* date, int length) {
	uint8_t i = 0;
	uint8_t* ch;
	StMlC_Position*	tempPtr = NULL;

	for (i = 0; i < length; i ++) {
		tempPtr = position + i;
		ch = date + i;
		MLC_DrawChar(tempPtr, *ch);
	}
}

void MLC_DrawTemp(int32_t temp, bool is_plus){
	uint8_t	tempTmp[TMP_LENGTH];
	StMlC_Position	tempPos[TMP_LENGTH] = {
		{11, 16, 0x00CC},		// month[left]
		{16, 16, 0x0A25},		// temp[right]
		{21, 16, 0x0FF0},		// temp[left]
		{26, 16, 0x0FFF},		// right[right]
	};
	if (is_plus) {
		tempTmp[0] = 0x00;
	}
	else {
		tempTmp[0] = '-';
	}
	tempTmp[1] = (temp / 10) + 0x30;
	tempTmp[2] = (temp % 10) + 0x30;
	tempTmp[3] = 'C';
	s_lastWriteTmp = temp;
	MLC_String(tempPos, tempTmp, TMP_LENGTH);
}

void MLC_DrawChar(StMlC_Position* position, uint8_t ch) {
	int x, y;
	uint8_t data;
	uint8_t point;

	if ((NULL != position) && (FONT_MIN <= ch) && (FONT_MAX >= ch)) {
		for (x = 0; x < FONT_SIZE_X; x++) {
			data = Font[ch - FONT_MIN][x];
			for (y = 0; y < FONT_SIZE_Y; y++) {
				point = ((data >> y) & 0x01);
				if (0 != point) {
					MLC_DrawPoints(position->endPosX + x, position->endPoxY + y, position->colorVal);
				}
			}
		}
	}
}














