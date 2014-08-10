/*!@addtogroup Dexter_Industries
 * @{
 * @defgroup dFlex dFlex Sensor
 * Dexter Industries dFlex Sensor driver
 * @{
 */

#ifndef __DFLEX_H__
#define __DFLEX_H__
/** \file dexterind-flex.h
 * \brief ROBOTC Dexter Industries dFlex Sensor driver
 *
 * DFLEX-driver.h provides an API for the Dexter Industries dFlex Sensor.
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
 * \date 23 June 2010
 * \version 0.1
 * \example dexterind-flex-test1.c
 * \example dexterind-flex-test2.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define DFLEXDAT "DFLEX.dat"    /*!< Datafile for dFlex Sensor calibration info */

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
bool _readCalVals(tDIMCptr dimcPtr);
bool _writeCalVals(tDIMCptr dimcPtr);

// Globals
short dflexlow = 0;                    /*!< Low calibration value */
short dflexhigh = 1023;                /*!< High calibration value */
bool DFLEX_calibrated = false;   /*!< Has the sensor been calibrated yet */

// Function prototypes
short DFLEXvalRaw(tSensors link);
short DFLEXvalNorm(tSensors link);

void DFLEXcalLow(tSensors link);
void DFLEXcalLow(short lowval);
void DFLEXcalHigh(tSensors link);
void DFLEXcalHigh(short highval);

void _DFLEXcheckSensor(tSensors link);
void _DFLEXwriteCalVals(short lowval, short highval);
void _DFLEXreadCalVals(short &lowval, short &highval);

/**
 * Read the raw value of the dFlex Sensor.
 * @param link the dFlex Sensor port number
 * @return the raw value of the dFlex Sensor
 */
short DFLEXvalRaw(tSensors link) {
  _DFLEXcheckSensor(link);

  return SensorRaw[link];
}

/**
 * Read the normalised value of the dFlex Sensor, based on the low and high values.
 *
 * Note: this is not a linear value
 * @param link the dFlex Sensor port number
 * @return the normalised value (0-100)
 */
short DFLEXvalNorm(tSensors link) {
  long currval = 0;

  _DFLEXcheckSensor(link);

  if (!DFLEX_calibrated) {
    _DFLEXreadCalVals(dflexlow, dflexhigh);
  }

  currval = DFLEXvalRaw(link);

  if (currval <= dflexlow)
    return 0;
  else if (currval >= dflexhigh)
    return 100;

  return ((currval - dflexlow) * 100) / (dflexhigh - dflexlow);
}

/**
 * Calibrate the dFlex Sensor's low calibration value with the current raw sensor reading.
 * @param link the dFlex Sensor port number
 */
void DFLEXcalLow(tSensors link) {
  _DFLEXcheckSensor(link);

  dflexlow = SensorRaw[link];
  _DFLEXwriteCalVals(dflexlow, dflexhigh);
}

/**
 * Calibrate the dFlex Sensor's low calibration value with the supplied value.
 * @param lowval the sensor's low calibration value
 */
void DFLEXcalLow(short lowval) {
  dflexlow = lowval;
  _DFLEXwriteCalVals(dflexlow, dflexhigh);
}

/**
 * Calibrate the dFlex Sensor's high calibration value with the current raw sensor reading.
 * @param link the dFlex Sensor port number
 */
void DFLEXcalHigh(tSensors link) {
  _DFLEXcheckSensor(link);

  dflexhigh = SensorRaw[link];
  _DFLEXwriteCalVals(dflexlow, dflexhigh);
}

/**
 * Calibrate the dFlex Sensor's high calibration value with the supplied value.
 * @param highval the sensor's high calibration value
 */
void DFLEXcalHigh(short highval) {
  dflexhigh = highval;
  _DFLEXwriteCalVals(dflexlow, dflexhigh);
}

/**
 * Check if the sensor is set to raw and that it's been configured as a
 * sensorAnalogInactive. If not, reconfigure the port.
 *
 * Note: this is an internal function and should not be called directly
 * @param link the dFlex Sensor port number
 */
void _DFLEXcheckSensor(tSensors link) {
  if (SensorMode[link] != modeRaw)
    SensorMode[link] = modeRaw;
  if (SensorType[link] != sensorAnalogInactive)
    SensorType[link] = sensorAnalogInactive;
}

/**
 * Write the low and high calibration values to a data file.
 *
 * Note: this is an internal function and should not be called directly
 * @param lowval the low calibration value
 * @param highval the high calibration value
 */
void _DFLEXwriteCalVals(short lowval, short highval) {
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize = 4;

  // Delete the old data file and open a new one for writing
  Delete(DFLEXDAT, nIoResult);
  OpenWrite(hFileHandle, nIoResult, DFLEXDAT, nFileSize);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    eraseDisplay();
    displayTextLine(3, "W:can't cal file");
    playSound(soundException);
    while(bSoundActive) sleep(1);
    sleep(5000);
    stopAllTasks();
  }

  // Write the low calibration value
  WriteShort(hFileHandle, nIoResult, lowval);
  if (nIoResult != ioRsltSuccess) {
    eraseDisplay();
    displayTextLine(3, "can't write lowval");
    playSound(soundException);
    while(bSoundActive) sleep(1);
    sleep(5000);
    stopAllTasks();
  }

  // Write the high calibration value
  WriteShort(hFileHandle, nIoResult, highval);
  if (nIoResult != ioRsltSuccess) {
    eraseDisplay();
    displayTextLine(3, "can't write highval");
    playSound(soundException);
    while(bSoundActive) sleep(1);
    sleep(5000);
    stopAllTasks();
  }

  // Close the file
  Close(hFileHandle, nIoResult);
  if (nIoResult != ioRsltSuccess) {
    eraseDisplay();
    displayTextLine(3, "Can't close");
    playSound(soundException);
    while(bSoundActive) sleep(1);
    sleep(5000);
    stopAllTasks();
  }
}

/**
 * Read the low and high calibration values from a data file.
 *
 * Note: this is an internal function and should not be called directly
 * @param lowval the low calibration value
 * @param highval the high calibration value
 */
void _DFLEXreadCalVals(short &lowval, short &highval) {
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize;

  short lv = 0;
  short hv = 0;

  // Open the data file for reading
  DFLEX_calibrated = true;
  OpenRead(hFileHandle, nIoResult, DFLEXDAT, nFileSize);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    return;
  }

  // Read the low calibration value
  ReadShort(hFileHandle, nIoResult, lv);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    return;
  }

  // Read the high calibration value
  ReadShort(hFileHandle, nIoResult, hv);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    return;
  }

  // Assign values and close file
  lowval = lv;
  highval = hv;
  Close(hFileHandle, nIoResult);
}

#endif // __DFLEX_H__

/* @} */
/* @} */
