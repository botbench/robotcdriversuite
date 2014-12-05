/*!@addtogroup HiTechnic
 * @{
 * @defgroup htmc Compass Sensor
 * HiTechnic Compass Sensor
 * @{
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
#define HTMC_OFFSET          0x42  /*!< Offset for data registers */
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

tConfigParams HTMC_config = {HTSMUX_CHAN_I2C, 2, 0x02, 0x42}; /*!< Array to hold SMUX config data for sensor */

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
  htmcPtr->I2CData.port = (tSensors)SPORT(muxsensor);
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

/* @} */
/* @} */
