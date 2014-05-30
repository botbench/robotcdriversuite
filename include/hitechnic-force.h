/*!@addtogroup HiTechnic
 * @{
 * @defgroup htforce Force Sensor
 * HiTechnic Force Sensor
 * @{
 */

/*
 * $Id: hitechnic-force.h $
 */

#ifndef HTF
#define __HTF_H__
/** \file hitechnic-force.h
 * \brief HiTechnic Force Sensor driver
 *
 * hitechnic-force.h provides an API for the HiTechnic Force sensor.
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
 * \date 30 September 2012
 * \version 0.1
 * \example hitechnic-force-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

typedef struct
{
  tI2CData I2CData;
  int force;
  bool smux;
  tMUXSensor smuxport;
} tHTF, *tHTFPtr;

bool initSensor(tHTFPtr htfPtr, tSensors port);
bool initSensor(tHTFPtr htfPtr, tMUXSensor muxsensor);
bool sensorReadAll(tHTFPtr htfPtr);

//int HTFreadSensor(tSensors link);

//#ifdef __HTSMUX_SUPPORT__
//int HTFreadSensor(tMUXSensor muxsensor);
//#endif

///**
// * Get the raw value from the sensor
// * @param link the HTF port number
// * @return raw value of the sensor
// */
//int HTFreadSensor(tSensors link) {
//  return 1023 - SensorRaw[link];
//}


///**
// * Get the raw value from the sensor
// * @param muxsensor the SMUX sensor port number
// * @return raw value of the sensor
// */
//#ifdef __HTSMUX_SUPPORT__
//int HTFreadSensor(tMUXSensor muxsensor) {
//  return 1023 - HTSMUXreadAnalogue(muxsensor);
//}
//#endif // __HTSMUX_SUPPORT__


/**
 * Initialise the sensor's data struct and port
 *
 * @param htfPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTFPtr htfPtr, tSensors port)
{
  memset(htfPtr, 0, sizeof(tHTFPtr));
  htfPtr->I2CData.port = port;
  htfPtr->I2CData.type = sensorAnalogActive;
  htfPtr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htfPtr->I2CData.port] != htfPtr->I2CData.type)
    SensorType[htfPtr->I2CData.port] = htfPtr->I2CData.type;

  return true;
}


/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htfPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTFPtr htfPtr, tMUXSensor muxsensor)
{
  memset(htfPtr, 0, sizeof(tHTFPtr));
  htfPtr->I2CData.type = sensorI2CCustom;
  htfPtr->smux = true;
	htfPtr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[htfPtr->I2CData.port] != htfPtr->I2CData.type)
    SensorType[htfPtr->I2CData.port] = htfPtr->I2CData.type;

  return HTSMUXsetAnalogueActive(muxsensor);
}


/**
 * Read all the sensor's data
 *
 * @param htfPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool sensorReadAll(tHTFPtr htfPtr)
{
	memset(htfPtr->I2CData.request, 0, sizeof(htfPtr->I2CData.request));

	if (htfPtr->smux)
		htfPtr->force = 1023 - HTSMUXreadAnalogue(htfPtr->smuxport);
	else
	  htfPtr->force = 1023 - SensorValue[htfPtr->I2CData.port];

	  return true;
}


#endif // __HTF_H__

/*
 * $Id: hitechnic-force.h $
 */
/* @} */
/* @} */
