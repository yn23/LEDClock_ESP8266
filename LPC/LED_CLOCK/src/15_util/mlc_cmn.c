/********************************************************/
// FILE: mlc_cmn.c
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
 * @file	mlc_cmn.c
 * @brief	Matrix LED 制御機能
 * @author	Y.Narisawa
 * @date 	2015.01.18
 */
#include "mlc_port.h"
#include "mlc_cmn.h"
#include "uart_11xx.h"

void uart_out(uint8_t out_data)
{
//	Chip_UART_SendByte(LPC_USART, out_data);
	Chip_UART_SendBlocking(LPC_USART, &out_data, 1);
	return ;
}

void MLC_DelayMs(int count)
{
    int j=0,i=0;

    for(j=0;j<count;j++)
    {
        /* At 48MHz, the below loop introduces delay of 10 μsec */
        for(i=0;i<780;i++) {
			/* At 48MHz, the below loop introduces delay of 1 μsec */
			asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
			asm("nop");		asm("nop");		asm("nop");
        }
    }
}

/* order to usec */
void MLC_DelayUs(int count)
{
	int j=0;

    for(j=0;j<count;j++)
    {
        /* At 48MHz, the below loop introduces delay of 1 μsec */
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");
    }
}


/* order to usec */
void MLC_Delay10Cycle(int count)
{
	int j=0;

    for(j=0;j<count;j++)
    {
        /* At 48MHz, the below loop introduces delay of 1 μsec */
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    	asm("nop");		asm("nop");		asm("nop");		asm("nop");		asm("nop");
    }
}
