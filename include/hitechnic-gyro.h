/*!@addtogroup HiTechnic
 * @{
 * @defgroup htgyro Gyroscopic Sensor
 * HiTechnic Gyroscopic Sensor
 * @{
 */

#ifndef __HTGYRO_H__
#define __HTGYRO_H__
/** \file hitechnic-gyro.h
 * \brief HiTechnic Gyroscopic Sensor driver
 *
 * hitechnic-gyro.h provides an API for the HiTechnic Gyroscopic Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Renamed HTGYROgetCalibration to HTGYROreadCal<br>
 *        Renamed HTGYROsetCalibration to HTGYROsetCal<br>
 *        Renamed HTGYROcalibrate to HTGYROstartCal<br>
 *        Added SMUX functions
 * - 0.3: Removed some of the functions requiring SPORT/MPORT macros
 * - 0.4: Removed "NW - No Wait" functions\n
 *        Replaced array structs with typedefs\n
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 20 February 2011
 * \version 0.4
 * \example hitechnic-gyro-test1.c
 * \example hitechnic-gyro-test2.c
 * \example hitechnic-gyro-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

// This ensures the correct sensor types are used.
#if defined(NXT)
TSensorTypes HTGyroType = sensorAnalogInactive;
#elif defined(EV3)
TSensorTypes HTGyroType = sensorLightInactive;
#endif

typedef struct
{
  tI2CData I2CData;
  float rotation;
  float offset;
  bool smux;
  tMUXSensor smuxport;
} tHTGYRO, *tHTGYROPtr;

bool initSensor(tHTGYROPtr htgyroPtr, tSensors port);
bool initSensor(tHTGYROPtr htgyroPtr, tMUXSensor muxsensor);
bool readSensor(tHTGYROPtr htgyroPtr);
bool sensorCalibrate(tHTGYROPtr htgyroPtr);

float HTGYROreadRot(tSensors link);
float HTGYROstartCal(tSensors link);
float HTGYROreadCal(tSensors link);
// void HTGYROsetCal(tSensors link, short offset);

#ifdef __HTSMUX_SUPPORT__
float HTGYROreadRot(tMUXSensor muxsensor);
float HTGYROstartCal(tMUXSensor muxsensor);
float HTGYROreadCal(tMUXSensor muxsensor);
void HTGYROsetCal(tMUXSensor muxsensor, short offset);
#endif // __HTSMUX_SUPPORT__

float HTGYRO_offsets[][] = {{620.0, 620.0, 620.0, 620.0}, /*!< Array for offset values.  Default is 620 */
                          {620.0, 620.0, 620.0, 620.0},
                          {620.0, 620.0, 620.0, 620.0},
                          {620.0, 620.0, 620.0, 620.0}};

/**
 * Read the value of the gyro
 * @param link the HTGYRO port number
 * @return the value of the gyro
 */
float HTGYROreadRot(tSensors link) {
  // Make sure the sensor is configured as type sensorRawValue
  if (SensorType[link] != HTGyroType) {
    SensorType[link] = HTGyroType;
    sleep(100);
  }

  return (SensorValue[link] - HTGYRO_offsets[link][0]);
}

/**
 * Read the value of the gyro
 * @param muxsensor the SMUX sensor port number
 * @return the value of the gyro
 */
#ifdef __HTSMUX_SUPPORT__
float HTGYROreadRot(tMUXSensor muxsensor) {
  return HTSMUXreadAnalogue(muxsensor) - HTGYRO_offsets[SPORT(muxsensor)][MPORT(muxsensor)];
}
#endif // __HTSMUX_SUPPORT__

/**
 * Calibrate the gyro by calculating the average offset of 5 raw readings.
 * @param link the HTGYRO port number
 * @return the new offset value for the gyro
 */
float HTGYROstartCal(tSensors link) {
  long _avgdata = 0;

  // Make sure the sensor is configured as type sensorRawValue
  if (SensorType[link] != HTGyroType) {
    SensorType[link] = HTGyroType;
    sleep(100);
  }

  // Take 50 readings and average them out
  for (short i = 0; i < 50; i++) {
    _avgdata += SensorValue[link];
    sleep(5);
  }

  // Store new offset
  HTGYRO_offsets[link][0] = (_avgdata / 50.0);

  // Return new offset value
  return HTGYRO_offsets[link][0];
}

/**
 * Calibrate the gyro by calculating the average offset of 50 raw readings.
 * @param muxsensor the SMUX sensor port number
 * @return the new offset value for the gyro
 */
#ifdef __HTSMUX_SUPPORT__
float HTGYROstartCal(tMUXSensor muxsensor) {
  long _avgdata = 0;

  // Take 5 readings and average them out
  for (short i = 0; i < 50; i++) {
    _avgdata += HTSMUXreadAnalogue(muxsensor);
    sleep(50);
  }

  // Store new offset
  HTGYRO_offsets[SPORT(muxsensor)][MPORT(muxsensor)] = (_avgdata / 50.0);

  // Return new offset value
  return HTGYRO_offsets[SPORT(muxsensor)][MPORT(muxsensor)];
}
#endif // __HTSMUX_SUPPORT__

/**
 * Override the current offset for the gyro manually
 * @param link the HTGYRO port number
 * @param offset the new offset to be used
 */
//#define HTGYROsetCal(link, offset) HTGYRO_offsets[link][0] = offset
void HTGYROsetCal(tSensors link, short offset) {
  HTGYRO_offsets[link][0] = offset;
}

/**
 * Override the current offset for the gyro manually
 * @param muxsensor the SMUX sensor port number
 * @param offset the new offset to be used
 */
#ifdef __HTSMUX_SUPPORT__
//#define HTGYROsetCal(muxsensor, offset) HTGYRO_offsets[SPORT(muxsensor)][MPORT(muxsensor)] = offset
void HTGYROsetCal(tMUXSensor muxsensor, short offset) {
  HTGYRO_offsets[SPORT(muxsensor)][MPORT(muxsensor)] = offset;
}
#endif // __HTSMUX_SUPPORT__

/**
 * Retrieve the current offset for the gyro
 * @param link the HTGYRO port number
 * @return the offset value for the gyro
 */
float HTGYROreadCal(tSensors link) {
  return HTGYRO_offsets[link][0];
}

/**
 * Retrieve the current offset for the gyro
 * @param muxsensor the SMUX sensor port number
 * @return the offset value for the gyro
 */
#ifdef __HTSMUX_SUPPORT__
float HTGYROreadCal(tMUXSensor muxsensor) {
  return HTGYRO_offsets[SPORT(muxsensor)][MPORT(muxsensor)];
}
#endif // __HTSMUX_SUPPORT__

/**
 * Initialise the sensor's data struct and port
 *
 * @param htgyroPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTGYROPtr htgyroPtr, tSensors port)
{
  memset(htgyroPtr, 0, sizeof(tHTGYROPtr));
  htgyroPtr->I2CData.port = port;
  htgyroPtr->I2CData.type = HTGyroType;
  htgyroPtr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htgyroPtr->I2CData.port] != htgyroPtr->I2CData.type)
    SensorType[htgyroPtr->I2CData.port] = htgyroPtr->I2CData.type;

  return true;
}

/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htgyroPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTGYROPtr htgyroPtr, tMUXSensor muxsensor)
{
  memset(htgyroPtr, 0, sizeof(tHTGYROPtr));
  htgyroPtr->I2CData.port = (tSensors)SPORT(muxsensor);
  htgyroPtr->I2CData.type = sensorI2CCustom;
  htgyroPtr->smux = true;
  htgyroPtr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[htgyroPtr->I2CData.port] != htgyroPtr->I2CData.type)
    SensorType[htgyroPtr->I2CData.port] = htgyroPtr->I2CData.type;

  return HTSMUXsetAnalogueActive(muxsensor);
}

/**
 * Read all the sensor's data
 *
 * @param htgyroPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTGYROPtr htgyroPtr)
{
  memset(htgyroPtr->I2CData.request, 0, sizeof(htgyroPtr->I2CData.request));

  if (htgyroPtr->smux)
    htgyroPtr->rotation = HTSMUXreadAnalogue(htgyroPtr->smuxport) - htgyroPtr->offset;
  else
    htgyroPtr->rotation = SensorValue[htgyroPtr->I2CData.port] - htgyroPtr->offset;

    return true;
}

bool sensorCalibrate(tHTGYROPtr htgyroPtr)
{
  float avgdata = 0.0;

  // Take 50 readings and average them out
  for (short i = 0; i < 50; i++)
  {
    if (htgyroPtr->smux)
      avgdata += HTSMUXreadAnalogue(htgyroPtr->smuxport);
    else
      avgdata += SensorValue[htgyroPtr->I2CData.port];

    sleep(50);
  }
  htgyroPtr->offset = avgdata / 50;

	return true;
}

#endif // __HTGYRO_H__

/* @} */
/* @} */
