/********************************************************/
// FILE: mlc_cmn.h
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
 * @file	mlc_cmn.h
 * @brief	Matrix LED Clock Port 制御機能
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */
#ifndef MLC_CMN_H__
#define MLC_CMN_H__

#include "chip.h"
#include "mlc_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if USE_DEBUG_PRINT
#define DEBUG_PUTC(s)   xputc(s)
#define DEBUG_PUTS(s)   xputs(s)
#define DEBUG_PRINT(...)  xprintf(__VA_ARGS__)
#else /* USE_DEBUG_PRINT */
#define PUTC(s)
#define PUTS(s)
#define DEBUG_PRINT(...)
#endif  /* _USE_XFUNC_OUT */


void uart_out(uint8_t out_data);
void MLC_DelayMs(int count);
void MLC_DelayUs(int count);
void MLC_Delay10Cycle(int count);

#define B(b) (\
        ((UINT64_C(0##b)>> 0) & 7 ? (1U<< 0) : 0U) | \
        ((UINT64_C(0##b)>> 3) & 7 ? (1U<< 1) : 0U) | \
        ((UINT64_C(0##b)>> 6) & 7 ? (1U<< 2) : 0U) | \
        ((UINT64_C(0##b)>> 9) & 7 ? (1U<< 3) : 0U) | \
        ((UINT64_C(0##b)>>12) & 7 ? (1U<< 4) : 0U) | \
        ((UINT64_C(0##b)>>15) & 7 ? (1U<< 5) : 0U) | \
        ((UINT64_C(0##b)>>18) & 7 ? (1U<< 6) : 0U) | \
        ((UINT64_C(0##b)>>21) & 7 ? (1U<< 7) : 0U) | \
        ((UINT64_C(0##b)>>24) & 7 ? (1U<< 8) : 0U) | \
        ((UINT64_C(0##b)>>27) & 7 ? (1U<< 9) : 0U) | \
        ((UINT64_C(0##b)>>30) & 7 ? (1U<<10) : 0U) | \
        ((UINT64_C(0##b)>>33) & 7 ? (1U<<11) : 0U) | \
        ((UINT64_C(0##b)>>36) & 7 ? (1U<<12) : 0U) | \
        ((UINT64_C(0##b)>>39) & 7 ? (1U<<13) : 0U) | \
        ((UINT64_C(0##b)>>42) & 7 ? (1U<<14) : 0U) | \
        ((UINT64_C(0##b)>>45) & 7 ? (1U<<15) : 0U) | \
        ((UINT64_C(0##b)>>48) & 7 ? (1U<<16) : 0U) | \
        ((UINT64_C(0##b)>>51) & 7 ? (1U<<17) : 0U) | \
        ((UINT64_C(0##b)>>54) & 7 ? (1U<<18) : 0U) | \
        ((UINT64_C(0##b)>>57) & 7 ? (1U<<19) : 0U) | \
        ((UINT64_C(0##b)>>60) & 7 ? (1U<<20) : 0U) | \
        ((UINT64_C(0##b)>>63) & 7 ? (1U<<21) : 0U) | \
        (0/!((UINT64_C(0##b)) & ~01111111111111111111111 ) ) | \
        ( 0 &sizeof&(b[(char*)0] )) \
\
)

#ifdef __cplusplus
}
#endif

#endif /* MLC_CMN_H__ */
