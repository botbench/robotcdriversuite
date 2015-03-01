/*!@addtogroup Dexter_Industries
* @{
* @defgroup DIMC 3D Compass Sensor
* Dexter Industries DIMC 3D Compass Sensor driver
* @{
*/

#ifndef __DIMC_H__
#define __DIMC_H__
/** \file dexterind-compass.h
* \brief Dexter Industries IMU Sensor driver
*
* dexterind-compass.h provides an API for the Dexter Industries compass Sensor.\n
*
* Changelog:
* - 0.1: Initial release
*
* Credits:
* - Big thanks to Dexter Industries for providing me with the hardware necessary to write and test this.
*
* License: You may use this code as you wish, provided you give credit where its due.
*
* THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

* \author Xander Soldaat (xander_at_botbench.com)
* \date 18 March 2012
* \version 0.1
* \example dexterind-compass-test1.c
* \example dexterind-compass-test2.c
* \example dexterind-compass-test3.c
*/

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define DIMCDAT "dimc%d.dat"

#define DIMC_I2C_ADDR           0x3C  /*!< Compass I2C address */

#define DIMC_REG_CONFIG_A       0x00  /*!< 250 dps range */
#define DIMC_REG_CONFIG_B       0x01  /*!< 500 dps range */
#define DIMC_REG_MODE           0x02  /*!< 2000 dps range */
#define DIMC_REG_X_MSB          0x03  /*!< Register MSB X axis */
#define DIMC_REG_X_LSB          0x04  /*!< Register LSB X axis */
#define DIMC_REG_Z_MSB          0x05  /*!< Register MSB Z axis */
#define DIMC_REG_Z_LSB          0x06  /*!< Register LSB Z axis */
#define DIMC_REG_Y_MSB          0x07  /*!< Register MSB Y axis */
#define DIMC_REG_Y_LSB          0x08  /*!< Register LSB Y axis */
#define DIMC_REG_STATUS         0x09  /*!< Status register */
#define DIMC_REG_IDENT_A        0x0A  /*!< Identification Register A */
#define DIMC_REG_IDENT_B        0x0B  /*!< Identification Register B */
#define DIMC_REG_IDENT_C        0x0C  /*!< Identification Register C */

// HMC5883L status definitions
// See page 16 of HMC5883L.pdf

#define DIMC_STATUS_LOCK  2           /*!< Data output register lock active */
#define DIMC_STATUS_RDY   1           /*!< Data is ready for reading */

// HMC5883L configuration definitions
// See pages 12, 13, 14 of HMC5883L.pdf

// The Mode Register has 2 possible values for Idle mode.
#define DIMC_MODE_CONTINUOUS  0       /*!< Continuous-Measurement Mode */
#define DIMC_MODE_SINGLE      1       /*!< Single-Measurement Mode (Default) */
#define DIMC_MODE_IDLE_A      2       /*!< Idle Mode */
#define DIMC_MODE_IDLE_B      3       /*!< Idle Mode */
#define DIMC_MODE_MASK        3       /*!< Mask for setting mode */

// How many samples averaged? Default=1
#define DIMC_CONF_A_SAMPLES_1    0x00 /*!< Number of samples averaged: 1 */
#define DIMC_CONF_A_SAMPLES_2    0x20 /*!< Number of samples averaged: 2 */
#define DIMC_CONF_A_SAMPLES_4    0x40 /*!< Number of samples averaged: 4 */
#define DIMC_CONF_A_SAMPLES_8    0x60 /*!< Number of samples averaged: 8 */
#define DIMC_CONF_A_SAMPLES_MASK 0x60 /*!< Mask for setting sample number */

// Data output rate for continuous mode. Default=15Hz
#define DIMC_CONF_A_RATE_0_75     0x00 /*!< Data Output Rate: 0.75 Hz */
#define DIMC_CONF_A_RATE_1_5      0x04 /*!< Data Output Rate: 1.5 Hz */
#define DIMC_CONF_A_RATE_3        0x08 /*!< Data Output Rate: 3 Hz */
#define DIMC_CONF_A_RATE_7_5      0x0C /*!< Data Output Rate: 7.5 Hz */
#define DIMC_CONF_A_RATE_15       0x10 /*!< Data Output Rate: 15 Hz */
#define DIMC_CONF_A_RATE_30       0x14 /*!< Data Output Rate: 30 Hz */
#define DIMC_CONF_A_RATE_75       0x18 /*!< Data Output Rate: 75 Hz */
#define DIMC_CONF_A_RATE_RESERVED 0x1C
#define DIMC_CONF_A_RATE_MASK     0x1C /*!< Mask for setting Data Output Rate */

// Measurement configuration, whether to apply bias. Default=Normal
#define DIMC_CONF_A_BIAS_NORMAL   0x00 /*!< Normal measurement configuration (Default) */
#define DIMC_CONF_A_BIAS_POSITIVE 0x01 /*!< Positive bias configuration for X, Y, and Z axes. */
#define DIMC_CONF_A_BIAS_NEGATIVE 0x02 /*!< Negative bias configuration for X, Y and Z axes. */
#define DIMC_CONF_A_BIAS_RESERVED 0x03
#define DIMC_CONF_A_BIAS_MASK     0x03 /*!< Mask for setting measurement bias */

// Gain configuration. Default=1.3Ga
#define DIMC_CONF_B_GAIN_0_88 0x00     /*!< Sensor Field Range ±0.88 Ga */
#define DIMC_CONF_B_GAIN_1_3  0x20     /*!< Sensor Field Range ±1.3 Ga */
#define DIMC_CONF_B_GAIN_1_9  0x40     /*!< Sensor Field Range ±1.9 Ga */
#define DIMC_CONF_B_GAIN_2_5  0x60     /*!< Sensor Field Range ±2.5 Ga */
#define DIMC_CONF_B_GAIN_4_0  0x80     /*!< Sensor Field Range ±4.0 Ga */
#define DIMC_CONF_B_GAIN_4_7  0xA0     /*!< Sensor Field Range ±4.7 Ga */
#define DIMC_CONF_B_GAIN_5_6  0xC0     /*!< Sensor Field Range ±5.6 Ga */
#define DIMC_CONF_B_GAIN_8_1  0xE0     /*!< Sensor Field Range ±8.1 Ga */
#define DIMC_CONF_B_GAIN_MASK 0xE0     /*!< Mask for setting Sensor Field Range */

// Digital resolution (mG/LSb) for each gain
#define DIMC_GAIN_SCALE_0_88  0.73     /*!< Ramge multiplier for ±0.88 Ga */
#define DIMC_GAIN_SCALE_1_3   0.92     /*!< Ramge multiplier for ±1.3 Ga */
#define DIMC_GAIN_SCALE_1_9   1.22     /*!< Ramge multiplier for ±1.9 Ga */
#define DIMC_GAIN_SCALE_2_5   1.52     /*!< Ramge multiplier for ±2.5 Ga */
#define DIMC_GAIN_SCALE_4_0   2.27     /*!< Ramge multiplier for ±4.0 Ga */
#define DIMC_GAIN_SCALE_4_7   2.56     /*!< Ramge multiplier for ±4.7 Ga */
#define DIMC_GAIN_SCALE_5_6   3.03     /*!< Ramge multiplier for ±5.6 Ga */
#define DIMC_GAIN_SCALE_8_1   4.35     /*!< Ramge multiplier for ±8.1 Ga */

// Sensors with calibration data require a <name>CalData struct
// to make use of the generic calibration data file writing/reading
typedef struct
{
  short _offsets[3];
} tDIMCCalData, *tDIMCCalDataptr;

typedef struct
{
  tI2CData I2CData;
  tDIMCCalData calData;
  float heading;
  short axes[3];
  short _minVals[3];
  short _maxVals[3];
  bool _calibrated;
  bool _calibrating;
  string _calibrationFile;
} tDIMC, *tDIMCptr;

bool initSensor(tDIMCptr dimcPtr, tSensors port);
bool readSensor(tDIMCptr dimcPtr);
bool startCal(tDIMCptr dimcPtr);
bool stopCal(tDIMCptr dimcPtr);
#ifdef NXT
bool _readCalVals(tDIMCptr dimcPtr);
bool _writeCalVals(tDIMCptr dimcPtr);
#endif // NXT

/**
 * Configure the Compass
 * @param dimcPtr pointer to tDIMC struct holding sensor info
 * @param port the port number
 * @return true if no error occured, false if it did
 */
bool initSensor(tDIMCptr dimcPtr, tSensors port)
{
  memset(dimcPtr, 0, sizeof(tDIMC));
  dimcPtr->I2CData.address = DIMC_I2C_ADDR;
  dimcPtr->I2CData.port = port;
#ifdef NXT
  dimcPtr->I2CData.type = sensorI2CCustomFastSkipStates;
#else
	dimcPtr->I2CData.type = sensorEV3_GenericI2C;
#endif

  // Set the file name for the calibration data
  sprintf(dimcPtr->_calibrationFile, "dimc%d.dat", dimcPtr->I2CData.port);

  // Ensure the sensor is configured correctly
  if (SensorType[dimcPtr->I2CData.port] != dimcPtr->I2CData.type)
    SensorType[dimcPtr->I2CData.port] = dimcPtr->I2CData.type;

  // Setup the size and address, same for all requests.
  dimcPtr->I2CData.request[0] = 3;    // Sending address, register, value. Optional, defaults to true
  dimcPtr->I2CData.request[1] = DIMC_I2C_ADDR; // I2C Address of Compass.

  // Not expecting a reply for the next commands
  dimcPtr->I2CData.replyLen = 0;

  // Write CONFIG_A
  // Set to 15Hz sample rate and a 8 sample average.
  dimcPtr->I2CData.request[2] = DIMC_REG_CONFIG_A;
  dimcPtr->I2CData.request[3] = DIMC_CONF_A_SAMPLES_8 + DIMC_CONF_A_RATE_15;
  if (!writeI2C(&dimcPtr->I2CData))
    return false;

  // Write CONFIG_B
  // Set gain to 1.9
  dimcPtr->I2CData.request[2] = DIMC_REG_CONFIG_B;
  dimcPtr->I2CData.request[3] = DIMC_CONF_B_GAIN_1_3;
  if (!writeI2C(&dimcPtr->I2CData))
    return false;

  // Write REG_MODE
  // Set to continuous mode
  ////////////////////////////////////////////////////////////////////////////
  dimcPtr->I2CData.request[2] = DIMC_REG_MODE;           // Register address of CTRL_REG3
  dimcPtr->I2CData.request[3] = DIMC_MODE_CONTINUOUS;    // No interrupts.  Date ready.
  if (!writeI2C(&dimcPtr->I2CData))
    return false;

#ifdef NXT
  return _readCalVals(dimcPtr);
#else
	return true;
#endif // NXT
}

/**
 * Read all three axes of the Compass and calculate the current heading
 * @param dimcPtr pointer to tDIMC struct holding sensor info
 * @return true if no error occured, false if it did
 */
bool readSensor(tDIMCptr dimcPtr)
{
  float angle;

  dimcPtr->I2CData.request[0] = 2;                   // Message size
  dimcPtr->I2CData.request[1] = DIMC_I2C_ADDR;  // I2C Address
  dimcPtr->I2CData.request[2] = DIMC_REG_X_MSB;            // Register address
  dimcPtr->I2CData.replyLen = 6;

  if (!writeI2C(&dimcPtr->I2CData)) {
    writeDebugStreamLine("error write");
    return false;
  }

  dimcPtr->axes[0] = (dimcPtr->I2CData.reply[0]<<8) + dimcPtr->I2CData.reply[1];
  dimcPtr->axes[1] = (dimcPtr->I2CData.reply[2]<<8) + dimcPtr->I2CData.reply[3];
  dimcPtr->axes[2] = (dimcPtr->I2CData.reply[4]<<8) + dimcPtr->I2CData.reply[5];

  if (dimcPtr->_calibrating)
  {
    for (short i = 0; i < 3; i++)
    {
      dimcPtr->_minVals[i] = min2(dimcPtr->axes[i], dimcPtr->_minVals[i]);
      dimcPtr->_maxVals[i] = max2(dimcPtr->axes[i], dimcPtr->_maxVals[i]);
    }
  }

  dimcPtr->axes[0] -= dimcPtr->calData._offsets[0];
  dimcPtr->axes[1] -= dimcPtr->calData._offsets[1];
  dimcPtr->axes[2] -= dimcPtr->calData._offsets[2];

  angle = atan2(dimcPtr->axes[0], dimcPtr->axes[1]);
  if (angle < 0) angle += 2*PI;
  dimcPtr->heading = ((angle * (180/PI)) + 270) % 360;

  return true;
}

/**
 * Start calibration.  The robot should be made to rotate
 * about its axis at least twice to get an accurate result.
 * Stop the calibration with stopCal()
 * @param dimcPtr pointer to tDIMC struct holding sensor info
 * @return true if no error occured, false if it did
 */
bool startCal(tDIMCptr dimcPtr)
{
  dimcPtr->_calibrating = true;
  return true;
}

/**
 * Stop calibration.  The appropriate offsets will be calculated for all
 * the axes.
 * @param dimcPtr pointer to tDIMC struct holding sensor info
 * @return true if no error occured, false if it did
 */
bool stopCal(tDIMCptr dimcPtr)
{
#ifdef EV3
#warning "Calibration values cannot currently be saved on the EV3"
	return true;
#else
  dimcPtr->_calibrating = false;
  for (short i = 0; i < 3; i++)
  {
    dimcPtr->calData._offsets[i] = ((dimcPtr->_maxVals[i] - dimcPtr->_minVals[i]) / 2) + dimcPtr->_minVals[i];
  }
  _writeCalVals(dimcPtr);
  return true;
#endif // EV3
}

/**
 * Write the calibration values to a data file.
 *
 * Note: this is an internal function and should not be called directly
 * @param dimcPtr pointer to tDIMC struct holding sensor info
 * @return true if no error occured, false if it did
 */
#ifdef NXT
bool _writeCalVals(tDIMCptr dimcPtr)
{
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize = 3*sizeof(short);

  // Delete the old data file and open a new one for writing
  Delete(DIMCDAT, nIoResult);
  OpenWrite(hFileHandle, nIoResult, dimcPtr->_calibrationFile, nFileSize);
  if (nIoResult != ioRsltSuccess)
  {
    Close(hFileHandle, nIoResult);
    eraseDisplay();
    displayTextLine(3, "W:can't cal file");
    playSound(soundException);
    while(bSoundActive) sleep(1);
    sleep(5000);
    stopAllTasks();
  }

  for (short i = 0; i < 3; i++)
  {
    WriteShort(hFileHandle, nIoResult, dimcPtr->calData._offsets[i]);
    if (nIoResult != ioRsltSuccess)
    {
      eraseDisplay();
      displayTextLine(3, "can't write offset");
      playSound(soundException);
      while(bSoundActive) sleep(1);
      sleep(5000);
      stopAllTasks();
    }
  }

  // Close the file
  Close(hFileHandle, nIoResult);
  if (nIoResult != ioRsltSuccess)
  {
    eraseDisplay();
    displayTextLine(3, "Can't close");
    playSound(soundException);
    while(bSoundActive) sleep(1);
    sleep(5000);
    stopAllTasks();
  }
  return true;
}
#endif // NXT

/**
 * Read the calibration values from a data file.
 *
 * Note: this is an internal function and should not be called directly
 * @param dimcPtr pointer to tDIMC struct holding sensor info
 * @return true if no error occured, false if it did
 */
#ifdef NXT
bool _readCalVals(tDIMCptr dimcPtr)
{
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize;

  // Open the data file for reading
  OpenRead(hFileHandle, nIoResult, dimcPtr->_calibrationFile, nFileSize);
  if (nIoResult != ioRsltSuccess)
  {
    Close(hFileHandle, nIoResult);

    // Assign default values
    memset(dimcPtr->calData._offsets, 0, 3 * sizeof(short));
    _writeCalVals(dimcPtr);
    return true;
  }

  for (short i = 0; i < 3; i++)
  {
    ReadShort(hFileHandle, nIoResult, (short)dimcPtr->calData._offsets[i]);
    // writeDebugStream("R offsets[%d][%d]:", i, j);
    // writeDebugStreamLine(" %d", DIMCoffsets[i][j]);
    if (nIoResult != ioRsltSuccess)
    {
      memset(dimcPtr->calData._offsets, 0, 3 * sizeof(short));
      _writeCalVals(dimcPtr);
      return true;
    }
  }

  Close(hFileHandle, nIoResult);
  dimcPtr->_calibrated = true;
  return true;
}
#endif // NXT
#endif // __DIMC_H__

/* @} */
/* @} */
