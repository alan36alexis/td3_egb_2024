/**
 ********************************************************************************
 * @file    i2c_slave_bluepill.c
 * @author  julianvroey (TD3 UTN FRA)
 * @date    2024
 * @brief   I2C slave functions for the BluePill
 ********************************************************************************
 */

#include "i2c_slave_bluepill.h"

static uint8_t rxBuffer[rxBUFFER_SIZE], txBuffer[txBUFFER_SIZE];

void i2c_listen_callback(i2c_handler *hi2c) {
	i2c_start_listen(hi2c);
}

void i2c_addr_callback(i2c_handler *hi2c,
		               uint8_t TransferDirection,
					   uint16_t AddrMatchCode) {
	if(I2C_DIRECTION_TRANSMIT == TransferDirection)
		i2c_seq_rx(hi2c, rxBuffer, rxBUFFER_SIZE, I2C_FIRST_AND_LAST_FRAME);
	else {
		i2c_seq_tx(hi2c, txBuffer, txBUFFER_SIZE, I2C_FIRST_AND_LAST_FRAME);
	}
}

void i2c_rx_callback(i2c_handler *hi2c) {
		i2c_slave_rx_process(rxBuffer, rxBUFFER_SIZE);
		memset(rxBuffer,'\0', rxBUFFER_SIZE);
}

void i2c_tx_callback(i2c_handler *hi2c) {
}

void i2c_err_callback(i2c_handler *hi2c) {
	i2c_start_listen(hi2c);
}

void i2c_set_txBuffer(uint8_t* data, uint8_t size) {
	memset(txBuffer,'\0', txBUFFER_SIZE);
	memcpy(txBuffer, data, size);
}
