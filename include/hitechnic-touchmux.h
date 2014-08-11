/*!@addtogroup HiTechnic
 * @{
 * @defgroup httmux Touch Sensor MUX
 * HiTechnic Touch Sensor MUX
 * @{
 */

#ifndef __HTTMUX_H__
#define __HTTMUX_H__
/** \file hitechnic-touchmux.h
 * \brief HiTechnic Touch Sensor Multiplexer Sensor driver
 *
 * hitechnic-touchmux.h provides an API for the HiTechnic Touch Sensor Multiplexer Sensor.
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
 * \date 15 March 2009
 * \version 0.1
 * \example hitechnic-touchmux-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

typedef struct
{
  tI2CData I2CData;
  ubyte statusMask;
  bool status[4];
} tHTTMUX, *tHTTMUXPtr;

bool initSensor(tHTTMUXPtr httmuxPtr, tSensors port);
bool readSensor(tHTTMUXPtr httmuxPtr);


/**
 * Initialise the sensor's data struct and port
 *
 * @param httmuxPtr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTTMUXPtr httmuxPtr, tSensors port)
{
  memset(httmuxPtr, 0, sizeof(tHTTMUXPtr));
  httmuxPtr->I2CData.port = port;
  httmuxPtr->I2CData.type = sensorRawValue;

  // Ensure the sensor is configured correctly
  if (SensorType[httmuxPtr->I2CData.port] != httmuxPtr->I2CData.type)
    SensorType[httmuxPtr->I2CData.port] = httmuxPtr->I2CData.type;

  return true;
}


/**
 * Read all the sensor's data
 *
 * @param httmuxPtr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTTMUXPtr httmuxPtr)
{
  long muxvalue = 0;
  long switches = 0;

  // Voodoo magic starts here.  This is taken straight from the Touch MUX pamphlet.
  // No small furry animals were hurt during the calculation of this algorithm.
  muxvalue = 1023 - SensorRaw[httmuxPtr->I2CData.port];
  switches = 339 * muxvalue;
  switches /= (1023 - muxvalue);
  switches += 5;
  switches /= 10;

  httmuxPtr->statusMask = switches;
  for (int i = 0; i < 4; i++)
  {
  	httmuxPtr->status[i] = (httmuxPtr->statusMask & (1 << i)) ? true : false;
  }

  return true;
}
#endif // __HTTMUX_H__

/* @} */
/* @} */
