/*!@addtogroup HiTechnic
 * @{
 * @defgroup HTBM Barometric Sensor
 * HiTechnic Barometric Sensor
 * @{
 */

#ifndef __HTBM_H__
#define __HTBM_H__
/** \file hitechnic-barometer.h
 * \brief HiTechnic Barometric Sensor driver
 *
 * hitechnic-barometer.h provides an API for the HiTechnic Barometric Sensor.\n
 * Pressure values are between 0 and approx 31 inHG.\n
 * Temperature values are between -25C and +85C.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Replaced array structs with typedefs
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 20 February 2011
 * \version 0.2
 * \example hitechnic-barometer-test1.c
 */

#pragma systemFile

#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTBM_I2C_ADDR  0x02      /*!< Barometric sensor device address */
#define HTBM_OFFSET    0x42      /*!< Offset for data registers */
#define HTBM_TEMP_HIGH 0x00      /*!< Temperature high byte */
#define HTBM_TEMP_LOW  0x01      /*!< Temperature low byte */
#define HTBM_PRES_HIGH 0x02      /*!< Pressure high byte */
#define HTBM_PRES_LOW  0x03      /*!< Pressure low byte */

const float mInHgtohPa = 0.03386389;
const float mInHgtoPsi = 0.491098/1000;

long HTBMreadMInHg(tSensors link);
float HTBMreadhPa(tSensors link);
float HTBMreadPsi(tSensors link);
float HTBMreadTemp(tSensors link);
float HTBMreadTempF(tSensors link);

typedef struct
{
  tI2CData I2CData;
  short mInHg;
  float hPa;
  float Psi;
  float Celcius;
  float Fahrenheit;
  bool smux;
  tMUXSensor smuxport;
} tHTBM, *tHTBMPtr;

//tByteArray HTBM_I2CRequest;    /*!< Array to hold I2C command data */
//tByteArray HTBM_I2CReply;      /*!< Array to hold I2C reply data */

tConfigParams HTBM_config = {HTSMUX_CHAN_I2C, 4, 0x02, 0x42};  /*!< Array to hold SMUX config data for sensor */

///**
// * Read the current atmospheric pressure in 1/1000th inch Hg (mercury)
// * @param link the HTBM port number
// * @return pressure in 1/1000th inch Hg (mercury), -255 if something went wrong
// */
//long HTBMreadMInHg(tSensors link) {
//  memset(HTBM_I2CRequest, 0, sizeof(tByteArray));

//  HTBM_I2CRequest[0] = 2;                       // Message size
//  HTBM_I2CRequest[1] = HTBM_I2C_ADDR;           // I2C Address
//  HTBM_I2CRequest[2] = HTBM_OFFSET + HTBM_PRES_HIGH; // Pressure high byte

//  if (!writeI2C(link, HTBM_I2CRequest, HTBM_I2CReply, 2))
//    return -255;

//  return (HTBM_I2CReply[0] <<  8) + HTBM_I2CReply[1];
//}

///**
// * Read the current atmospheric pressure in hecto Pascal
// * @param link the HTBM port number
// * @return pressure in hecto Pascal, -255 if something went wrong
// */
//float HTBMreadhPa(tSensors link) {
//  return HTBMreadMInHg(link) * mInHgtohPa;
//}

///**
// * Read the current atmospheric pressure in pounds per square inch
// * @param link the HTBM port number
// * @return pressure in pounds per square inch, -255 if something went wrong
// */
//float HTBMreadPsi(tSensors link) {
//  return (float)HTBMreadMInHg(link) * mInHgtoPsi;
//}

///**
// * Read the current air temperature in degrees Celcius
// * @param link the HTBM port number
// * @return current air temperature in degrees Celcius, -255 if something went wrong
// */
//float HTBMreadTemp(tSensors link) {
//  float temp = 0.0;
//  memset(HTBM_I2CRequest, 0, sizeof(tByteArray));

//  HTBM_I2CRequest[0] = 2;                       // Message size
//  HTBM_I2CRequest[1] = HTBM_I2C_ADDR;           // I2C Address
//  HTBM_I2CRequest[2] = HTBM_OFFSET + HTBM_TEMP_HIGH; // Pressure high byte

//  // Send the request
//  if (!writeI2C(link, HTBM_I2CRequest, HTBM_I2CReply, 2))
//    return -255;

//  temp = (HTBM_I2CReply[0] <<  8) + HTBM_I2CReply[1];
//  temp /= 10;
//  return temp;
//}

///**
// * Read the current air temperature in degrees Fahrenheit
// * @param link the HTBM port number
// * @return the current air temperature in degrees Fahrenheit, -255 if something went wrong
// */
//float HTBMreadTempF(tSensors link) {
//  return (HTBMreadTemp(link) * 1.8) + 32;
//}

/**
 * Initialise the sensor's data struct and port
 *
 * @param htbmPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTBMPtr htbmPtr, tSensors port)
{
  memset(htbmPtr, 0, sizeof(tHTBMPtr));
  htbmPtr->I2CData.address = HTBM_I2C_ADDR;
  htbmPtr->I2CData.port = port;
  htbmPtr->I2CData.type = sensorI2CCustom;

  htbmPtr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htbmPtr->I2CData.port] != htbmPtr->I2CData.type)
    SensorType[htbmPtr->I2CData.port] = htbmPtr->I2CData.type;

  return true;
}

/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htbmPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTBMPtr htbmPtr, tMUXSensor muxsensor)
{
  memset(htbmPtr, 0, sizeof(tHTBMPtr));
  htbmPtr->I2CData.port = (tSensors)SPORT(muxsensor);
  htbmPtr->I2CData.address = HTBM_I2C_ADDR;
  htbmPtr->I2CData.type = sensorI2CCustom;
  htbmPtr->smux = true;
  htbmPtr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[htbmPtr->I2CData.port] != htbmPtr->I2CData.type)
    SensorType[htbmPtr->I2CData.port] = htbmPtr->I2CData.type;

  return HTSMUXconfigChannel(muxsensor, HTBM_config);
}

/**
 * Read all the sensor's data
 *
 * @param htbmPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTBMPtr htbmPtr)
{
  memset(htbmPtr->I2CData.request, 0, sizeof(htbmPtr->I2CData.request));

  if (htbmPtr->smux)
  {
    if (!HTSMUXreadPort(htbmPtr->smuxport, htbmPtr->I2CData.reply, 4, HTBM_TEMP_HIGH))
      return false;
  }
  else
  {
    // Read all of the data available on the sensor
    htbmPtr->I2CData.request[0] = 2;                    // Message size
    htbmPtr->I2CData.request[1] = htbmPtr->I2CData.address; // I2C Address
    htbmPtr->I2CData.request[2] = HTBM_OFFSET + HTBM_TEMP_HIGH;
    htbmPtr->I2CData.replyLen = 4;
    htbmPtr->I2CData.requestLen = 2;

    if (!writeI2C(&htbmPtr->I2CData))
      return false;
  }

  // Populate the struct with the newly retrieved data

  // Get the temperature
  htbmPtr->Celcius = ((htbmPtr->I2CData.reply[0] <<  8) + htbmPtr->I2CData.reply[1]) / 10;
  htbmPtr->Fahrenheit = (htbmPtr->Celcius * 1.8) + 32;

  // Get the pressure data
  htbmPtr->mInHg = (htbmPtr->I2CData.reply[2] <<  8) + htbmPtr->I2CData.reply[3];
  htbmPtr->hPa = (float)htbmPtr->mInHg * mInHgtohPa;
  htbmPtr->Psi = (float)htbmPtr->mInHg * mInHgtoPsi;

  return true;
}

#endif // __HTBM_H__

/* @} */
/* @} */
