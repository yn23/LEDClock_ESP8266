/*
 * mlc_img.h
 *
 *  Created on: 2015/02/08
 *      Author: yohji
 */

#ifndef MLC_IMG_H_
#define MLC_IMG_H_

#include <chip.h>

#include "mlc_port.h"

/* 背景 */
extern const uint16_t globalClock[FRM_BUF_HIGHT][FRM_BUF_WIDE];
extern const uint16_t globalRainMark[MARK_HIGHT][MARK_WIDE];
extern const uint16_t globalFineMark[MARK_HIGHT][MARK_WIDE];
extern const uint16_t globalCloudyMark[MARK_HIGHT][MARK_WIDE];

#endif /* MLC_IMG_H_ */
