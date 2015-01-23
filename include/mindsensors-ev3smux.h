/*!@addtogroup Mindsensors
 * @{
 * @defgroup MSEV3SMUX
 * Mindsensors EV3 Sensor MUX
 * @{
 */

#ifndef __MSEV3SMUX_H__
#define __MSEV3SMUX_H__
/** \file mindsensors-ev3-smux.h
 * \brief Mindensors EV3 Sensor MUX
 *
 * mindsensor-ev3smux.h provides an API for the Mindsensors EV3 Sensor MUX
 *
 * Changelog:
 * - 0.1: Initial release
 *
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 14 December 2014
 * \version 0.1
 * \example mindsensors-ev3smux-test1.c
 * \example mindsensors-ev3smux-test1.c
 */

// #pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define MSEV3_I2C_ADDR_CHAN1   0xA0      /*!< MSEV3 I2C device address */
#define MSEV3_I2C_ADDR_CHAN2   0xA2      /*!< MSEV3 I2C device address */
#define MSEV3_I2C_ADDR_CHAN3   0xA4      /*!< MSEV3 I2C device address */
#define MSEV3_CMD_REG          0x52      /*!< Command register */
#define MSEV3_DATA_REG         0x54      /*!< Data registers */

// Taken from Mindsensor's NXC lib
typedef enum tEV3SensorTypeMode
{
	colorReflectedLight = 0x00,   /*!< for measuring reflected light from sensor */
	colorAmbientLight   = 0x01,   /*!< for measuring ambient light around the sensor */
	colorMeasureColor   = 0x02,   /*!< to measure color */
	gyroAngle           = 0x10,   /*!< for Angle measurements */
	gyroRate            = 0x11,   /*!< for rate of change measurement */
	infraRedProximity   = 0x20,   /*!< for measuring Proximity with EV3 Infrared sensor */
	infraRedBeacon      = 0x21,   /*!< for measuring in Mode Beacon (returns 8 bytes values - 2 per channel) */
	infraRedRemote      = 0x22,   /*!< for Remote mode  */
	sonarCM             = 0x30,   /*!< for measurements in centimeters */
	sonarInches         = 0x31,   /*!< measurements in inches */
	sonarPresence       = 0x32,   /*!< for presence detection mode */
	touchStateBump			= 0x4F,		/*!< pressed (1) or not pressed (0) */
} tEV3SensorTypeMode;

typedef struct
{
  tI2CData I2CData;
  bool touch;
  short bumpCount;
  short light;
  short color;
  short angle;
  short rate;
  short distance;
  short beaconProx[4];
  short beaconHeading[4];
  short remote[4];
  bool presence;
  tEV3SensorTypeMode typeMode;
  ubyte _cmd;
} tMSEV3, *tMSEV3Ptr;

bool initSensor(tMSEV3Ptr msev3Ptr, tMUXSensor muxsensor, tEV3SensorTypeMode typeMode);
bool readSensor(tMSEV3Ptr msev3Ptr);
bool _sensorSendCommand(tMSEV3Ptr msev3Ptr);

/**
 * Initialise the sensor's data struct and port
 *
 * @param msev3Ptr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tMSEV3Ptr msev3Ptr, tMUXSensor muxsensor, tEV3SensorTypeMode typeMode)
{
	memset(msev3Ptr, 0, sizeof(tMSEV3Ptr));

	switch (MPORT(muxsensor))
	{
		case 0: msev3Ptr->I2CData.address = MSEV3_I2C_ADDR_CHAN1; break;
		case 1: msev3Ptr->I2CData.address = MSEV3_I2C_ADDR_CHAN2; break;
		case 2: msev3Ptr->I2CData.address = MSEV3_I2C_ADDR_CHAN3; break;
		default: return false; // There are only 3 ports on the SMUX
	}

	msev3Ptr->typeMode = typeMode;
  msev3Ptr->I2CData.port = (tSensors)SPORT(muxsensor);
#if defined(NXT)
  msev3Ptr->I2CData.type = sensorI2CCustom;
#else
  msev3Ptr->I2CData.type = sensorEV3_GenericI2C;
#endif
  msev3Ptr->_cmd = (ubyte)msev3Ptr->typeMode & 0x0F;

  // Ensure the sensor is configured correctly
  if (SensorType[msev3Ptr->I2CData.port] != msev3Ptr->I2CData.type)
  {
    writeDebugStreamLine("Port[%d] not configured properly (type: %d), reconfiguring", msev3Ptr->I2CData.port, SensorType[msev3Ptr->I2CData.port]);
    SensorType[msev3Ptr->I2CData.port] = msev3Ptr->I2CData.type;
    sleep(1000);
  }

  return _sensorSendCommand(msev3Ptr);
  //return true;
}

/**
 * Read all the sensor's data
 *
 * @param msev3Ptr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tMSEV3Ptr msev3Ptr)
{
  memset(msev3Ptr->I2CData.request, 0, sizeof(msev3Ptr->I2CData.request));

	// Request only the number of bytes required.
 	switch(msev3Ptr->typeMode)
 	{

	  case infraRedProximity:
 		case sonarPresence:
 			msev3Ptr->I2CData.replyLen = 1;
 			break;

 		case touchStateBump:
 		case colorReflectedLight:
 		case colorAmbientLight:
 		case colorMeasureColor:
	  case gyroAngle:
	  case gyroRate:
	  case sonarCM:
	  case sonarInches:
	   	msev3Ptr->I2CData.replyLen = 2;
 			break;

 		case infraRedRemote:
	  	msev3Ptr->I2CData.replyLen = 4;
 			break;

	  case infraRedBeacon:
	   	msev3Ptr->I2CData.replyLen = 8;
 			break;
	}

  // Read all of the data available on the sensor
  msev3Ptr->I2CData.request[0] = 2;                    // Message size
  msev3Ptr->I2CData.request[1] = msev3Ptr->I2CData.address; // I2C Address
  msev3Ptr->I2CData.request[2] = MSEV3_DATA_REG;
  msev3Ptr->I2CData.requestLen = 2;

  if (!writeI2C(&msev3Ptr->I2CData))
  {
    return false;
  }

 	switch(msev3Ptr->typeMode)
 	{
 		case touchStateBump:
 			msev3Ptr->touch = (msev3Ptr->I2CData.reply[0] == 1) ? true : false;
 			msev3Ptr->bumpCount = msev3Ptr->I2CData.reply[1];
 			break;

 		case colorReflectedLight:
 			msev3Ptr->light = msev3Ptr->I2CData.reply[0] + (msev3Ptr->I2CData.reply[1] << 8);
 			break;
 		case colorAmbientLight:
 			msev3Ptr->light = msev3Ptr->I2CData.reply[0] + (msev3Ptr->I2CData.reply[1] << 8);
 			break;
 		case colorMeasureColor:
 			msev3Ptr->color = msev3Ptr->I2CData.reply[0] + (msev3Ptr->I2CData.reply[1] << 8);
 			break;

	  case gyroAngle:
	  	msev3Ptr->angle = (short)(msev3Ptr->I2CData.reply[0] + (msev3Ptr->I2CData.reply[1] << 8));
	  	break;
	  case gyroRate:
	  	msev3Ptr->rate = (short)(msev3Ptr->I2CData.reply[0] + (msev3Ptr->I2CData.reply[1] << 8));
	  	break;

	  case infraRedProximity:
	  	msev3Ptr->distance = msev3Ptr->I2CData.reply[0];
	  	break;
	  case infraRedBeacon:
			for (int i = 0; i < 4; i++)
			{
	  		msev3Ptr->beaconProx[i] = msev3Ptr->I2CData.reply[i*2];
	  		msev3Ptr->beaconHeading[i] = msev3Ptr->I2CData.reply[(i*2)+1];
	  	}
	  	break;
	  case infraRedRemote:
	  	msev3Ptr->remote[0] = msev3Ptr->I2CData.reply[0];
	  	msev3Ptr->remote[1] = msev3Ptr->I2CData.reply[1];
	  	msev3Ptr->remote[2] = msev3Ptr->I2CData.reply[2];
	  	msev3Ptr->remote[3] = msev3Ptr->I2CData.reply[3];
	  	break;

	  case sonarCM:
		case sonarInches:
	  	msev3Ptr->distance = msev3Ptr->I2CData.reply[0] + (msev3Ptr->I2CData.reply[1] << 8);
	  	break;
	  case sonarPresence:
		  msev3Ptr->presence = (msev3Ptr->I2CData.reply[0] == 1) ? true : false;
	  	break;

 		default: return false;
 	}
  return true;
}


/**
 * Send a command to the sensor
 *
 * Note: this is an internal function and should not be called directly.
 * @param msev3Ptr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool _sensorSendCommand(tMSEV3Ptr msev3Ptr) {
  bool retVal = false;

  memset(msev3Ptr->I2CData.request, 0, sizeof(msev3Ptr->I2CData.request));

  msev3Ptr->I2CData.request[0] = 3;              						// Message size
  msev3Ptr->I2CData.request[1] = msev3Ptr->I2CData.address; // I2C Address
  msev3Ptr->I2CData.request[2] = MSEV3_CMD_REG;  						// Command register
  msev3Ptr->I2CData.request[3] = msev3Ptr->_cmd;  					// Command to be sent

  retVal = writeI2C(&msev3Ptr->I2CData);

  return retVal;
}


#endif // __MSEV3SMUX_H__

/* @} */
/* @} */
