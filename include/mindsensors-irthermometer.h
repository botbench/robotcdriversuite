/*!@addtogroup mindsensors
 * @{
 * @defgroup irthermo IR Thermometer Sensor
 * IRThermo Sensor
 * @{
 */

#ifndef __MSIR_H__
#define __MSIR_H__
/** \file mindsensors-irthermometer.h
 * \brief Mindsensors IRThermo driver
 *
 * mindsensors-irthermometer.h provides an API for the Mindsensors IR Thermometer driver
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.53 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 18 May 2016
 * \version 0.1
 * \example mindsensors-irthermometer-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define MSIR_I2C_ADDR       0x2A  /*!< MSIR I2C device address */

#define MSIR_CMD            0x41  /*!< MSIR command register */

#define MSIR_AMBIENT_C      0x42  /*!< MSIR Ambient Temp (C) */
#define MSIR_TARGET_C       0x44  /*!< MSIR Target Temp (C) */
#define MSIR_AMBIENT_F      0x46  /*!< MSIR Ambient Temp (F) */
#define MSIR_TARGET_F       0x48  /*!< MSIR Target Temp (F) */
#define MSIR_EMISSIVITY     0x4A  /*!< MSIR Emissivity */

typedef struct
{
  tI2CData I2CData;
  float ambient;
  float target;
  bool bUseCelcius;
  short emissivity;
} tMSIR, *tMSIRPtr;

bool initSensor(tMSIRPtr MSIRPtr, tSensors port, bool bUseCelcius = true);
bool readSensor(tMSIRPtr MSIRPtr);
bool configSensor(tMSIRPtr MSIRPtr);

/**
 * Initialise the sensor's data struct and port
 *
 * @param MSIRPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tMSIRPtr MSIRPtr, tSensors port, bool bUseCelcius)
{
  memset(MSIRPtr, 0, sizeof(tMSIRPtr));
  MSIRPtr->I2CData.address = MSIR_I2C_ADDR;
  MSIRPtr->I2CData.port = port;
#ifdef EV3
  MSIRPtr->I2CData.type = sensorEV3_GenericI2C;
#elif defined(NXT)
  MSIRPtr->I2CData.type = sensorI2CCustom;
#endif
  // Ensure the sensor is configured correctly
  if (SensorType[MSIRPtr->I2CData.port] != MSIRPtr->I2CData.type)
    SensorType[MSIRPtr->I2CData.port] = MSIRPtr->I2CData.type;
	MSIRPtr->bUseCelcius = bUseCelcius;

  return configSensor(MSIRPtr);
}


/**
 * Read all the sensor's data
 *
 * @param MSIRPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tMSIRPtr MSIRPtr)
{
	float tmp;

  memset(MSIRPtr->I2CData.request, 0, sizeof(MSIRPtr->I2CData.request));

  // Read all of the data available on the sensor
  MSIRPtr->I2CData.request[0] = 2;                    // Message size
  MSIRPtr->I2CData.request[1] = MSIRPtr->I2CData.address; // I2C Address
  MSIRPtr->I2CData.request[2] = (MSIRPtr->bUseCelcius) ? MSIR_AMBIENT_C : MSIR_AMBIENT_F;
  MSIRPtr->I2CData.replyLen = 4;
  MSIRPtr->I2CData.requestLen = 2;

  if (!writeI2C(&MSIRPtr->I2CData))
    return false;

  // Populate the struct with the newly retrieved data
  // Convert 2 bytes into a signed 16 bit value.
  MSIRPtr->ambient = MSIRPtr->I2CData.reply[0] | (short)MSIRPtr->I2CData.reply[1] << 8;
  MSIRPtr->target = MSIRPtr->I2CData.reply[2] | (short)MSIRPtr->I2CData.reply[3] << 8;

  MSIRPtr->ambient /= 100.0;
  MSIRPtr->target /= 100.0;

  return true;
}

bool configSensor(tMSIRPtr MSIRPtr)
{
	return true;
}

#endif //__MSIR_H__

/* @} */
/* @} */
