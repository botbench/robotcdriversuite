/*!@addtogroup HiTechnic
 * @{
 * @defgroup hteopd EOPD Sensor
 * HiTechnic EOPD Sensor
 * @{
 */

#ifndef __HTEOPD_H__
#define __HTEOPD_H__
/** \file hitechnic-eopd.h
 * \brief HiTechnic EOPD Sensor driver
 *
 * hitechnic-eopd.h provides an API for the HiTechnic EOPD sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Removed HTEOPDsetRange() and HTEOPDgetRange(), not really necessary
 *        Changed the way raw value is calculated due to sensor type change
 * - 0.3: Renamed HTEOPDgetRaw to HTEOPDreadRaw
 *        Renamed HTEOPDgetProcessed to HTEOPDreadProcessed
 *        Added SMUX functions
 * - 0.4: Added No Wait versions of HTEOPDsetShortRange and HTEOPDsetLongRange for non-SMUX functions
 *        Changed the underlying sensor types for RobotC 1.57A and higher.
 * - 0.5: Now only supports ROBOTC 2.00<br>
 *        Make use of the new analogue sensor calls for SMUX sensors in common.h
 * - 0.6: Replaced array structs with typedefs
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 20 February 2011
 * \version 0.6
 * \example hitechnic-eopd-test1.c
 * \example hitechnic-eopd-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

// This ensures the correct sensor types are used.
TSensorTypes HTEOPDLRType = sensorAnalogActive;
TSensorTypes HTEOPDSRType = sensorAnalogInactive;

typedef struct
{
  tI2CData I2CData;
  short processed;
  short raw;
  bool shortRange;
  bool smux;
  tMUXSensor smuxport;
} tHTEOPD, *tHTEOPDPtr;

bool initSensor(tHTEOPDPtr hteopdPtr, tSensors port);
bool initSensor(tHTEOPDPtr hteopdPtr, tMUXSensor muxsensor);
bool readSensor(tHTEOPDPtr hteopdPtr);
bool configSensor(tHTEOPDPtr hteopdPtr);

short HTEOPDreadRaw(tSensors link);
short HTEOPDreadProcessed(tSensors link);
void HTEOPDsetShortRange(tSensors link);
void HTEOPDsetLongRange(tSensors link);

#ifdef __HTSMUX_SUPPORT__
short HTEOPDreadRaw(tMUXSensor muxsensor);
short HTEOPDreadProcessed(tMUXSensor muxsensor);
void HTEOPDsetShortRange(tMUXSensor muxsensor);
void HTEOPDsetLongRange(tMUXSensor muxsensor);
#endif

/**
 * Get the raw value from the sensor
 * @param link the HTEOPD port number
 * @return raw value of the sensor
 */
short HTEOPDreadRaw(tSensors link) {
  return 1023 - SensorRaw[link];
}

/**
 * Get the raw value from the sensor
 * @param muxsensor the SMUX sensor port number
 * @return raw value of the sensor
 */
#ifdef __HTSMUX_SUPPORT__
short HTEOPDreadRaw(tMUXSensor muxsensor) {
  return 1023 - HTSMUXreadAnalogue(muxsensor);
}
#endif // __HTSMUX_SUPPORT__

/**
 * Get the processed value from the sensor. This is obtained by using sqrt(raw value * 10)
 * @param link the HTEOPD port number
 * @return processed value of the sensor
 */
short HTEOPDreadProcessed(tSensors link) {
  short _val = sqrt(HTEOPDreadRaw(link) * 10);
  return _val;
}

/**
 * Get the processed value from the sensor. This is obtained by using sqrt(raw value * 10)
 * @param muxsensor the SMUX sensor port number
 * @return processed value of the sensor
 */
#ifdef __HTSMUX_SUPPORT__
short HTEOPDreadProcessed(tMUXSensor muxsensor) {
  short _val = sqrt((long)HTEOPDreadRaw(muxsensor) * (long)10);
  return _val;
}
#endif // __HTSMUX_SUPPORT__

/**
 * Set the range of the sensor to short range, this is done
 * by configuring the sensor as sensorRawValue
 * @param link the HTEOPD port number
 */
void HTEOPDsetShortRange(tSensors link) {
  SensorType[link] = HTEOPDSRType;
}

/**
 * Set the range of the sensor to short range, this is done
 * by switching off dig0
 * @param muxsensor the SMUX sensor port number
 */
#ifdef __HTSMUX_SUPPORT__
void HTEOPDsetShortRange(tMUXSensor muxsensor) {
  HTSMUXsetAnalogueInactive(muxsensor);
}
#endif // __HTSMUX_SUPPORT__

/**
 * Set the range of the sensor to long range, this is done
 * by configuring the sensor as sensorLightActive and setting
 * it to modeRaw
 * @param link the HTEOPD port number
 */
void HTEOPDsetLongRange(tSensors link) {
  SensorType[link] = HTEOPDSRType;
}

/**
 * Set the range of the sensor to long range, this is done
 * by setting dig0 high (1).
 * @param muxsensor the SMUX sensor port number
 */
#ifdef __HTSMUX_SUPPORT__
void HTEOPDsetLongRange(tMUXSensor muxsensor) {
  HTSMUXsetAnalogueActive(muxsensor);
}
#endif // __HTSMUX_SUPPORT__


/**
 * Initialise the sensor's data struct and port
 *
 * @param hteopdPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTEOPDPtr hteopdPtr, tSensors port)
{
  memset(hteopdPtr, 0, sizeof(tHTEOPDPtr));
  hteopdPtr->I2CData.port = port;
  hteopdPtr->I2CData.type = HTEOPDSRType;
  hteopdPtr->shortRange = true;
  hteopdPtr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[hteopdPtr->I2CData.port] != hteopdPtr->I2CData.type)
    SensorType[hteopdPtr->I2CData.port] = hteopdPtr->I2CData.type;

  return true;
}

/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param hteopdPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTEOPDPtr hteopdPtr, tMUXSensor muxsensor)
{
  memset(hteopdPtr, 0, sizeof(tHTEOPDPtr));
  hteopdPtr->I2CData.type = sensorI2CCustom;
  hteopdPtr->shortRange = true;
  hteopdPtr->smux = true;
  hteopdPtr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[hteopdPtr->I2CData.port] != hteopdPtr->I2CData.type)
    SensorType[hteopdPtr->I2CData.port] = hteopdPtr->I2CData.type;

  // Configure as short range
  return HTSMUXsetAnalogueInactive(muxsensor);
}

/**
 * Read all the sensor's data
 *
 * @param hteopdPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTEOPDPtr hteopdPtr)
{
  memset(hteopdPtr->I2CData.request, 0, sizeof(hteopdPtr->I2CData.request));

  // Read the raw analogue data
  if (hteopdPtr->smux)
    hteopdPtr->raw = 1023 - HTSMUXreadAnalogue(hteopdPtr->smuxport);
  else
    hteopdPtr->raw = 1023 - SensorRaw[hteopdPtr->I2CData.port];

  // Calculate the processed value
	hteopdPtr->processed = round(sqrt((long)hteopdPtr->raw * 10));
  return true;
}


/**
 * Configure the sensor for either short or long range mode
 *
 * @param hteopdPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool configSensor(tHTEOPDPtr hteopdPtr)
{
	if (hteopdPtr->smux)
	{
		if (hteopdPtr->shortRange)
			return HTSMUXsetAnalogueInactive(hteopdPtr->smuxport);
		else
			return HTSMUXsetAnalogueActive(hteopdPtr->smuxport);
	}
	else
	{
		if (hteopdPtr->shortRange)
			SensorType[hteopdPtr->I2CData.port] = HTEOPDSRType;
		else
			SensorType[hteopdPtr->I2CData.port] = HTEOPDLRType;
	}
	return true;
}

#endif // __HTEOPD_H__

/* @} */
/* @} */
