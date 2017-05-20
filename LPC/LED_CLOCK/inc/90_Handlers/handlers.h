/*
 * handlers.h
 *
 *  Created on: 2015/05/03
 *      Author: yohji
 */

#ifndef HANDLERS_HANDLERS_H_
#define HANDLERS_HANDLERS_H_

/* ----- tick ------ */
void SysTick_Handler(void);
inline uint32_t get_tick_time(void);

/* ----- 16bit timer ------ */
void TIMER16_0_IRQHandler(void);

/* ----- I2C ------ */
void I2C_IRQHandler(void);

/* ----- IRQ ------ */
void PIOINT2_IRQHandler(void);

#endif /* HANDLERS_HANDLERS_H_ */
