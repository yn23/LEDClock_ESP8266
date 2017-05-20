/********************************************************/
// FILE: mlc_app.h
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
 * @brief	Matrix LED Clock Port 制御機能
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */
#ifndef MLC_APP_H__
#define MLC_APP_H__

#include <chip.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	APLMODE_AnalogClock,
	APLMODE_DigitalClock,
} eAplMode;

typedef enum {
	APLWeather_Fine = 1,
} eAplWeather;

void MLC_LedControlMain( uint32_t exinf );
void MLC_AnalogClockMain( void );
void MLC_MainAppInit( void );
void MLC_1sCounter( void );
void MLC_1mCounter( void );
void sys_40ms_counter( void );
void MLC_LedOnTimeUp( void );
eAplMode MLC_PrepareModeCheck( void );
int32_t MLC_GetCommand( char* buffer, int size );
void MLC_ParseCommand( char* buffer, int size );

#ifdef __cplusplus
}
#endif

#endif /* MLC_APP_H__ */
