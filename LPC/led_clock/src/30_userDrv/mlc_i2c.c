/*
 * mlc_i2c.c
 *
 *  Created on: 2015/07/20
 *      Author: yohji
 */
#include <chip.h>
#include "board.h"
#include "mlc_config.h"
#include "mlc_cmn.h"

/*--------------------------------------------------------------------------------------------------
 * Macros
 ---------------------------------------------------------------------------------------------------*/
/* I2C Clock Speed */
#define		I2C_CLOCK_SPEED_400KHZ	(400 * 1000)
#define 	I2C_CLOCK_SPEED_100KHZ  (100000)
/* RTC-8564 Slave Address */
#define		I2C_RTC_ADR				B(1010001)
/* RTC-8564 Read Cmd [Addr] */
#define		I2C_CMD_ADR_0			(0x00)

/*--------------------------------------------------------------------------------------------------
 * Struct
 ---------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------
 * Lable
 ---------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------
 * Globals
 ---------------------------------------------------------------------------------------------------*/
uint8_t		i2c_recv_buffer[16] = {0};
uint8_t		i2c_send_buffer[] = {
									0x00,

									0x20,
									0x11,
									0x00,	// 秒
									0x00,	// 分

									0x00,	// 時
									0x00,	// 日
									0x00,	// 曜日
									0x00,	// 月

									0x00,	// 年
									0x80,	// アラームの分[無効]
									0x80,	// アラームの時[無効]
									0x80,	// アラームの日[無効]

									0x80,	// アラームの曜日[無効]
									0x83,	// CLKOUT
									0x00,	// TimerControl[無効]
									0x00	// Timer[無効]
								  };
uint8_t		i2c_send_buffer_reset[2] = {0x00, 0x00};

/*--------------------------------------------------------------------------------------------------
 * 割り込みハンドラ
 ---------------------------------------------------------------------------------------------------*/
/* State machine handler for I2C0 and I2C1 */
void i2c_state_handling(I2C_ID_T id)
{
	if (Chip_I2C_IsMasterActive(id)) {
		Chip_I2C_MasterStateHandler(id);
	} else {
		Chip_I2C_SlaveStateHandler(id);
	}
}

/*--------------------------------------------------------------------------------------------------
 * 通常Function
 ---------------------------------------------------------------------------------------------------*/
/**
 * @brief	I2C Init Function
 * @return	None
 */
void i2c_init(void)
{
	int32_t		ret = 0;

	/* ペリフェラルリセット(重要!!!!!) */
	Chip_SYSCTL_PeriphReset(RESET_I2C0);

	/* i2cの初期化 */
	Chip_I2C_Init(I2C0);
	/* clock設定 */
	Chip_I2C_SetClockRate(I2C0, I2C_CLOCK_SPEED_100KHZ);

	/* イベントハンドラ登録 */
	Chip_I2C_SetMasterEventHandler(I2C0, Chip_I2C_EventHandler);
	NVIC_EnableIRQ(I2C0_IRQn);

	/* データ読み込み */
	ret = Chip_I2C_MasterCmdRead(I2C0, I2C_RTC_ADR, I2C_CMD_ADR_0, &i2c_recv_buffer[0], sizeof(i2c_recv_buffer));
	if (ret > 0) {
		/* パラメタチェック */
		/* データ設定 */
		ret = Chip_I2C_MasterSend(I2C0, I2C_RTC_ADR, &i2c_send_buffer[0], sizeof(i2c_send_buffer));
		if (ret > 0) {
			ret = Chip_I2C_MasterSend(I2C0, I2C_RTC_ADR, &i2c_send_buffer_reset[0], sizeof(i2c_send_buffer_reset));
		}
		else {
			while(1);
		}
	}

	//int Chip_I2C_MasterSend(I2C_ID_T id, uint8_t slaveAddr, const uint8_t *buff, uint8_t len)
	return ;
}
