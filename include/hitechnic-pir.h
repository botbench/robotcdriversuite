/*!@addtogroup HiTechnic
 * @{
 * @defgroup HTPIR PIR Sensor
 * HiTechnic PIR Sensor
 * @{
 */

/** \file hitechnic-pir.h
 * \brief HiTechnic PIR Sensor Driver
 *
 * hitechnic-pir.h provides an API for the HiTechnic PIR Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 15 August 2012
 * \version 0.1
 * \example hitechnic-pir-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

// I2C address + registers
#define HTPIR_I2C_ADDR       0x02  /*!< HTPIR I2C device address */
#define HTPIR_DEADBAND       0x41  /*!< HTPIR Mode control */
#define HTPIR_OFFSET         0x42  /*!< Offset for data registers */

// Values contained by registers in active mode
#define HTPIR_MSB           0x00  /*!< MSB register */

#define HTPIR_DEFAULT_DEADBAND 12

typedef struct
{
  tI2CData I2CData;
  long value;
  long deadband;
} tHTPIR, *tHTPIRPtr;

bool initSensor(tHTPIRPtr htpirPtr, tSensors port);
bool readSensor(tHTPIRPtr htpirPtr);
bool resetSensor(tHTPIRPtr htpirPtr);
bool sensorCalibrate(tHTPIRPtr htpirPtr);


/**
 * Initialise the sensor's data struct and port
 *
 * @param htpirPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTPIRPtr htpirPtr, tSensors port)
{
  memset(htpirPtr, 0, sizeof(tHTPIRPtr));
  htpirPtr->I2CData.address = HTPIR_I2C_ADDR;
  htpirPtr->I2CData.port = port;
  htpirPtr->deadband = HTPIR_DEFAULT_DEADBAND;
#if defined(NXT)
  htpirPtr->I2CData.type = sensorI2CCustom;
#else
	htpirPtr->I2CData.type = sensorEV3_GenericI2C;
#endif

  // Ensure the sensor is configured correctly
  if (SensorType[htpirPtr->I2CData.port] != htpirPtr->I2CData.type)
    SensorType[htpirPtr->I2CData.port] = htpirPtr->I2CData.type;

  sleep(50);
  writeDebugStreamLine("type: %d", SensorType[htpirPtr->I2CData.port]);
  // Make sure the sensor is in the measurement mode
  return sensorCalibrate(htpirPtr);
}


/**
 * Read all the sensor's data
 *
 * @param htpirPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTPIRPtr htpirPtr)
{
  memset(htpirPtr->I2CData.request, 0, sizeof(htpirPtr->I2CData.request));


  // Read all of the data available on the sensor
  htpirPtr->I2CData.request[0] = 2;                    // Message size
  htpirPtr->I2CData.request[1] = htpirPtr->I2CData.address; // I2C Address
  htpirPtr->I2CData.request[2] = HTPIR_OFFSET + HTPIR_MSB;
  htpirPtr->I2CData.replyLen = 1;
  htpirPtr->I2CData.requestLen = 2;

  if (!writeI2C(&htpirPtr->I2CData))
    return false;

  // Populate the struct with the newly retrieved data
  htpirPtr->value = (htpirPtr->I2CData.reply[0] >= 128) ? (short)htpirPtr->I2CData.reply[0] - 256 : (short)htpirPtr->I2CData.reply[0];

  return true;
}


/**
 * The sensor element with the PIR sensor generates continuous noise.
 * The size of the deadband is set to minimize the number of false
 * detections which will be reported by the sensor. If the deadband
 * value is too small, some of the noise fluctuations will exceed the
 * deadband threshold and will appear as actual non-zero readings. \n
 * The Deadband field may be set from 0 to 47 to define the half width
 * of the deadband. The default value is 12.
 * @param htpirPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool sensorCalibrate(tHTPIRPtr htpirPtr)
{
  memset(htpirPtr->I2CData.request, 0, sizeof(htpirPtr->I2CData.request));

  htpirPtr->I2CData.request[0] = 3;              // Message size
  htpirPtr->I2CData.request[1] = HTPIR_I2C_ADDR; // I2C Address
  htpirPtr->I2CData.request[2] = HTPIR_DEADBAND;  // Command register
  htpirPtr->I2CData.request[3] =  (ubyte)(htpirPtr->deadband & 0x000000FF);        // Cdeadband to configure

  return writeI2C(&htpirPtr->I2CData);
}

/* @} */
/* @} */
