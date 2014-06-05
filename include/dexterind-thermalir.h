/*!@addtogroup Dexter_Industries
 * @{
 * @defgroup tirPtr Thermal Infrared Sensor
 * Dexter Industries Thermal Infrared Sensor
 * @{
 */

#ifndef __TIR_H__
#define __TIR_H__
/** \file dexterind-thermalir.h
 * \brief Dexter Industries Thermal Infrared Sensor driver
 *
 * TIR2-driver.h provides an API for the Dexter Industries Thermal Infrared Sensor driver.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to Dexter Industries for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 10 June 2011
 * \version 0.1
 * \example dexterind-thermalir-test1.c
 * \example dexterind-thermalir-test2.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define TIR_I2C_ADDR        0x0E      /*!< tirPtr I2C device address */
#define TIR_AMBIENT         0x00      /*!< Thermal Infrared number */
#define TIR_OBJECT          0x01
#define TIR_SET_EMISSIVITY  0x02
#define TIR_GET_EMISSIVITY  0x03
#define TIR_CHK_EMISSIVITY  0x04
#define TIR_RESET           0x05

// Source http://www.rwc.uc.edu/koehler/biophys/8d.html
#define TIR_EM_SKIN_LIGHT   5660
#define TIR_EM_SKIN_DARK    8380
#define TIR_EM_GLASS        9200
#define TIR_EM_CANDLE_SOOT  9500

typedef struct
{
  tI2CData I2CData;
  float ambientTemp;
  float objectTemp;
  short emissivity;
} tTIR, *tTIRPtr;

bool initSensor(tTIRPtr tirPtr, tSensors port)
{
  memset(tirPtr, 0, sizeof(tTIR));
  tirPtr->I2CData.address = TIR_I2C_ADDR;
  tirPtr->I2CData.port = port;
  tirPtr->I2CData.type = sensorI2CCustomFastSkipStates;

  // Ensure the sensor is configured correctly
  if (SensorType[tirPtr->I2CData.port] != tirPtr->I2CData.type)
    SensorType[tirPtr->I2CData.port] = tirPtr->I2CData.type;

  return true;
}

bool readSensor(tTIRPtr tirPtr)
{
  memset(tirPtr->I2CData.request, 0, sizeof(tirPtr->I2CData.request));

  // Read the ambient temperature
  tirPtr->I2CData.request[0] = 2;                    // Message size
  tirPtr->I2CData.request[1] = tirPtr->I2CData.address; // I2C Address
  tirPtr->I2CData.request[2] = TIR_AMBIENT;
  tirPtr->I2CData.replyLen = 2;

  if (!writeI2C(&tirPtr->I2CData))
    return false;

  tirPtr->ambientTemp = ((tirPtr->I2CData.reply[1] << 8) + tirPtr->I2CData.reply[0]) * 0.02 - 273.15;

  // Now read the object temperature
  tirPtr->I2CData.request[2] = TIR_OBJECT;

  if (!writeI2C(&tirPtr->I2CData))
    return false;

  tirPtr->objectTemp = ((tirPtr->I2CData.reply[1] << 8) + tirPtr->I2CData.reply[0]) * 0.02 - 273.15;

  return true;
}

/**
 * Set the current emissivity
 * @param link the tirPtr port number
 * @param emissivity the emissivity of the object that is to be measured
 * @return true if no error occured, false if it did
 */
bool setEmissivity(tTIRPtr tirPtr, short emissivity) {
  tirPtr->emissivity = emissivity;

  tirPtr->I2CData.request[0] = 4;            // Message size
  tirPtr->I2CData.request[1] = tirPtr->I2CData.address; // I2C Address
  tirPtr->I2CData.request[2] = TIR_SET_EMISSIVITY;
  tirPtr->I2CData.request[4] = (tirPtr->emissivity >> 8) & 0xFF;    // High Byte
  tirPtr->I2CData.request[3] = tirPtr->emissivity & 0xFF; // Low Byte.
  tirPtr->I2CData.replyLen = 0;

  return writeI2C(&tirPtr->I2CData);
}

/**
 * Reset the sensor
 * @param link the tirPtr port number
 * @return true if no error occured, false if it did
 */
bool resetSensor(tTIRPtr tirPtr) {

  tirPtr->I2CData.request[0] = 2;            // Message size
  tirPtr->I2CData.request[1] = tirPtr->I2CData.address; // I2C Address
  tirPtr->I2CData.request[2] = TIR_RESET;
  tirPtr->I2CData.replyLen = 0;

  return writeI2C(&tirPtr->I2CData);
}

#endif // __TIR_H__

/* @} */
/* @} */
