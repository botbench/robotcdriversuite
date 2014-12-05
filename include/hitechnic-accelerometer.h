/*!@addtogroup HiTechnic
 * @{
 * @defgroup htacc Acceleration Sensor
 * HiTechnic Acceleration Sensor
 * @{
 */

#ifndef __HTAC_H__
#define __HTAC_H__
/** \file hitechnic-accelerometer.h
 * \brief HiTechnic Acceleration Sensor driver
 *
 * hitechnic-accelerometer.h provides an API for the HiTechnic Acceleration Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Fixed bad registers<br>
 *        Added HTACreadAllAxes(tSensors link, short &x, short &y, short &z)<br>
 *        Removed HTACreadAllAxes(tSensors link, tIntArray &data)<br>
 *        Changed HTACreadX, Y, Z to use by reference instead of as return value<br>
 * - 0.3: SMUX functions added.
 * - 0.4: Removed HTAC_SMUXData, reused HTAC_I2CReply to save memory
 * - 0.5: Use new calls in common.h that don't require SPORT/MPORT macros<br>
 *        Fixed massive bug in HTACreadAllAxes() in the way values are calculated
 * - 0.6: Removed single axis functions
 * - 0.7: Replaced array structs with typedefs
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 20 February 2011
 * \version 0.7
 * \example hitechnic-accelerometer-test1.c
 * \example hitechnic-accelerometer-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTAC_I2C_ADDR  0x02      /*!< IR Seeker I2C device address */
#define HTAC_OFFSET    0x42      /*!< Offset for data registers */
#define HTAC_X_UP      0x00      /*!< X axis upper 8 bits */
#define HTAC_Y_UP      0x01      /*!< Y axis upper 8 bits */
#define HTAC_Z_UP      0x02      /*!< Z axis upper 8 bits */
#define HTAC_X_LOW     0x03      /*!< X axis lower 2 bits */
#define HTAC_Y_LOW     0x04      /*!< Y axis lower 2 bits */
#define HTAC_Z_LOW     0x05      /*!< Z axis lower 2 bits */

typedef struct
{
  tI2CData I2CData;
  short x;
  short y;
  short z;
  short axes[3];
  bool smux;
  tMUXSensor smuxport;
} tHTAC, *tHTACPtr;

bool initSensor(tHTACPtr htacPtr, tSensors port);
bool initSensor(tHTACPtr htacPtr, tMUXSensor muxsensor);
bool readSensor(tHTACPtr htacPtr);

tConfigParams HTAC_config = {HTSMUX_CHAN_I2C, 6, 0x02, 0x42}; /*!< Array to hold SMUX config data for sensor */

/**
 * Initialise the sensor's data struct and port
 *
 * @param htacPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTACPtr htacPtr, tSensors port)
{
  memset(htacPtr, 0, sizeof(tHTACPtr));
  htacPtr->I2CData.address = HTAC_I2C_ADDR;
  htacPtr->I2CData.port = port;
  htacPtr->I2CData.type = sensorI2CCustom;

  htacPtr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htacPtr->I2CData.port] != htacPtr->I2CData.type)
    SensorType[htacPtr->I2CData.port] = htacPtr->I2CData.type;

  return true;
}

/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htacPtr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTACPtr htacPtr, tMUXSensor muxsensor)
{
  memset(htacPtr, 0, sizeof(tHTACPtr));
  htacPtr->I2CData.port = (tSensors)SPORT(muxsensor);
  htacPtr->I2CData.address = HTAC_I2C_ADDR;
  htacPtr->I2CData.type = sensorI2CCustom;
  htacPtr->smux = true;
  htacPtr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[htacPtr->I2CData.port] != htacPtr->I2CData.type)
    SensorType[htacPtr->I2CData.port] = htacPtr->I2CData.type;

  return HTSMUXconfigChannel(muxsensor, HTAC_config);
}

/**
 * Read all the sensor's data
 *
 * @param htacPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTACPtr htacPtr)
{
  memset(htacPtr->I2CData.request, 0, sizeof(htacPtr->I2CData.request));

  if (htacPtr->smux)
  {
    if (!HTSMUXreadPort(htacPtr->smuxport, htacPtr->I2CData.reply, 6, HTAC_X_UP))
      return false;
  }
  else
  {
    // Read all of the data available on the sensor
    htacPtr->I2CData.request[0] = 2;                    // Message size
    htacPtr->I2CData.request[1] = htacPtr->I2CData.address; // I2C Address
    htacPtr->I2CData.request[2] = HTAC_OFFSET + HTAC_X_UP;
    htacPtr->I2CData.replyLen = 6;
    htacPtr->I2CData.requestLen = 2;

    if (!writeI2C(&htacPtr->I2CData))
      return false;
  }

  // Populate the struct with the newly retrieved data
  // Convert 2 bytes into a signed 10 bit value.  If the 8 high bits are more than 127, make
  // it a signed value before combing it with the lower 2 bits.
  // Gotta love conditional assignments!
  htacPtr->x = (htacPtr->I2CData.reply[0] > 127) ? (htacPtr->I2CData.reply[0] - 256) * 4 + htacPtr->I2CData.reply[3]
                                   : htacPtr->I2CData.reply[0] * 4 + htacPtr->I2CData.reply[3];

  htacPtr->y = (htacPtr->I2CData.reply[1] > 127) ? (htacPtr->I2CData.reply[1] - 256) * 4 + htacPtr->I2CData.reply[4]
                                   : htacPtr->I2CData.reply[1] * 4 + htacPtr->I2CData.reply[4];

  htacPtr->z = (htacPtr->I2CData.reply[2] > 127) ? (htacPtr->I2CData.reply[2] - 256) * 4 + htacPtr->I2CData.reply[5]
                                   : htacPtr->I2CData.reply[2] * 4 + htacPtr->I2CData.reply[5];

	htacPtr->axes[0] = htacPtr->x;
	htacPtr->axes[1] = htacPtr->y;
	htacPtr->axes[2] = htacPtr->z;

  return true;
}

#endif // __HTAC_H__

/* @} */
/* @} */
