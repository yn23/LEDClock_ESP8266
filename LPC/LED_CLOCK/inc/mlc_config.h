/********************************************************/
// FILE: mlc_config.h
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
 * @file	mlc_app.h
 * @brief	Matrix LED Clock 設定ファイル
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */
#ifndef MLC_CONFIG_H__
#define MLC_CONFIG_H__

/* アプリケーション全体に影響する定義 */
#define USE_DEBUG_PRINT			     (1)
#define MLC_MAX_COLOR_DEPTH			(12)		/* カラー情報の深さ */
#define MLC_LED_OUTPUT_MAX			(15)		/* マトリクスLEDを点灯させている時間(us) */
/* UART */
#define UART_RB_SIZE (32)		// Due to ring buffer implementation, UART_RB_SIZE must be a number of 2 ^ n

#endif /* MLC_CONFIG_H__ */
