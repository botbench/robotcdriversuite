/*!@addtogroup HiTechnic
 * @{
 * @defgroup grove IR Seeker V2
 * HiTechnic IR Seeker V2
 * @{
 */

#ifndef __GROVE_H__
#define __GROVE_H__
/** \file hitechnic-irseeker-v2.h
 * \brief HiTechnic IR Seeker V2 driver
 *
 * mindsensors-ev3-grove.h provides an API for the Mindsensors EV3-Grove Sensor.
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.29 AND HIGHER

 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 16 February 2015
 * \version 0.5
 * \example mindensors-ev3-grove-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

#define GROVE_I2C_ADDR    0x42      /*!< EV3 Grove I2C device address */

#define GROVE_COMMAND    	0x41      /*!< Commands register */
#define GROVE_OPMODE      0x42      /*!< Operation mode */
#define GROVE_POLL_INT    0x43      /*!< I2C auto poll interval */

#define GROVE_MODE_ANALOG	1					/*!< Read analogue voltage */
#define GROVE_MODE_DIG_0	2					/*!< Write digital 0 */
#define GROVE_MODE_DIG_1	3					/*!< Write digital 1 */
#define GROVE_MODE_I2C		4					/*!< I2C mode */

#define GROVE_CMD_AUTO_POLL		0x50			/*!< Enable auto-polling */
#define GROVE_CMD_READ_I2C		0x52			/*!< Read I2C */
#define GROVE_CMD_WRITE_I2C		0x54			/*!< Write I2C */

#define GROVE_ANALOG_LSB  0x44      /*!< Analog Read LSB */
#define GROVE_ANALOG_MSB  0x45      /*!< Analog Read LSB */

// I2C Read
#define GROVE_I2C_STATUS  0x46      /*!< I2C Status */
#define GROVE_I2C_RD_ADDR 0x47      /*!< Grove device I2C address */
#define GROVE_I2C_RD_LEN  0x48      /*!< Length to read */
#define GROVE_I2C_RD_REG  0x49      /*!< Grove device register address */
#define GROVE_I2C_RD_DAT  0x4A      /*!< Grove sensor data - up to 32 bytes */

// I2C Write
#define GROVE_I2C_WR_ADDR 0x6A      /*!< Grove device I2C address */
#define GROVE_I2C_WR_LEN  0x6B      /*!< Length to write */
#define GROVE_I2C_WR_REG  0x6C      /*!< Grove device register address */
#define GROVE_I2C_WR_DAT  0x6D      /*!< Grove sensor data - up to 32 bytes */

typedef enum tGroveSensor
{
	grove_DigitalLight,
	grove_AnalogLight
} tGroveSensor;

typedef struct
{
  ubyte address;
  ubyte register;
  ubyte requestLen;
  ubyte data[32];
} tGROVEI2CData;

typedef struct
{
  tI2CData I2CData;
	tGROVEI2CData read;
	tGROVEI2CData write;
	bool isAnalog;
	long analog;
	long pollTime;
	ubyte cmd;
	tGroveSensor sensorType;
} tGROVE, *tGROVEPtr;

bool initSensor(tGROVEPtr grovePtr, tSensors port, tGroveSensor type);
bool readSensor(tGROVEPtr grovePtr);
bool configSensor(tGROVEPtr grovePtr);
bool sendCmdSensor(tGROVEPtr grovePtr);


/**
 * Initialise the sensor's data struct and port
 *
 * @param grovePtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tGROVEPtr grovePtr, tSensors port, tGroveSensor type)
{
	//clearI2CError(GROVE, GROVE_I2C_ADDR);

  memset(grovePtr, 0, sizeof(tGROVEPtr));
  grovePtr->I2CData.address = GROVE_I2C_ADDR;
  grovePtr->I2CData.port = port;
#if defined(NXT)
  grovePtr->I2CData.type = sensorI2CCustom;
#elif defined (EV3)
  grovePtr->I2CData.type = sensorEV3_GenericI2C;
#endif
  grovePtr->pollTime = 50; // default poll time
  grovePtr->sensorType = type;

  // Ensure the sensor is configured correctly
  if (SensorType[grovePtr->I2CData.port] != grovePtr->I2CData.type)
    SensorType[grovePtr->I2CData.port] = grovePtr->I2CData.type;

  // Here all the sensor types are configured
  switch(grovePtr->sensorType)
  {
  	case grove_DigitalLight:
  	{
  		grovePtr->isAnalog = false;
  		grovePtr->write.address = 0x52;
  		grovePtr->write.register = 0x80;
  		grovePtr->write.data[0] = 0x03;
  		grovePtr->write.requestLen = 1;
  		grovePtr->pollTime = 50;
  		grovePtr->isAnalog = false;
  		grovePtr->read.address = 0x52;
  		grovePtr->read.register = 0x80;
  		grovePtr->read.requestLen = 4;
  		grovePtr->pollTime = 50;
  		break;
  		break;

  	}
  	case grove_AnalogLight:
  	{
  		grovePtr->isAnalog = true;
  		break;
  	}
  }
  // Make sure the sensor is configured properly
  return configSensor(grovePtr);
}

/**
 * Read all the sensor's data
 *
 * @param grovePtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tGROVEPtr grovePtr)
{
  memset(grovePtr->I2CData.request, 0, sizeof(grovePtr->I2CData.request));


	if (grovePtr->isAnalog)
	{
	  // Read the analogue data from the sensor
	  grovePtr->I2CData.request[0] = 2;                    // Message size
	  grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
	  grovePtr->I2CData.request[2] = GROVE_ANALOG_LSB;
	  grovePtr->I2CData.replyLen = 2;
	  grovePtr->I2CData.requestLen = 2;

	  if (!writeI2C(&grovePtr->I2CData))
	    return false;

	  grovePtr->analog = grovePtr->I2CData.reply[0] + (grovePtr->I2CData.reply[1] << 8);
	}
	// It's a digital sensor, so this is going to take a bit more work
	else
	{
	  grovePtr->I2CData.request[0] = 2;                    // Message size
	  grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
	  grovePtr->I2CData.request[2] = GROVE_I2C_RD_DAT;
	  grovePtr->I2CData.replyLen = grovePtr->read.requestLen;
	  grovePtr->I2CData.requestLen = 2;

		if (!writeI2C(&grovePtr->I2CData))
			return false;

		memcpy(grovePtr->read.data, grovePtr->I2CData.reply, grovePtr->read.requestLen);

	}

  return true;
}

bool configSensor(tGROVEPtr grovePtr)
{
	if (grovePtr->isAnalog)
	{
		grovePtr->I2CData.request[0] = 3;                    // Message size
		grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
		grovePtr->I2CData.request[2] = GROVE_OPMODE;
		grovePtr->I2CData.request[3] = GROVE_MODE_ANALOG;
		grovePtr->I2CData.replyLen = 0;
		grovePtr->I2CData.requestLen = 3;

		if (!writeI2C(&grovePtr->I2CData))
			return false;
	}
	else
	{
		// First setup for an I2C write
	  grovePtr->I2CData.request[0] = 4;                    // Message size
	  grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
	  grovePtr->I2CData.request[2] = GROVE_COMMAND;					// Send a command
	  grovePtr->I2CData.request[3] = GROVE_CMD_WRITE_I2C;		// Send an I2C write
	  grovePtr->I2CData.request[4] = GROVE_MODE_I2C;				// Set mode to I2C
	  grovePtr->I2CData.replyLen = 0;
	  grovePtr->I2CData.requestLen = 4;

		if (!writeI2C(&grovePtr->I2CData))
			return false;

		// Now configure the Grove adapter's grove sensor I2C data,
		// this will configure the grove sensor
	  grovePtr->I2CData.request[0] = 5 + grovePtr->write.requestLen;	// Message size
	  grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
	  grovePtr->I2CData.request[2] = GROVE_I2C_WR_ADDR;
	  grovePtr->I2CData.request[3] = grovePtr->write.address;
	  grovePtr->I2CData.request[4] = grovePtr->write.requestLen;
	  grovePtr->I2CData.request[5] = grovePtr->write.register;
	  memcpy(grovePtr->I2CData.request[6], grovePtr->write.data[0], grovePtr->write.requestLen);
	  grovePtr->I2CData.replyLen = 0;
	  grovePtr->I2CData.requestLen = 5 + grovePtr->write.requestLen;

		if (!writeI2C(&grovePtr->I2CData))
			return false;

		// Next, enable auto-polling
	  grovePtr->I2CData.request[0] = 5;                    // Message size
	  grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
	  grovePtr->I2CData.request[2] = GROVE_COMMAND;					// Send a command
	  grovePtr->I2CData.request[3] = GROVE_CMD_AUTO_POLL;		// Send an I2C write
	  grovePtr->I2CData.request[4] = GROVE_MODE_I2C;				// Set mode to I2C
	  grovePtr->I2CData.request[5] = grovePtr->pollTime ;
	  grovePtr->I2CData.replyLen = 0;
	  grovePtr->I2CData.requestLen = 5;

		if (!writeI2C(&grovePtr->I2CData))
			return false;

		// Finally, configure the Grove adapter to read the grove sensor
	  grovePtr->I2CData.request[0] = 5;	// Message size
	  grovePtr->I2CData.request[1] = grovePtr->I2CData.address; // I2C Address
	  grovePtr->I2CData.request[2] = GROVE_I2C_RD_ADDR;
	  grovePtr->I2CData.request[3] = grovePtr->read.address;
	  grovePtr->I2CData.request[4] = grovePtr->read.requestLen;
	  grovePtr->I2CData.request[5] = grovePtr->read.register;
	  grovePtr->I2CData.replyLen = 0;
	  grovePtr->I2CData.requestLen = 5;

		if (!writeI2C(&grovePtr->I2CData))
			return false;
	}

	return true;
}

bool sendCmdSensor(tGROVEPtr grovePtr)
{
	return true;
}

#endif // __GROVE_H__

/* @} */
/* @} */
