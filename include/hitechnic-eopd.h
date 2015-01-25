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
#if defined(NXT)
TSensorTypes HTEOPDLRType = sensorAnalogActive;
TSensorTypes HTEOPDSRType = sensorAnalogInactive;
#elif defined(EV3)
TSensorTypes HTEOPDLRType = sensorLightActive;
TSensorTypes HTEOPDSRType = sensorLightInactive;
#endif

typedef struct
{
  tI2CData I2CData;
  short processed;
  long raw;
  bool shortRange;
  bool smux;
  tMUXSensor smuxport;
} tHTEOPD, *tHTEOPDPtr;

bool initSensor(tHTEOPDPtr hteopdPtr, tSensors port);
bool initSensor(tHTEOPDPtr hteopdPtr, tMUXSensor muxsensor);
bool readSensor(tHTEOPDPtr hteopdPtr);
bool configSensor(tHTEOPDPtr hteopdPtr);


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
  hteopdPtr->I2CData.port = (tSensors)SPORT(muxsensor);
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
#ifdef NXT
    hteopdPtr->raw = 1023 - SensorRaw[hteopdPtr->I2CData.port];
#else // EV3
		hteopdPtr->raw = 4095 - SensorRaw[hteopdPtr->I2CData.port];
#endif


  // Calculate the processed value
#ifdef NXT
	hteopdPtr->processed = round(sqrt(hteopdPtr->raw * 10));
#else // EV3
	hteopdPtr->processed = round(sqrt((hteopdPtr->raw / 4) * 10));
#endif
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
