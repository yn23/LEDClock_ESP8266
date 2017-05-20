
/********************************************************/
// FILE: mlc_port.h
// DESCRIPTION: Matrix LED Clock Port
// Copyright (CC) 2015.01.18 Y.Narisawa
/********************************************************/
// This work is licensed under the Creative Commons 
// Attribution 4.0 Unported License. 
// To view a copy of this license, visit 
// http://creativecommons.org/licenses/by/4.0/
// Good Luck
/********************************************************/
/**
 * @file	mlc_port.h
 * @brief	Matrix LED Clock Port 制御機能
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */
#ifndef MLC_PORT_H__
#define MLC_PORT_H__

#include <00_chip/chip.h>

extern RINGBUFF_T txring, rxring;

/* マクロ定義 */
#define PORT_HIGH		true
#define PORT_LOW		false

#define MAX_ADDRESS		8	/* 最大アドレス値 */
#define FRM_BUF_WIDE	32	/* フレームバッファ横幅*/
#define FRM_BUF_HIGHT	32	/* フレームバッファ縦幅 */

/* MatrixLedPort */
typedef struct _StMlPortSetting {
	CHIP_IOCON_PIO_T	pinNo;		/* PortNo */
	uint8_t				pinSts;		/* PinStatus(Pullup/down/etc...) */
	uint8_t				pinFunc;	/* PinFunction(UART/GPIO/SPI/etc...) */
	uint8_t				isUsing;	/* this pin is using (YES = ture/ No = false) */
	uint8_t				padding;	/* padding */
} StMlPortSetting;

#ifdef __cplusplus
extern "C" {
#endif

/* ポート初期化関数 */
void MLC_InitPort(void);


/* Matrix LED Clock 出力ラインアドレス設定機能 */
void MLC_SetAddress(uint16_t address);

/**
 *  描画API
 */
void MLC_DrawPoints(int8_t x, int8_t y, uint16_t color_val);
void MLC_DrawLineTop(uint16_t lineNum, uint16_t colorDepth, uint16_t* pFrmBuf);
void MLC_DrawLineBottom(uint16_t lineNum, uint16_t colorDepth, uint16_t* pFrmBuf);
int8_t MLC_GetFrmBufNum(void);
void MLC_SetFrmBufNum(int8_t frmBufNum);
uint16_t* MLC_GetFrmBufAdr(void);
void MLC_initFrmBufNum(int8_t frmBufNum);

/* RGB制御関数 */
static inline void MLC_R1LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 5, param);
}
static inline void MLC_G1LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 6, param);
}
static inline void MLC_B1LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 7, param);
}
static inline void MLC_R2LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 9, param);
}
static inline void MLC_G2LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 10, param);
}
static inline void MLC_B2LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 11, param);
}

/* Address制御関数 */
static inline void MLC_Adr_A_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 3, param);
}
static inline void MLC_Adr_B_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, 3, param);
}
static inline void MLC_Adr_C_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, 7, param);
}

/* Clock制御関数 */
static inline void MLC_ClockLowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 5, param);
}
/* LAT制御関数 */
static inline void MLC_LatLowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 4, param);
}
/* DE制御関数 */
static inline void MLC_DE1_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 4, param);
}
/* Clock制御関数 */
static inline void MLC_Clock2_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 1, 0, param);
}
/* LAT制御関数 */
static inline void MLC_Lat2_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 1, 2, param);
}
static inline void MLC_DE2_LowHigh( bool param ) {
	Chip_GPIO_WritePortBit(LPC_GPIO, 1, 9, param);
}

/**
 *  
 *  other function
 *  
 */
/* コンペアマッチタイマ初期化 */
void cmt_timer16_init(uint32_t timer_num);
/* GPIO割り込み設定 */
void set_gpio_interrupt(void);
/* I2Cの初期化 */
void i2c_init(void);
/* I2Cの割り込みこみ処理 */
void i2c_state_handling(I2C_ID_T id);
 
#ifdef __cplusplus
}
#endif


#endif	/* MLC_PORT_H__ */
