/*!@addtogroup HiTechnic
 * @{
 * @defgroup htirr IR Receiver Sensor
 * HiTechnic IR Receiver Sensor
 * @{
 */

#ifndef __HTIRR_H__
#define __HTIRR_H__
/** \file hitechnic-irrecv.h
 * \brief HiTechnic IR Receiver Sensor driver
 *
 * HTIRR2-driver.h provides an API for the IR Receiver Sensor driver.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Changed HTIRRreadChannel() proto to use signed bytes like function.
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 03 November 2009
 * \version 0.2
 * \example hitechnic-irrecv-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTIRR_I2C_ADDR        0x02      /*!< HTIRR I2C device address */
#define HTIRR_OFFSET          0x42      /*!< Offset for data registers */

// Values contained by registers in active mode
#define HTIRR_MOTOR_1A        0x00      /*!< Motor 1A */
#define HTIRR_MOTOR_1B        0x01      /*!< Motor 1B */
#define HTIRR_MOTOR_2A        0x02      /*!< Motor 2A */
#define HTIRR_MOTOR_2B        0x03      /*!< Motor 2B */
#define HTIRR_MOTOR_3A        0x04      /*!< Motor 3A */
#define HTIRR_MOTOR_3B        0x05      /*!< Motor 3B */
#define HTIRR_MOTOR_4A        0x06      /*!< Motor 4A */
#define HTIRR_MOTOR_4B        0x07      /*!< Motor 4B */

/*! Some define. */
#define HTIRR_MOTOR_BRAKE     -128      /*!< Motor brake */

typedef struct
{
  tI2CData I2CData;
  short motA[4];
  short motB[4];
} tHTIRR, *tHTIRRPtr;

bool initSensor(tHTIRRPtr htirrPtr, tSensors port);
bool readSensor(tHTIRRPtr htirrPtr);


/**
 * Initialise the sensor's data struct and port
 *
 * @param htirrPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTIRRPtr htirrPtr, tSensors port)
{
  memset(htirrPtr, 0, sizeof(tHTIRRPtr));
  htirrPtr->I2CData.address = HTIRR_I2C_ADDR;
  htirrPtr->I2CData.port = port;
  htirrPtr->I2CData.type = sensorI2CCustom;

  // Ensure the sensor is configured correctly
  if (SensorType[htirrPtr->I2CData.port] != htirrPtr->I2CData.type)
    SensorType[htirrPtr->I2CData.port] = htirrPtr->I2CData.type;

	return true;
}


/**
 * Read all the sensor's data
 *
 * @param htirrPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTIRRPtr htirrPtr)
{
  memset(htirrPtr->I2CData.request, 0, sizeof(htirrPtr->I2CData.request));

  // Read all of the data available on the sensor
  htirrPtr->I2CData.request[0] = 2;                    // Message size
  htirrPtr->I2CData.request[1] = htirrPtr->I2CData.address; // I2C Address
  htirrPtr->I2CData.request[2] = HTIRR_OFFSET + HTIRR_MOTOR_1A;
  htirrPtr->I2CData.replyLen = 8;
  htirrPtr->I2CData.requestLen = 2;

  if (!writeI2C(&htirrPtr->I2CData))
    return false;

  // Populate the struct with the newly retrieved data
  for (int i = 0; i < 4; i++)
  {
		htirrPtr->motA[i] = (htirrPtr->I2CData.reply[(i*2)+0] >= 128) ? (short)htirrPtr->I2CData.reply[(i*2)+0] - 256 : (short)htirrPtr->I2CData.reply[(i*2)+0];
		htirrPtr->motB[i] = (htirrPtr->I2CData.reply[(i*2)+1] >= 128) ? (short)htirrPtr->I2CData.reply[(i*2)+1] - 256 : (short)htirrPtr->I2CData.reply[(i*2)+1];
  }

  return true;
}

#endif // __HTIRR_H__

/* @} */
/* @} */
