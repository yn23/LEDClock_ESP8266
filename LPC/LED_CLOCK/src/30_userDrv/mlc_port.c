/********************************************************/
// FILE: mlc_port.c
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
 * @file	mlc_port.c
 * @brief	Matrix LED 制御機能
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */
#include <chip.h>

#include <string.h>
#include "mlc_config.h"
#include "board.h"
#include "mlc_app.h"
#include "mlc_port.h"
#include "mlc_cmn.h"
#include "mlc_img.h"

/*--------------------------------------------------------------------------------------------------
 * Globals
 ---------------------------------------------------------------------------------------------------*/
/* Matrix Led Port Setting */
static const StMlPortSetting s_PortSetting[] =
{
	/* portNo			mode				funcMask		isUsing	*/
	/* ---------- NO.0 -------- */
//	{IOCON_PIO0_0,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO0_1,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	{IOCON_PIO0_2,		IOCON_MODE_PULLUP,	IOCON_FUNC2, 	true,	0 },	/* 16bit compare match timer */
	{IOCON_PIO0_3,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* B(address) */
	{IOCON_PIO0_4,		IOCON_SFI2C_EN,		IOCON_FUNC1, 	true,	0 },	/* iic */
	{IOCON_PIO0_5,		IOCON_SFI2C_EN,		IOCON_FUNC1, 	true,	0 },	/* iic */
//	{IOCON_PIO0_6,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },	/* G2 */
	{IOCON_PIO0_7,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* C(address) */
	{IOCON_PIO0_8,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/*  */
//	{IOCON_PIO0_9,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO0_10,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO0_11,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	/* -------- NO.1 -------- */
	{IOCON_PIO1_0,		IOCON_MODE_INACT,	IOCON_FUNC1, 	true,	0 },	/* CLK2 */
//	{IOCON_PIO1_1,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	{IOCON_PIO1_2,		IOCON_MODE_INACT,	IOCON_FUNC1, 	true,	0 },	/* LAT2 */
//	{IOCON_PIO1_3,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO1_4,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO1_5,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO1_6,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO1_7,		IOCON_MODE_INACT,	IOCON_FUNC0,	false,	0 },
//	{IOCON_PIO1_8,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	{IOCON_PIO1_9,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* DE2 */
//	{IOCON_PIO1_10,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	{IOCON_PIO1_11,		IOCON_MODE_PULLUP,	IOCON_FUNC0 | MD_DIGMODE, 	true,	0 },	/* btn */
	/* -------- NO.2 -------- */
//	{IOCON_PIO2_0,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO2_1,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },
//	{IOCON_PIO2_2,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO2_3,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	{IOCON_PIO2_4,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* DE1 */
	{IOCON_PIO2_5,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* R1 */
	{IOCON_PIO2_6,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* G1 */
	{IOCON_PIO2_7,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* B1 */
//	{IOCON_PIO2_8,		IOCON_MODE_PULLDOWN,	IOCON_FUNC0, 	true,	0 },	/* 外部割り込み */
	{IOCON_PIO2_8,		IOCON_MODE_PULLUP,	IOCON_FUNC0, 	true,	0 },	/* 外部割り込み */
	{IOCON_PIO2_9,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* R2 */
	{IOCON_PIO2_10,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* G2 */
	{IOCON_PIO2_11,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* B2 */
	/* -------- NO.3 -------- */
//	{IOCON_PIO3_0,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
//	{IOCON_PIO3_1,		IOCON_MODE_INACT,	IOCON_FUNC0, 	false,	0 },
	{IOCON_PIO3_2,		IOCON_MODE_PULLUP,	IOCON_FUNC0, 	true,	0 },	/* btn */
	{IOCON_PIO3_3,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* A(address) */
	{IOCON_PIO3_4,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* LAT */
	{IOCON_PIO3_5,		IOCON_MODE_INACT,	IOCON_FUNC0, 	true,	0 },	/* CLK */
};


/**
 * グローバル変数
 */
#ifdef DEBUG_PC
uint32_t programCounter3 = 0;
#endif /* DEBUG_PC */
uint16_t g_FramBuffer1[FRM_BUF_HIGHT][FRM_BUF_WIDE];		/* LED用フレームバッファ1 */
uint16_t g_FramBuffer2[FRM_BUF_HIGHT][FRM_BUF_WIDE];		/* LED用フレームバッファ2 */
int8_t g_FrmBufNum = 0;

/**
 * マクロ定義
 */
/** LED_RのON/OFFを判定 */
#define CNV_COLOR2LOW_HIGH_R(VAL, COLOR_DEPTH)			(((VAL >> 8) & 0x000F) > COLOR_DEPTH) ? 1 : 0
/** LED_GのON/OFFを判定 */
#define CNV_COLOR2LOW_HIGH_G(VAL, COLOR_DEPTH)			(((VAL >> 4) & 0x000F) > COLOR_DEPTH) ? 1 : 0
/** LED_BのON/OFFを判定 */
#define CNV_COLOR2LOW_HIGH_B(VAL, COLOR_DEPTH)			(((VAL)      & 0x000F) > COLOR_DEPTH) ? 1 : 0

/** LED_R1のON/OFFを判定しそのレジスタ値を返却 */
#define CNV_COLOR2LOW_HIGH_R1(VAL, COLOR_DEPTH)			(((VAL >> 8) & 0x000F) > COLOR_DEPTH) ? 0x0020 : 0x0000
/** LED_G1のON/OFFを判定しそのレジスタ値を返却 */
#define CNV_COLOR2LOW_HIGH_G1(VAL, COLOR_DEPTH)			(((VAL >> 4) & 0x000F) > COLOR_DEPTH) ? 0x0040 : 0x0000
/** LED_B1のON/OFFを判定しそのレジスタ値を返却 */
#define CNV_COLOR2LOW_HIGH_B1(VAL, COLOR_DEPTH)			(((VAL)      & 0x000F) > COLOR_DEPTH) ? 0x0080 : 0x0000
/** LED_R2のON/OFFを判定しそのレジスタ値を返却 */
#define CNV_COLOR2LOW_HIGH_R2(VAL, COLOR_DEPTH)			(((VAL >> 8) & 0x000F) > COLOR_DEPTH) ? 0x0200 : 0x0000
/** LED_G2のON/OFFを判定しそのレジスタ値を返却 */
#define CNV_COLOR2LOW_HIGH_G2(VAL, COLOR_DEPTH)			(((VAL >> 4) & 0x000F) > COLOR_DEPTH) ? 0x0400 : 0x0000
/** LED_B2のON/OFFを判定しそのレジスタ値を返却 */
#define CNV_COLOR2LOW_HIGH_B2(VAL, COLOR_DEPTH)			(((VAL)      & 0x000F) > COLOR_DEPTH) ? 0x0800 : 0x0000

/** LED_R1～B2までのレジスタをマスクして落とすための定義 */
#define MASK_LED_PIN	0x0000F11F

/** LED_R1～B2までの出力を制御するレジスタ定義 */
#define LED_OUT_PORT_REG		((volatile unsigned int *)(0x50023FFC))


/*
 * static Function
 */
static void MLC_SetInitPin(void);




/**
 * @fn
 * Matrix LED Clock 出力ピンセッティング
 * @breaf Matrix LED Clock 出力ピンセッティング
 * @param なし
 * @return なし
 * @sa なし
 * @detail なし
 */
static void MLC_SetInitPin(void)
{
	/* DEを最初に初期化 */
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 4, true);
	MLC_DE1_LowHigh(PORT_HIGH);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 1, 9, true);
	MLC_DE2_LowHigh(PORT_HIGH);

	/* その他Direction初期化 */
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 3, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 7, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 8, true);
	
	Chip_GPIO_WriteDirBit(LPC_GPIO, 1, 0, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 1, 2, true);
	
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 1, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 5, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 6, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 7, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 9, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 10, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 11, true);
	
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 3, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 4, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 5, true);

	/* その他Low/High初期化 */
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, 8, false);	/* 計測用 */
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 1, false);	/* 計測用 */

	MLC_R1LowHigh(PORT_LOW);
	MLC_G1LowHigh(PORT_LOW);
	MLC_B1LowHigh(PORT_LOW);
	MLC_R2LowHigh(PORT_LOW);
	MLC_G2LowHigh(PORT_LOW);
	MLC_B2LowHigh(PORT_LOW);

	MLC_Adr_A_LowHigh(PORT_LOW);
	MLC_Adr_B_LowHigh(PORT_LOW);
	MLC_Adr_C_LowHigh(PORT_LOW);

	MLC_ClockLowHigh(PORT_LOW);
	MLC_LatLowHigh(PORT_LOW);

	/* TODO:暫定 */
	memcpy(g_FramBuffer1, globalClock, sizeof(globalClock));
	memcpy(g_FramBuffer2, globalClock, sizeof(globalClock));
//	memcpy(g_FramBuffer1, StrArray0, sizeof(StrArray0));
//	memcpy(g_FramBuffer2, StrArray1, sizeof(StrArray1));
//	memcpy(g_FramBuffer1, StrArray1, sizeof(StrArray1));
//	memcpy(g_FramBuffer1, StrArray2, sizeof(StrArray2));
//	memcpy(g_FramBuffer1, StrArray3, sizeof(StrArray3));

	/* TODO:暫定 */

	return ;
}

/**
 * @fn
 * Matrix LED Clock Port初期化
 * @breaf Matrix LED Clock Port初期化処理
 * @param なし
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_InitPort(void)
{
	int i;

	/* 初期化 */
	for (i = 0; i < sizeof(s_PortSetting)/sizeof(StMlPortSetting); i++) {
		/* 使用するの場合 */
		if (true == s_PortSetting[i].isUsing) {
			/* GPIOセット */
			Chip_IOCON_PinMux(LPC_IOCON, s_PortSetting[i].pinNo, s_PortSetting[i].pinSts, s_PortSetting[i].pinFunc);
		}
	}

	/* PinのDirectionやLowHighの初期値を設定する */
	MLC_SetInitPin();
	return ;
}


/**
 * @fn
 * Matrix LED Clock 出力アドレス設定
 * @breaf Matrix LED Clockの出力ラインの設定を行う
 * @param なし
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_SetAddress(uint16_t address)
{
	bool	adr_a_val = false;
	bool	adr_b_val = false;
	bool	adr_c_val = false;

//	syslog_1(1, "MLC_SetAddress address=%x", address);

#ifdef DEBUG_PC
	programCounter3 = 1;
#endif /* DEBUG_PC */
	/* アドレスの値を取得 */
	adr_a_val = address & 0x0001;
	adr_b_val = address & 0x0002;
	adr_c_val = address & 0x0004;

	/* アドレス設定 */
	MLC_Adr_A_LowHigh(adr_a_val);
	MLC_Adr_B_LowHigh(adr_b_val);
	MLC_Adr_C_LowHigh(adr_c_val);
#ifdef DEBUG_PC
	programCounter3 = 2;
#endif /* DEBUG_PC */

	return ;
}

/**
 * @fn
 * Matrix LED Clock 出力アドレス設定
 * @breaf Matrix LED Clockの出力ラインの設定を行う
 * @param なし
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_SetFrmBufNum(int8_t frmBufNum)
{
	if (0 == frmBufNum) {
		g_FrmBufNum = 0;
	}
	else {
		g_FrmBufNum = 1;
	}
	return ;
}

void MLC_initFrmBufNum(int8_t frmBufNum)
{
	if (0 == frmBufNum) {
		memcpy(g_FramBuffer1, globalClock, sizeof(globalClock));
	}
	else {
		memcpy(g_FramBuffer2, globalClock, sizeof(globalClock));
	}
	return ;
}

void MLC_blankFrmBufNum(int8_t frmBufNum)
{
	if (0 == frmBufNum) {
		memset(g_FramBuffer1, 0x00, sizeof(globalClock));
	}
	else {
		memset(g_FramBuffer2, 0x00, sizeof(globalClock));
	}
	return ;
}


int8_t MLC_GetFrmBufNum(void)
{
	return g_FrmBufNum;
}

uint16_t* MLC_GetFrmBufAdr(void)
{
	uint16_t*		pFrmBuf;

	if (0 == g_FrmBufNum) {
		pFrmBuf = &g_FramBuffer1[0][0];
	}
	else {
		pFrmBuf = &g_FramBuffer2[0][0];
	}
	return pFrmBuf;
}

/**
 * @fn
 * 指定座標塗りつぶし関数
 * @breaf 指定された座標を指定された色で塗りつぶす
 * @param x			座標(横)
 * @param y			座標(縦)
 * @param color_val	x,yで指定された座標に描画する色
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_DrawPoints(int8_t x, int8_t y, uint16_t color_val) {
//	syslog_2(1, "MyPoint2 x=%d y=%d", x, y);

	if (0 == g_FrmBufNum) {
		g_FramBuffer2[y][x] = color_val;
	}
	else {
		g_FramBuffer1[y][x] = color_val;
	}
	return ;
}


/**
 * @fn
 * Matrix LED ON/OFF制御
 * @breaf Matrix LED ClockのLEDの点灯ON/OFFの制御を行う(上側)
 * @param lineNum		：	ライン番号(0～8、16～24)
 * @param colorDepth	：	色情報の深さ(クロック数)
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_DrawLineTop(uint16_t lineNum, uint16_t colorDepth, uint16_t* pFrmBuf)
{
	uint32_t		i = 0;
	uint32_t		tempData;
	uint32_t		regData = 0;
#ifdef DEBUG_PC
	programCounter3 = 3;
#endif /* DEBUG_PC */
	if (NULL != pFrmBuf) {
		/* ドット数分繰り返す */
		do {
#if 0
	//		tempData = *(pFrmBuf + ((lineNum * 32) + i));
			tempData = pFrmBuf[lineNum * 32 + i];
	//		tempData = g_FramBuffer1[lineNum][i];

			MLC_R1LowHigh(CNV_COLOR2LOW_HIGH_R(tempData, colorDepth));
			MLC_G1LowHigh(CNV_COLOR2LOW_HIGH_G(tempData, colorDepth));
			MLC_B1LowHigh(CNV_COLOR2LOW_HIGH_B(tempData, colorDepth));

	//		tempData = *(pFrmBuf + (((lineNum + MAX_ADDRESS) * 32)) + i);
			tempData = pFrmBuf[(MAX_ADDRESS + lineNum) * 32 + i];
	//		tempData = g_FramBuffer1[lineNum + MAX_ADDRESS][i];

			MLC_R2LowHigh(CNV_COLOR2LOW_HIGH_R(tempData, colorDepth));
			MLC_G2LowHigh(CNV_COLOR2LOW_HIGH_G(tempData, colorDepth));
			MLC_B2LowHigh(CNV_COLOR2LOW_HIGH_B(tempData, colorDepth));
#else
			/* マスクしてLEDのビットのみ落とす */
			*LED_OUT_PORT_REG &= MASK_LED_PIN;
			regData = 0;

			/* フレームバッファからデータ読み出し */
			tempData = pFrmBuf[lineNum * 32 + i];
			/* R1G1B1のLED ON/OFF情報を書き込み */
			regData |= CNV_COLOR2LOW_HIGH_R1(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_G1(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_B1(tempData, colorDepth);

			/* フレームバッファからデータ読み出し */
			tempData = pFrmBuf[(MAX_ADDRESS + lineNum) * 32 + i];
			/* R2G2B2のLED ON/OFF情報を書き込み */
			regData |= CNV_COLOR2LOW_HIGH_R2(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_G2(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_B2(tempData, colorDepth);
			/* レジスタに書き戻す */
			*LED_OUT_PORT_REG |= regData;
#endif
			/* Clock入力 */
			MLC_ClockLowHigh(PORT_HIGH);
			MLC_ClockLowHigh(PORT_LOW);
			i++;
		} while (i < 32);
		/* LAT入力 */
		MLC_LatLowHigh(PORT_HIGH);
		MLC_LatLowHigh(PORT_LOW);
	}
#ifdef DEBUG_PC
	programCounter3 = 4;
#endif /* DEBUG_PC */

	return;
}

/**
 * @fn
 * Matrix LED ON/OFF制御
 * @breaf Matrix LED ClockのLEDの点灯ON/OFFの制御を行う(下側)
 * @param lineNum		：	ライン番号(0～8、16～24)
 * @param colorDepth	：	色情報の深さ(クロック数)
 * @return なし
 * @sa なし
 * @detail なし
 */
void MLC_DrawLineBottom(uint16_t lineNum, uint16_t colorDepth, uint16_t* pFrmBuf)
{
	uint32_t		i = 0;
	uint32_t		tempData;
	uint32_t		regData = 0;
#ifdef DEBUG_PC
	programCounter3 = 5;
#endif /* DEBUG_PC */
	if (NULL != pFrmBuf) {

		/* ドット数分繰り返す */
		do {
#if 0
			tempData = pFrmBuf[lineNum * 32 + i];

			MLC_R1LowHigh(CNV_COLOR2LOW_HIGH_R(tempData, colorDepth));
			MLC_G1LowHigh(CNV_COLOR2LOW_HIGH_G(tempData, colorDepth));
			MLC_B1LowHigh(CNV_COLOR2LOW_HIGH_B(tempData, colorDepth));

			tempData = pFrmBuf[(MAX_ADDRESS + lineNum) * 32 + i];

			MLC_R2LowHigh(CNV_COLOR2LOW_HIGH_R(tempData, colorDepth));
			MLC_G2LowHigh(CNV_COLOR2LOW_HIGH_G(tempData, colorDepth));
			MLC_B2LowHigh(CNV_COLOR2LOW_HIGH_B(tempData, colorDepth));
#else
			/* マスクしてLEDのビットのみ落とす */
			*LED_OUT_PORT_REG &= MASK_LED_PIN;
			regData = 0;

			/* フレームバッファからデータ読み出し */
			tempData = pFrmBuf[lineNum * 32 + i];
			/* R1G1B1のLED ON/OFF情報を書き込み */
			regData |= CNV_COLOR2LOW_HIGH_R1(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_G1(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_B1(tempData, colorDepth);

			/* フレームバッファからデータ読み出し */
			tempData = pFrmBuf[(MAX_ADDRESS + lineNum) * 32 + i];
			/* R2G2B2のLED ON/OFF情報を書き込み */
			regData |= CNV_COLOR2LOW_HIGH_R2(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_G2(tempData, colorDepth);
			regData |= CNV_COLOR2LOW_HIGH_B2(tempData, colorDepth);
			/* レジスタに書き戻す */
			*LED_OUT_PORT_REG |= regData;
#endif
			/* Clock入力 */
			MLC_Clock2_LowHigh(PORT_HIGH);
			MLC_Clock2_LowHigh(PORT_LOW);
			i++;
		} while (i < 32);
		/* LAT入力 */
		MLC_Lat2_LowHigh(PORT_HIGH);
		MLC_Lat2_LowHigh(PORT_LOW);
	}
#ifdef DEBUG_PC
	programCounter3 = 6;
#endif /* DEBUG_PC */

	return ;
}

void MLC_PutMark(uint16_t* input_mark, StMlC_Position* pos, int8_t x, int8_t y) {
//	syslog_2(1, "MyPoint2 x=%d y=%d", x, y);
	uint16_t *srcAddr = input_mark;

	if (0 == g_FrmBufNum) {
		for (int j = pos->endPosY; j < pos->endPosY + y; j++) {
			for (int i = pos->endPosX; i < pos->endPosX + x; i++) {
				g_FramBuffer2[j][i] = *srcAddr;
				srcAddr++;
			}
		}
	}
	else {
		for (int j = pos->endPosY; j < pos->endPosY + y; j++) {
			for (int i = pos->endPosX; i < pos->endPosX + x; i++) {
				g_FramBuffer1[j][i] = *srcAddr;
				srcAddr++;
			}
		}
	}
	return ;
}

void set_gpio_interrupt(void)
{

	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 2, 8);

	/* 立ち上がりエッジ */
//	Chip_GPIO_SetupPinInt(LPC_GPIO, 2, 8, GPIO_INT_RISING_EDGE);
	Chip_GPIO_SetupPinInt(LPC_GPIO, 2, 8, GPIO_INT_FALLING_EDGE);

	/* 割り込みマスク設定 */
	Chip_GPIO_EnableInt(LPC_GPIO, 2, (1 << 8));

	/* 割り込み有効 */
	NVIC_EnableIRQ(EINT2_IRQn);

	return ;
}
