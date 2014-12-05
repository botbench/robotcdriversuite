/*!@addtogroup HiTechnic
 * @{
 * @defgroup htirs2 IR Seeker V2
 * HiTechnic IR Seeker V2
 * @{
 */

#ifndef __HTIRS2_H__
#define __HTIRS2_H__
/** \file hitechnic-irseeker-v2.h
 * \brief HiTechnic IR Seeker V2 driver
 *
 * hitechnic-irseeker-v2.h provides an API for the HiTechnic IR Seeker V2.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Added SMUX functions
 * - 0.3: All functions using tIntArray are now pass by reference.<br>
 *        HTIRS2_SMUXData removed
 * - 0.4: Removed all calls to ubyteToInt()<br>
 *        Replaced all functions that used SPORT/MPORT macros
 * - 0.5: Driver renamed to HTIRS2
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 06 April 2010
 * \version 0.5
 * \example hitechnic-irseeker-v2-test1.c
 * \example hitechnic-irseeker-v2-enhanced-test1.c
 * \example hitechnic-irseeker-v2-SMUX-test1.c
 * \example hitechnic-irseeker-v2-enhanced-SMUX-test1.c
 */

#pragma systemFile
#include "hitechnic-sensormux.h"

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTIRS2_I2C_ADDR    0x10      /*!< IR Seeker I2C device address */
#define HTIRS2_DSP_MODE    0x41      /*!< AC DSP mode - 0 = 1200Hz, 1 = 600Hz */
#define HTIRS2_OFFSET      0x42      /*!< Offset for data registers */
#define HTIRS2_DC_DIR      0x00      /*!< DC Direction data */
#define HTIRS2_DC_SSTR1    0x01      /*!< DC Sensor 0 signal strength above average */
#define HTIRS2_DC_SSTR2    0x02      /*!< DC Sensor 1 signal strength above average */
#define HTIRS2_DC_SSTR3    0x03      /*!< DC Sensor 2 signal strength above average */
#define HTIRS2_DC_SSTR4    0x04      /*!< DC Sensor 3 signal strength above average */
#define HTIRS2_DC_SSTR5    0x05      /*!< DC Sensor 4 signal strength above average */
#define HTIRS2_DC_SAVG     0x06      /*!< DC sensor signal strength average */
#define HTIRS2_AC_DIR      0x07      /*!< AC Direction data */
#define HTIRS2_AC_SSTR1    0x08      /*!< AC Sensor 0 signal strength above average */
#define HTIRS2_AC_SSTR2    0x09      /*!< AC Sensor 1 signal strength above average */
#define HTIRS2_AC_SSTR3    0x0A      /*!< AC Sensor 2 signal strength above average */
#define HTIRS2_AC_SSTR4    0x0B      /*!< AC Sensor 3 signal strength above average */
#define HTIRS2_AC_SSTR5    0x0C      /*!< AC Sensor 4 signal strength above average */

/*!< AC DSP modes */
typedef enum tHTIRS2DSPMode {
  DSP_1200 = 0,
  DSP_600 = 1
} tHTIRS2DSPMode;

typedef struct
{
  tI2CData I2CData;
  long dcDirection;
  long acDirection;
  long enhStrength;
  long enhDirection;
  long acValues[5];
  long dcValues[5];
  long dcAvg;
  tHTIRS2DSPMode mode;
  bool smux;
  tMUXSensor smuxport;
} tHTIRS2, *tHTIRS2Ptr;

bool initSensor(tHTIRS2Ptr htirs2Ptr, tSensors port);
bool initSensor(tHTIRS2Ptr htirs2Ptr, tMUXSensor muxsensor);
bool readSensor(tHTIRS2Ptr htirs2Ptr);
bool configSensor(tHTIRS2Ptr htirs2Ptr);

tConfigParams HTIRS2_config = {HTSMUX_CHAN_I2C, 13, 0x10, 0x42}; /*!< Array to hold SMUX config data for sensor */

/**
 * Initialise the sensor's data struct and port
 *
 * @param htirs2Ptr pointer to the sensor's data struct
 * @param port the sensor port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTIRS2Ptr htirs2Ptr, tSensors port)
{
  memset(htirs2Ptr, 0, sizeof(tHTIRS2Ptr));
  htirs2Ptr->I2CData.address = HTIRS2_I2C_ADDR;
  htirs2Ptr->I2CData.port = port;
  htirs2Ptr->I2CData.type = sensorI2CCustom;
  htirs2Ptr->mode = DSP_1200;
  htirs2Ptr->smux = false;

  // Ensure the sensor is configured correctly
  if (SensorType[htirs2Ptr->I2CData.port] != htirs2Ptr->I2CData.type)
    SensorType[htirs2Ptr->I2CData.port] = htirs2Ptr->I2CData.type;

  // Make sure the sensor is in the measurement mode
  return configSensor(htirs2Ptr);
}

/**
 * Initialise the sensor's data struct and MUX port
 *
 * @param htirs2Ptr pointer to the sensor's data struct
 * @param muxsensor the sensor MUX port
 * @return true if no error occured, false if it did
 */
bool initSensor(tHTIRS2Ptr htirs2Ptr, tMUXSensor muxsensor)
{
  memset(htirs2Ptr, 0, sizeof(tHTIRS2Ptr));
  htirs2Ptr->I2CData.port = (tSensors)SPORT(muxsensor);
  htirs2Ptr->I2CData.address = HTIRS2_I2C_ADDR;
  htirs2Ptr->I2CData.type = sensorI2CCustom;
  htirs2Ptr->mode = DSP_1200;
  htirs2Ptr->smux = true;
  htirs2Ptr->smuxport = muxsensor;

  // Ensure the sensor is configured correctly
  if (SensorType[htirs2Ptr->I2CData.port] != htirs2Ptr->I2CData.type)
    SensorType[htirs2Ptr->I2CData.port] = htirs2Ptr->I2CData.type;

  return HTSMUXconfigChannel(muxsensor, HTIRS2_config);
}

/**
 * Read all the sensor's data
 *
 * @param htirs2Ptr pointer to the sensor's data struct
 * @return true if no error occured, false if it did
 */
bool readSensor(tHTIRS2Ptr htirs2Ptr)
{
	long dcSigSum = 0;
	long iMax = 0;

  memset(htirs2Ptr->I2CData.request, 0, sizeof(htirs2Ptr->I2CData.request));

  if (htirs2Ptr->smux)
  {
    if (!HTSMUXreadPort(htirs2Ptr->smuxport, htirs2Ptr->I2CData.reply, 13, HTIRS2_DC_DIR))
      return false;
  }
  else
  {
    // Read all of the data available on the sensor
    htirs2Ptr->I2CData.request[0] = 2;                    // Message size
    htirs2Ptr->I2CData.request[1] = htirs2Ptr->I2CData.address; // I2C Address
    htirs2Ptr->I2CData.request[2] = HTIRS2_OFFSET + HTIRS2_DC_DIR;
    htirs2Ptr->I2CData.replyLen = 13;
    htirs2Ptr->I2CData.requestLen = 2;

    if (!writeI2C(&htirs2Ptr->I2CData))
      return false;
  }

  // Populate the struct with the newly retrieved data
  htirs2Ptr->dcDirection = htirs2Ptr->I2CData.reply[0];

  for (int i = 0; i < 5; i++)
  {
  	htirs2Ptr->dcValues[i] = htirs2Ptr->I2CData.reply[HTIRS2_DC_SSTR1 + i];
  }

  htirs2Ptr->dcAvg = htirs2Ptr->I2CData.reply[HTIRS2_DC_SAVG];

  htirs2Ptr->acDirection = htirs2Ptr->I2CData.reply[HTIRS2_AC_DIR];

  for (int i = 0; i < 5; i++)
  {
  	htirs2Ptr->acValues[i] = htirs2Ptr->I2CData.reply[HTIRS2_AC_SSTR1 + i];
  }

  // Find the max DC sig strength
  for (short i = 0; i < 5; i++)
  {
    if (htirs2Ptr->dcValues[i] > htirs2Ptr->dcValues[iMax])
      iMax = i;
  }

  // Calc base DC direction value
  htirs2Ptr->enhDirection = iMax * 2 + 1;
  // Set base dcStrength based on max signal and average
  dcSigSum = htirs2Ptr->dcValues[iMax] +htirs2Ptr->dcAvg;

  // Check signal strength of neighbouring sensor elements
  if ((iMax > 0) && (htirs2Ptr->dcValues[iMax - 1] > (htirs2Ptr->dcValues[iMax] / 2)))
  {
      htirs2Ptr->enhDirection--;
      dcSigSum += htirs2Ptr->dcValues[iMax - 1];
  }

  if ((iMax < 4) && (htirs2Ptr->dcValues[iMax + 1] > (htirs2Ptr->dcValues[iMax] / 2)))
  {
      htirs2Ptr->enhDirection++;
      dcSigSum += htirs2Ptr->dcValues[iMax + 1];
  }

  // Make DC strength compatible with AC strength.
  // use: sqrt(dcSigSum*500)
  htirs2Ptr->enhStrength = sqrt(dcSigSum * 500);

  // Decide if using AC strength
  if ((htirs2Ptr->enhStrength <= 200) && (htirs2Ptr->enhDirection > 0))  {
    // Sum the sensor elements to get strength
    htirs2Ptr->enhStrength = htirs2Ptr->acValues[0] + htirs2Ptr->acValues[1] +
               							 htirs2Ptr->acValues[2] + htirs2Ptr->acValues[3] +
               							 htirs2Ptr->acValues[4];
  }
  return true;

}

bool configSensor(tHTIRS2Ptr htirs2Ptr)
{
	// this does not work for SMUX connected sensors
	if (htirs2Ptr->smux)
		return true;

  memset(htirs2Ptr->I2CData.request, 0, sizeof(htirs2Ptr->I2CData.request));

	// Configure the DSP mode
	htirs2Ptr->I2CData.request[0] = 3;                    // Message size
	htirs2Ptr->I2CData.request[1] = htirs2Ptr->I2CData.address; // I2C Address
	htirs2Ptr->I2CData.request[2] = HTIRS2_DSP_MODE;
	htirs2Ptr->I2CData.request[3] = (ubyte)htirs2Ptr->mode;
	htirs2Ptr->I2CData.replyLen = 0;
	htirs2Ptr->I2CData.requestLen = 3;

	return writeI2C(&htirs2Ptr->I2CData);
}


#endif // __HTIRS2_H__

/* @} */
/* @} */
