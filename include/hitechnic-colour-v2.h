/*!@addtogroup HiTechnic
 * @{
 * @defgroup htcs2 Color Sensor V2
 * HiTechnic Color Sensor V2
 * @{
 */

/*
 * $Id: hitechnic-colour-v2.h $
 */

#ifndef __HTCS2_H__
#define __HTCS2_H__
/** \file hitechnic-colour-v2.h
 * \brief HiTechnic Color Sensor V2 driver
 *
 * HTCS22-driver.h provides an API for the HiTechnic Color Sensor driver.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Use new calls in common.h that don't require SPORT/MPORT macros
 *        Removed usage of ubyteToInt();
 * - 0.3: Replaced array structs with typedefs
 * - 0.4: Added HTCSreadHSV(), thanks Mike Henning, Max Bareiss
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 27 April 2011
 * \version 0.4
 * \example hitechnic-colour-v2-test1.c
 * \example hitechnic-colour-v2-test2.c
 * \example hitechnic-colour-v2-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

#ifndef __LIGHT_COMMON_H__
#include "common-light.h"
#endif

#define HTCS2_I2C_ADDR        0x02      /*!< HTCS2 I2C device address */
#define HTCS2_CMD_REG         0x41      /*!< Command register */
#define HTCS2_OFFSET          0x42      /*!< Offset for data registers */

// Values contained by registers in active mode
#define HTCS2_COLNUM_REG      0x00      /*!< Color number */
#define HTCS2_RED_REG         0x01      /*!< Red reading */
#define HTCS2_GREEN_REG       0x02      /*!< Green reading */
#define HTCS2_BLUE_REG        0x03      /*!< Blue reading */
#define HTCS2_WHITE_REG       0x04      /*!< White channel reading */
#define HTCS2_COL_INDEX_REG   0x05      /*!< Color index number */
#define HTCS2_RED_NORM_REG    0x06      /*!< Normalised red reading */
#define HTCS2_GREEN_NORM_REG  0x07      /*!< Normalised green reading */
#define HTCS2_BLUE_NORM_REG   0x08      /*!< Normalised blue reading */

// Different modes
#define HTCS2_MODE_ACTIVE     0x00      /*!< Use ambient light cancellation */
#define HTCS2_MODE_PASSIVE    0x01      /*!< Disable ambient light cancellation */
#define HTCS2_MODE_RAW        0x03      /*!< Raw data from light sensor */
#define HTCS2_MODE_50HZ       0x35      /*!< Set sensor to 50Hz cancellation mode */
#define HTCS2_MODE_60HZ       0x36      /*!< Set sensor to 60Hz cancellation mode */

typedef struct
{
  tI2CData I2CData;
  short color;
  short red;
  short green;
  short blue;
  float hue;
  float saturation;
  float value;
	ubyte _cmd;
  bool smux;
  tMUXSensor smuxport;
} tHTCS2, *tHTCS2Ptr;

bool initSensor(tHTCS2Ptr htcs2Ptr, tSensors port);
bool initSensor(tHTCS2Ptr htcs2Ptr, tMUXSensor muxsensor);
bool readSensor(tHTCS2Ptr htcs2Ptr);
bool readSensorNorm(tHTCS2Ptr htcs2Ptr);
bool _sensorSendCommand(tHTCS2Ptr htcs2Ptr);

short HTCS2readColor(tSensors link);
bool HTCS2readRGB(tSensors link, short &red, short &green, short &blue);
bool HTCS2readHSV(tSensors link, float &hue, float &saturation, float &value);
bool HTCS2readWhite(tSensors link, short &white);
bool HTCS2readNormRGB(tSensors link, short &red, short &green, short &blue);
bool HTCS2readRawRGB(tSensors link, bool passive, long &red, long &green, long &blue);
bool HTCS2readRawWhite(tSensors link, bool passive, long &white);
bool _HTCSsendCommand(tSensors link, byte command);

#ifdef __HTSMUX_SUPPORT__
short HTCS2readColor(tMUXSensor muxsensor);
bool HTCS2readRGB(tMUXSensor muxsensor, short &red, short &green, short &blue);

tConfigParams HTCS2_config = {HTSMUX_CHAN_I2C, 4, 0x02, 0x42};  /*!< Array to hold SMUX config data for sensor */
#endif // __HTSMUX_SUPPORT__

tByteArray HTCS2_I2CRequest;           /*!< Array to hold I2C command data */
tByteArray HTCS2_I2CReply;             /*!< Array to hold I2C reply data */

/*!< Array to hold sensor modes */
signed byte active_mode[4] = {-1, -1, -1, -1};


///**
// * Return the color number currently detected.
// * @param link the HTCS2 port number
// * @return color index number or -1 if an error occurred.
// */
//short HTCS2readColor(tSensors link) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (active_mode[link] != HTCS2_MODE_ACTIVE)
//    _HTCSsendCommand(link, HTCS2_MODE_ACTIVE);

//  HTCS2_I2CRequest[0] = 2;                                // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;                   // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_COLNUM_REG;  // Start colour number register

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 1))
//    return -1;

//  return HTCS2_I2CReply[0];
//}


///**
// * Return the color number currently detected.
// * @param muxsensor the SMUX sensor port number
// * @return color index number or -1 if an error occurred.
// */
//#ifdef __HTSMUX_SUPPORT__
//short HTCS2readColor(tMUXSensor muxsensor) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, HTCS2_config);

//  if (!HTSMUXreadPort(muxsensor, HTCS2_I2CReply, 1, HTCS2_COLNUM_REG)) {
//    return -1;
//  }

//  return HTCS2_I2CReply[0];
//}
//#endif // __HTSMUX_SUPPORT__


///**
// * Get the detection levels for the three color components.
// * @param link the HTCS2 port number
// * @param red the red value
// * @param green the green value
// * @param blue the blue value
// * @return true if no error occured, false if it did
// */
//bool HTCS2readRGB(tSensors link, short &red, short &green, short &blue) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (active_mode[link] != HTCS2_MODE_ACTIVE)
//    _HTCSsendCommand(link, HTCS2_MODE_ACTIVE);

//  HTCS2_I2CRequest[0] = 2;                           // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;               // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_RED_REG;  // Start red sensor value

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 3))
//    return false;

//  red = HTCS2_I2CReply[0];
//  green = HTCS2_I2CReply[1];
//  blue = HTCS2_I2CReply[2];

//  return true;
//}


///**
// * Get the detection levels for the three color components.
// * @param muxsensor the SMUX sensor port number
// * @param red the red value
// * @param green the green value
// * @param blue the blue value
// * @return true if no error occured, false if it did
// */
//#ifdef __HTSMUX_SUPPORT__
//bool HTCS2readRGB(tMUXSensor muxsensor, short &red, short &green, short &blue) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, HTCS2_config);

//  if (!HTSMUXreadPort(muxsensor, HTCS2_I2CReply, 3, HTCS2_RED_REG)) {
//    return false;
//  }

//  red = HTCS2_I2CReply[0];
//  green = HTCS2_I2CReply[1];
//  blue = HTCS2_I2CReply[2];

//  return true;
//}
//#endif // __HTSMUX_SUPPORT__


///**
// * Get the detection levels for the hue, saturation, value components.
// * @param link the HTCS port number
// * @param hue the hue output value (from 0 to 365, or -1 if n/a)
// * @param saturation the saruration output value (from 0 to 100, or -1 if n/a)
// * @param value the value output value (from 0 to 100)
// * @return true if no error occured, false if it did

// */
//bool HTCS2readHSV(tSensors link, float &hue, float &saturation, float &value) {
//  short red,green,blue;

//  bool ret = HTCS2readRGB(link, red, green, blue);
//  RGBtoHSV(red,green,blue, hue, saturation, value);

//  return ret;
//}


///**
// * Get the detection levels for the hue, saturation, value components.
// * @param muxsensor the SMUX sensor port number
// * @param hue the hue output value (from 0 to 365, or -1 if n/a)
// * @param saturation the saruration output value (from 0 to 100, or -1 if n/a)
// * @param value the value output value (from 0 to 100)
// * @return true if no error occured, false if it did
// */
//#ifdef __HTSMUX_SUPPORT__
//bool HTCS2readHSV(tMUXSensor muxsensor, float &hue, float &saturation, float &value) {
//  short red,green,blue;

//  bool ret = HTCS2readRGB(muxsensor, red, green, blue);
//  RGBtoHSV(red,green,blue, hue, saturation, value);

//  return ret;
//}
//#endif // __HTSMUX_SUPPORT__


///**
// * Get the detection level for the white channel.
// * @param link the HTCS2 port number
// * @param white the white channel value
// * @return true if no error occured, false if it did
// */
//bool HTCS2readWhite(tSensors link, short &white) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (active_mode[link] != HTCS2_MODE_ACTIVE)
//    _HTCSsendCommand(link, HTCS2_MODE_ACTIVE);

//  HTCS2_I2CRequest[0] = 2;                           // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;               // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_RED_REG;  // Start red sensor value

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 1))
//    return false;

//  white = HTCS2_I2CReply[0];

//  return true;
//}


///**
// * Get the normalised RGB readings. The normalization sets the highest
// * value of the three Red, Green and Blue reading to 255 and adjusts the
// * other two proportionately.
// * @param link the HTCS2 port number
// * @param red the red value
// * @param green the green value
// * @param blue the blue value
// * @return true if no error occured, false if it did
// */
//bool HTCS2readNormRGB(tSensors link, short &red, short &green, short &blue) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  HTCS2_I2CRequest[0] = 2;                               // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;                   // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_RED_NORM_REG; // Start red normalised sensor values

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 3))
//    return false;

//  red = HTCS2_I2CReply[0];
//  green = HTCS2_I2CReply[1];
//  blue = HTCS2_I2CReply[2];

//  return true;
//}


///**
// * Get the raw RGB readings, these are unsigned 16 bit values.
// * These values will only fit in a long
// * @param link the HTCS2 port number
// * @param passive when set to true, it will not use the builtin white LED and use ambient light instead
// * @param red the red value
// * @param green the green value
// * @param blue the blue value
// * @return true if no error occured, false if it did
// */
//bool HTCS2readRawRGB(tSensors link, bool passive, long &red, long &green, long &blue) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (passive && (active_mode[link] != HTCS2_MODE_PASSIVE))
//    _HTCSsendCommand(link, HTCS2_MODE_PASSIVE);
//  else if (!passive && (active_mode[link] != HTCS2_MODE_RAW))
//    _HTCSsendCommand(link, HTCS2_MODE_RAW);

//  HTCS2_I2CRequest[0] = 2;                               // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;                   // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_RED_MSB;  // Start red raw sensor value

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 8))
//    return false;

//  red =   (long)HTCS2_I2CReply[0] * 256 + HTCS2_I2CReply[1];
//  green = (long)HTCS2_I2CReply[2] * 256 + HTCS2_I2CReply[3];
//  blue =  (long)HTCS2_I2CReply[4] * 256 + HTCS2_I2CReply[5];

//  return true;
//}


///**
// * Get the raw white channel reading, is and unsigned 16 bit value.
// * This value will only fit in a long
// * @param link the HTCS2 port number
// * @param passive when set to true, it will not use the builtin white LED and use ambient light instead
// * @param white the white value
// * @return true if no error occured, false if it did
// */
//bool HTCS2readRawWhite(tSensors link, bool passive, long &white) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (passive && (active_mode[link] != HTCS2_MODE_PASSIVE))
//    _HTCSsendCommand(link, HTCS2_MODE_PASSIVE);
//  else if (!passive && (active_mode[link] != HTCS2_MODE_RAW))
//    _HTCSsendCommand(link, HTCS2_MODE_RAW);

//  HTCS2_I2CRequest[0] = 2;                               // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;                   // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_WHITE_MSB;  // Start red raw sensor value

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 2))
//    return false;

//  white = (long)HTCS2_I2CReply[0] * 256 + HTCS2_I2CReply[1];

//  return true;
//}


///**
// * Return the color index number currently detected. This is a single
// * 6 bit number color index. Bits 5 and 4 encode the red signal level,
// * bits 3 and 2 encode the green signal level and bits 1 and 0 encode
// * the blue signal levels.
// * @param link the HTCS2 port number
// * @return color index number or -1 if an error occurred.
// */
//short HTCS2readColorIndex(tSensors link) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  if (active_mode[link] != HTCS2_MODE_ACTIVE)
//    _HTCSsendCommand(link, HTCS2_MODE_ACTIVE);

//  HTCS2_I2CRequest[0] = 2;                                // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;                    // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_OFFSET + HTCS2_COL_INDEX_REG; // Start colour index register

//  if (!writeI2C(link, HTCS2_I2CRequest, HTCS2_I2CReply, 1))
//    return -1;

//  return HTCS2_I2CReply[0];
//}


///**
// * Send a command to the sensor to change its mode.
// * @param link the HTCS2 port number
// * @param command the command to be sent
// * @return true if no error occured, false if it did
// */
//bool _HTCSsendCommand(tSensors link, byte command) {
//  memset(HTCS2_I2CRequest, 0, sizeof(tByteArray));

//  HTCS2_I2CRequest[0] = 3;                  // Message size
//  HTCS2_I2CRequest[1] = HTCS2_I2C_ADDR;     // I2C Address
//  HTCS2_I2CRequest[2] = HTCS2_CMD_REG;      // Start colour index register
//  HTCS2_I2CRequest[3] = command;

//  if (command < 30)
//    active_mode[link] = command;

//  return writeI2C(link, HTCS2_I2CRequest);
//}

/**
 * Initialise the sensor's data struct and port
 *
 * @param htcs2Ptr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTCS2Ptr htcs2Ptr, tSensors port)
{
  memset(htcs2Ptr, 0, sizeof(tHTCS2Ptr));
  htcs2Ptr->I2CData.address = HTCS2_I2C_ADDR;
  htcs2Ptr->I2CData.port = port;
  htcs2Ptr->I2CData.type = sensorI2CCustom;
  htcs2Ptr->_cmd = HTCS2_MODE_ACTIVE;
  htcs2Ptr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htcs2Ptr->I2CData.port] != htcs2Ptr->I2CData.type)
    SensorType[htcs2Ptr->I2CData.port] = htcs2Ptr->I2CData.type;

  // Make sure the sensor is in the measurement mode
  return _sensorSendCommand(htcs2Ptr);
}


/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htcs2Ptr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTCS2Ptr htcs2Ptr, tMUXSensor muxsensor)
{
  memset(htcs2Ptr, 0, sizeof(tHTCS2Ptr));
  htcs2Ptr->I2CData.address = HTCS2_I2C_ADDR;
  htcs2Ptr->I2CData.type = sensorI2CCustom;
  htcs2Ptr->smux = true;
	htcs2Ptr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[htcs2Ptr->I2CData.port] != htcs2Ptr->I2CData.type)
    SensorType[htcs2Ptr->I2CData.port] = htcs2Ptr->I2CData.type;

  return HTSMUXconfigChannel(muxsensor, HTCS2_config);
}


/**
 * Read all the sensor's data
 *
 * @param htcs2Ptr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTCS2Ptr htcs2Ptr)
{
	memset(htcs2Ptr->I2CData.request, 0, sizeof(htcs2Ptr->I2CData.request));

	if (htcs2Ptr->smux)
	{
		if (!HTSMUXreadPort(htcs2Ptr->smuxport, htcs2Ptr->I2CData.reply, 4, HTCS2_COLNUM_REG))
			return false;
	}
	else
	{
	  // Read all of the data available on the sensor
	  htcs2Ptr->I2CData.request[0] = 2;                    // Message size
	  htcs2Ptr->I2CData.request[1] = htcs2Ptr->I2CData.address; // I2C Address
	  htcs2Ptr->I2CData.request[2] = HTCS2_OFFSET + HTCS2_COLNUM_REG;
	  htcs2Ptr->I2CData.replyLen = 4;
	  htcs2Ptr->I2CData.requestLen = 2;

	  if (!writeI2C(&htcs2Ptr->I2CData))
	    return false;
	}

	// Populate the struct with the newly retrieved data
  htcs2Ptr->color = htcs2Ptr->I2CData.reply[0];
  htcs2Ptr->red = htcs2Ptr->I2CData.reply[1];
  htcs2Ptr->green = htcs2Ptr->I2CData.reply[2];
  htcs2Ptr->blue = htcs2Ptr->I2CData.reply[3];
  RGBtoHSV(htcs2Ptr->red, htcs2Ptr->green, htcs2Ptr->blue, &htcs2Ptr->hue, &htcs2Ptr->saturation, &htcs2Ptr->value);

  return true;
}


bool readSensorNorm(tHTCS2Ptr htcs2Ptr)
{
	return true;
}


bool readSensorRaw(tHTCS2Ptr htcs2Ptr)
{
	return true;
}


/**
 * Send a command to the sensor
 *
 * Note: this is an internal function and should not be called directly.
 * @param htcs2Ptr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool _sensorSendCommand(tHTCS2Ptr htcs2Ptr) {
	bool retVal = false;

  memset(htcs2Ptr->I2CData.request, 0, sizeof(htcs2Ptr->I2CData.request));

  htcs2Ptr->I2CData.request[0] = 3;              // Message size
  htcs2Ptr->I2CData.request[1] = HTCS2_I2C_ADDR; // I2C Address
  htcs2Ptr->I2CData.request[2] = HTCS2_CMD_REG;  // Command register
  htcs2Ptr->I2CData.request[3] = htcs2Ptr->_cmd;        // Command to be sent

  retVal = writeI2C(&htcs2Ptr->I2CData);
  htcs2Ptr->_cmd = HTCS2_MODE_ACTIVE;

  return retVal;
}


#endif // __HTCS2_H__

/*
 * $Id: hitechnic-colour-v2.h $
 */
/* @} */
/* @} */
