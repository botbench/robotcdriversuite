/*!@addtogroup HiTechnic
 * @{
 * @defgroup htmc Compass Sensor
 * HiTechnic Compass Sensor
 * @{
 */

/*
 * $Id: hitechnic-compass.h $
 */

/** \file hitechnic-compass.h
 * \brief HiTechnic Magnetic Compass Sensor Driver
 *
 * hitechnic-compass.h provides an API for the HiTechnic Magnetic Compass Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Added SMUX functions
 * - 0.3: Removed HTMC_SMUXData, reuses HTMC_I2CReply to save memory
 * - 0.4: Replaced hex values in calibration functions with #define's
 * - 0.5: Replaced functions requiring SPORT/MPORT macros
 * - 0.6: simplified relative heading calculations - Thanks Gus!
 * - 0.7: Changed to new SMUX support system\n
 *        Merged target functions with optional heading argument.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 18 January 2011
 * \version 0.7
 * \example hitechnic-compass-test1.c
 * \example hitechnic-compass-test2.c
 * \example hitechnic-compass-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

// I2C address + registers
#define HTMC_I2C_ADDR       0x02  /*!< HTMC I2C device address */
#define HTMC_MODE           0x41  /*!< HTMC Mode control */
#define HTMC_OFFSET					0x42  /*!< Offset for data registers */
#define HTMC_HEAD_U         0x00  /*!< HTMC Heading Upper bits */
#define HTMC_HEAD_L         0x43  /*!< HTMC Heading Lower bit */

// I2C commands
#define HTMC_MEASURE_CMD    0x00  /*!< HTMC measurement mode command */
#define HTMC_CALIBRATE_CMD  0x43 /*!< HTMC calibrate mode command */

typedef struct
{
  tI2CData I2CData;
  short heading;
  short relativeHeading;
  short offset;
  bool smux;
  tMUXSensor smuxport;
} tHTMC, *tHTMCPtr;

bool initSensor(tHTMCPtr htmcPtr, tSensors port);
bool initSensor(tHTMCPtr htmcPtr, tMUXSensor muxsensor);
bool readSensor(tHTMCPtr htmcPtr);
bool sensorCalibrate(tHTMCPtr htmcPtr);
bool sensorStopCalibrate(tHTMCPtr htmcPtr);

//bool HTMCstartCal(tSensors link);
//bool HTMCstopCal(tSensors link);
//short HTMCreadHeading(tSensors link);
//short HTMCreadRelativeHeading(tSensors link);
//short HTMCsetTarget(tSensors link, short offset = 0);

//#ifdef __HTSMUX_SUPPORT__
//short HTMCreadHeading(tMUXSensor muxsensor);
//short HTMCreadRelativeHeading(tMUXSensor muxsensor);
//short HTMCsetTarget(tMUXSensor muxsensor, short offset = 0);

tConfigParams HTMC_config = {HTSMUX_CHAN_I2C, 2, 0x02, 0x42}; /*!< Array to hold SMUX config data for sensor */
//#endif // __HTSMUX_SUPPORT__

//tByteArray HTMC_I2CRequest;       /*!< Array to hold I2C command data */
//tByteArray HTMC_I2CReply;         /*!< Array to hold I2C reply data */

//short target[][] = {{0, 0, 0, 0},   /*!< Offsets for the compass sensor relative readings */
//                  {0, 0, 0, 0},
//                  {0, 0, 0, 0},
//                  {0, 0, 0, 0}};


///**
// * Start the calibration. The sensor should be rotated a little more than 360 along the
// * horizontal plane in no less than 20 seconds.  After the sensor has been rotated,
// * call HTMCstopCal() to set the sensor back in measurement mode and save the
// * calibration data.  This calibration data is stored in the sensor until the
// * next calibration.
// * @param link the HTMC port number
// * @return true if no error occured, false if it did
// */
//bool HTMCstartCal(tSensors link) {
//  memset(HTMC_I2CRequest, 0, sizeof(tByteArray));

//  HTMC_I2CRequest[0] = 3;                   // Number of bytes in I2C command
//  HTMC_I2CRequest[1] = HTMC_I2C_ADDR;       // I2C address of compass sensor
//  HTMC_I2CRequest[2] = HTMC_MODE;           // Set write address to sensor mode register
//  HTMC_I2CRequest[3] = HTMC_CALIBRATE_CMD;  // The calibration mode command

//  // Start the calibration
//  return writeI2C(link, HTMC_I2CRequest);
//}


///**
// * Stop the calibration. This should be called no less than 20 seconds after
// * HTMCstartCal() and only if the sensor has been rotated more than 360 degrees
// * @param link the HTMC port number
// * @return true if no error occured, false if it did
// */
//bool HTMCstopCal(tSensors link) {
//  memset(HTMC_I2CRequest, 0, sizeof(tByteArray));

//  HTMC_I2CRequest[0] = 3;                 // Number of bytes in I2C command
//  HTMC_I2CRequest[1] = HTMC_I2C_ADDR;     // I2C address of compass sensor
//  HTMC_I2CRequest[2] = HTMC_MODE;         // Set write address to sensor mode register
//  HTMC_I2CRequest[3] = HTMC_MEASURE_CMD;  // The measurement mode command

//  // Stop the calibration by setting the mode register back to measurement.
//  // Read back the register value to check if an error has occurred.
//  if (!writeI2C(link, HTMC_I2CRequest, HTMC_I2CReply, 1))
//    return false;

//  // The register is equal to 2 if the calibration has failed.
//  if (HTMC_I2CReply[0] == 2)
//    return false;

//  return true;
//}


///**
// * Return the current absolute heading
// * @param link the HTMC port number
// * @return heading in degrees (0 - 359) or -1 if an error occurred.
// */
//short HTMCreadHeading(tSensors link) {
//  memset(HTMC_I2CRequest, 0, sizeof(tByteArray));

//  HTMC_I2CRequest[0] = 2;               // Number of bytes in I2C command
//  HTMC_I2CRequest[1] = HTMC_I2C_ADDR;   // I2C address of compass sensor
//  HTMC_I2CRequest[2] = HTMC_HEAD_U;     // Set write address to sensor mode register

//  if (!writeI2C(link, HTMC_I2CRequest, HTMC_I2CReply, 2))
//    return -1;

//  // Result is made up of two bytes.  Reassemble for final heading.
//  return (HTMC_I2CReply[0] * 2) + HTMC_I2CReply[1];
//}


///**
// * Return the current absolute heading
// * @param muxsensor the SMUX sensor port number
// * @return heading in degrees (0 - 359) or -1 if an error occurred.
// */
//#ifdef __HTSMUX_SUPPORT__
//short HTMCreadHeading(tMUXSensor muxsensor) {
//  memset(HTMC_I2CReply, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, HTMC_config);

//  if (!HTSMUXreadPort(muxsensor, HTMC_I2CReply, 2)) {
//    return -1;
//  }

//  // Result is made up of two bytes.  Reassemble for final heading.
//  return (HTMC_I2CReply[0] * 2) + HTMC_I2CReply[1];
//}
//#endif // __HTSMUX_SUPPORT__


///**
// * Return the current relative heading, value between -179 and 180
// * @param link the HTMC port number
// * @return relative heading in degrees or -255 if an error occurred.
// */
//short HTMCreadRelativeHeading(tSensors link) {

//  // The black voodoo magic code below is courtsey of Gus from HiTechnic.
//  short _tmpHeading = HTMCreadHeading(link) - target[link][0] + 180;
//  return (_tmpHeading >= 0 ? _tmpHeading % 360 : 359 - (-1 - _tmpHeading)%360) - 180;
//}


///**
// * Return the current relative heading, value between -179 and 180
// * @param muxsensor the SMUX sensor port number
// * @return relative heading in degrees or -255 if an error occurred.
// */
//#ifdef __HTSMUX_SUPPORT__
//short HTMCreadRelativeHeading(tMUXSensor muxsensor) {

//  // The black voodoo magic code below is courtsey of Gus from HiTechnic.
//  short _tmpHeading = HTMCreadHeading(muxsensor) - target[SPORT(muxsensor)][MPORT(muxsensor)] + 180;
//  return (_tmpHeading >= 0 ? _tmpHeading % 360 : 359 - (-1 - _tmpHeading)%360) - 180;

//  // return ((HTMCreadHeading(muxsensor) - target[SPORT(muxsensor)][MPORT(muxsensor)] + 540) % 360 - 180);
//}
//#endif


///**
// * Set the value for the offset to be used as the new zero-point
// * for the relative heading returned by HTMCreadRelativeHeading()
// * @param link the HTMC port number
// * @param offset to be used to calculate relative heading (0-360 degrees).  If unspecified, uses current heading.
// * @return the current target heading
// */
//short HTMCsetTarget(tSensors link, short offset) {
//  target[link][0] = (offset != 0) ? offset : HTMCreadHeading(link);
//  return target[link][0];
//}


///**
// * Set the value for the offset to be used as the new zero-point
// * for the relative heading returned by HTMCreadRelativeHeading()
// * @param muxsensor the SMUX sensor port number
// * @param offset to be used to calculate relative heading (0-360 degrees).  If unspecified, uses current heading.
// */
//#ifdef __HTSMUX_SUPPORT__
//short HTMCsetTarget(tMUXSensor muxsensor, short offset) {
//  target[SPORT(muxsensor)][MPORT(muxsensor)] = (offset != 0) ? offset : HTMCreadHeading(muxsensor);
//  return target[SPORT(muxsensor)][MPORT(muxsensor)];
//}
//#endif // __HTSMUX_SUPPORT__


/**
 * Initialise the sensor's data struct and port
 *
 * @param htmcPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTMCPtr htmcPtr, tSensors port)
{
  memset(htmcPtr, 0, sizeof(tHTMCPtr));
  htmcPtr->I2CData.address = HTMC_I2C_ADDR;
  htmcPtr->I2CData.port = port;
  htmcPtr->I2CData.type = sensorI2CCustom;
  htmcPtr->smux = false;
	htmcPtr->offset = 0;

  // Ensure the sensor is configured correctly
  if (SensorType[htmcPtr->I2CData.port] != htmcPtr->I2CData.type)
    SensorType[htmcPtr->I2CData.port] = htmcPtr->I2CData.type;

  return true;
}


/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htmcPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTMCPtr htmcPtr, tMUXSensor muxsensor)
{
  memset(htmcPtr, 0, sizeof(tHTMCPtr));
  htmcPtr->I2CData.address = HTMC_I2C_ADDR;
  htmcPtr->I2CData.type = sensorI2CCustom;
  htmcPtr->smux = true;
	htmcPtr->smuxport = muxsensor;
	htmcPtr->offset = 0;

  // Ensure the sensor is configured correctly
  if (SensorType[htmcPtr->I2CData.port] != htmcPtr->I2CData.type)
    SensorType[htmcPtr->I2CData.port] = htmcPtr->I2CData.type;

  return HTSMUXconfigChannel(muxsensor, HTMC_config);
}


/**
 * Read all the sensor's data
 *
 * @param htmcPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTMCPtr htmcPtr)
{
	short tempHeading = 0;
	memset(htmcPtr->I2CData.request, 0, sizeof(htmcPtr->I2CData.request));

	if (htmcPtr->smux)
	{
		if (!HTSMUXreadPort(htmcPtr->smuxport, htmcPtr->I2CData.reply, 2, HTMC_HEAD_U))
			return false;
	}
	else
	{
	  // Read all of the data available on the sensor
	  htmcPtr->I2CData.request[0] = 2;                    // Message size
	  htmcPtr->I2CData.request[1] = htmcPtr->I2CData.address; // I2C Address
	  htmcPtr->I2CData.request[2] = HTMC_OFFSET + HTMC_HEAD_U;
	  htmcPtr->I2CData.replyLen = 2;
	  htmcPtr->I2CData.requestLen = 2;

	  if (!writeI2C(&htmcPtr->I2CData))
	    return false;
	}

	// Populate the struct with the newly retrieved data
	htmcPtr->heading = (htmcPtr->I2CData.reply[0] * 2) + htmcPtr->I2CData.reply[1];
	tempHeading = htmcPtr->heading - htmcPtr->offset + 180;
  htmcPtr->relativeHeading = (tempHeading >= 0 ? tempHeading % 360 : 359 - (-1 - tempHeading)%360) - 180;

  return true;
}

bool sensorCalibrate(tHTMCPtr htmcPtr)
{
	// Operation not supported on a SMUX
	if (htmcPtr->smux)
		return false;

	memset(htmcPtr->I2CData.request, 0, sizeof(htmcPtr->I2CData.request));

  // Read all of the data available on the sensor
  htmcPtr->I2CData.request[0] = 3;                    // Message size
  htmcPtr->I2CData.request[1] = htmcPtr->I2CData.address; // I2C Address
  htmcPtr->I2CData.request[2] = HTMC_MODE;
  htmcPtr->I2CData.request[3] = HTMC_CALIBRATE_CMD;
  htmcPtr->I2CData.replyLen = 0;
  htmcPtr->I2CData.requestLen = 3;

  // Start the calibration
  return writeI2C(&htmcPtr->I2CData);
}


bool sensorStopCalibrate(tHTMCPtr htmcPtr)
{
	// Operation not supported on a SMUX
	if (htmcPtr->smux)
		return false;

	memset(htmcPtr->I2CData.request, 0, sizeof(htmcPtr->I2CData.request));

  // Read all of the data available on the sensor
  htmcPtr->I2CData.request[0] = 3;                    // Message size
  htmcPtr->I2CData.request[1] = htmcPtr->I2CData.address; // I2C Address
  htmcPtr->I2CData.request[2] = HTMC_MODE;
  htmcPtr->I2CData.request[3] = HTMC_MEASURE_CMD;
  htmcPtr->I2CData.replyLen = 1;
  htmcPtr->I2CData.requestLen = 3;

  // The register is equal to 2 if the calibration has failed.
  return (htmcPtr->I2CData.reply[0] == 2) ? false : true;
}


/*
 * $Id: hitechnic-compass.h $
 */
/* @} */
/* @} */
