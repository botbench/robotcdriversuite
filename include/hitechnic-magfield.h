/*!@addtogroup HiTechnic
 * @{
 * @defgroup HTMAG Magnetic Field Sensor
 * HiTechnic Magnetic Field Sensor
 * @{
 */

#ifndef __HTMAG_H__
#define __HTMAG_H__
/** \file hitechnic-magfield.h
 * \brief HiTechnic Magnetic Field Sensor driver
 *
 * hitechnic-magfield.h provides an API for the HiTechnic Magnetic Field Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 27 July 2010
 * \version 0.1
 * \example hitechnic-magfield-test1.c
 * \example hitechnic-magfield-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

// This ensures the correct sensor types are used.
#if defined(NXT)
TSensorTypes HTMAGType = sensorRawValue;
#elif defined(EV3)
TSensorTypes HTMAGType = sensorLightInactive;
#endif

typedef struct
{
  tI2CData I2CData;
  long strength;
  long bias;
  bool smux;
  tMUXSensor smuxport;
} tHTMAG, *tHTMAGPtr;

bool initSensor(tHTMAGPtr htmagPtr, tSensors port);
bool initSensor(tHTMAGPtr htmagPtr, tMUXSensor muxsensor);
bool readSensor(tHTMAGPtr htmagPtr);
bool sensorCalibrate(tHTMAGPtr htmagPtr);


/**
 * Initialise the sensor's data struct and port
 *
 * @param htmagPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTMAGPtr htmagPtr, tSensors port)
{
  memset(htmagPtr, 0, sizeof(tHTMAGPtr));
  htmagPtr->I2CData.port = port;
  htmagPtr->bias = 512;
  htmagPtr->I2CData.type = HTMAGType;
  htmagPtr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htmagPtr->I2CData.port] != htmagPtr->I2CData.type)
    SensorType[htmagPtr->I2CData.port] = htmagPtr->I2CData.type;

  return true;
}


/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htmagPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTMAGPtr htmagPtr, tMUXSensor muxsensor)
{
  memset(htmagPtr, 0, sizeof(tHTMAGPtr));
  htmagPtr->I2CData.port = (tSensors)SPORT(muxsensor);
  htmagPtr->I2CData.type = sensorI2CCustom;
  htmagPtr->smux = true;
  htmagPtr->smuxport = muxsensor;
  htmagPtr->bias = 512;

  // Ensure the sensor is configured correctly
  if (SensorType[htmagPtr->I2CData.port] != htmagPtr->I2CData.type)
    SensorType[htmagPtr->I2CData.port] = htmagPtr->I2CData.type;

  return HTSMUXsetAnalogueActive(muxsensor);
}


/**
 * Read all the sensor's data
 *
 * @param htmagPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTMAGPtr htmagPtr)
{
  memset(htmagPtr->I2CData.request, 0, sizeof(htmagPtr->I2CData.request));

  if (htmagPtr->smux)
    htmagPtr->strength = HTSMUXreadAnalogue(htmagPtr->smuxport) - htmagPtr->bias;
  else
    htmagPtr->strength = 1023 - SensorValue[htmagPtr->I2CData.port] - htmagPtr->bias;

    return true;
}


bool sensorCalibrate(tHTMAGPtr htmagPtr)
{
  long avgdata = 0.0;

  // Take 50 readings and average them out
  for (short i = 0; i < 50; i++)
  {
    if (htmagPtr->smux)
      avgdata += HTSMUXreadAnalogue(htmagPtr->smuxport);
    else
      avgdata += SensorValue[htmagPtr->I2CData.port];

    sleep(50);
  }
  htmagPtr->bias = avgdata / 50;

	return true;
}

#endif // __HTMAG_H__

/* @} */
/* @} */
