/**
 ********************************************************************************
 * @file    i2c_slave_bluepill.h
 * @author  julianvroey (TD3 UTN FRA)
 * @date    2024
 * @brief   I2C slave header file for the BluePill
 ********************************************************************************
 */

#ifndef I2C_SLAVE_BLUEPILL
#define I2C_SLAVE_BLUEPILL

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdio.h>
#include <string.h>

#define rxBUFFER_SIZE 128
#define txBUFFER_SIZE 128

#define i2c_handler I2C_HandleTypeDef

#define AF_ERROR 4
#define BERR_ERROR 1

/**
 * @brief  Enable the Address listen mode with Interrupt.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval HAL status
 */
#define i2c_start_listen(pHandler) HAL_I2C_EnableListen_IT(pHandler)

#define i2c_start_listen_check(pHandler) if(HAL_I2C_EnableListen_IT(pHandler) != HAL_OK)\
								   	     	 Error_Handler()

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
#define i2c_listen_callback HAL_I2C_ListenCpltCallback

/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection Master request Transfer Direction (Write/Read), value of @ref I2C_XferDirection_definition
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
#define i2c_addr_callback HAL_I2C_AddrCallback

/**
  * @brief  Sequential receive in slave mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
#define i2c_seq_rx HAL_I2C_Slave_Seq_Receive_IT

/**
  * @brief  Sequential transmit in slave mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
#define i2c_seq_tx HAL_I2C_Slave_Seq_Transmit_IT

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
#define i2c_rx_callback HAL_I2C_SlaveRxCpltCallback

/** @brief  Slave Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
#define i2c_tx_callback HAL_I2C_SlaveTxCpltCallback

/**
  * @brief  I2C error callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
#define i2c_err_callback HAL_I2C_ErrorCallback

/**
  * @brief  Return the I2C error code.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *              the configuration information for the specified I2C.
  * @retval I2C Error Code
  */
#define i2c_get_err HAL_I2C_GetError

/**
  * @brief  Initializes the I2C according to the specified parameters
  *         in the I2C_InitTypeDef and initialize the associated handle.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
#define i2c_reset(pHandler) 											\
/**
  * @brief  DeInitialize the I2C peripheral.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval HAL status
  */																	\
HAL_I2C_DeInit(pHandler);												\
/**
  * @brief  Initializes the I2C according to the specified parameters
  *         in the I2C_InitTypeDef and initialize the associated handle.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */																	\
HAL_I2C_Init(pHandler)


/**
  * @brief  Slave Rx Transfer buffer ready to process.
  * @param  The Rx Transfer buffer.
  * @param  The Rx Transfer buffer's size.
  * @retval None
  */
void i2c_slave_rx_process(uint8_t* data, uint16_t size);

/**
  * @brief  Set the data for slave Tx Transfer buffer.
  * @param  The Tx Transfer buffer.
  * @param  The Tx Transfer buffer's size.
  * @retval None
  */
void i2c_set_txBuffer(uint8_t* data, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif
