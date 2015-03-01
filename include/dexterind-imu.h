/*!@addtogroup Dexter_Industries
 * @{
 * @defgroup DIMU IMU Sensor
 * Dexter Industries DIMU Sensor driver
 * @{
 */

#ifndef __DIMU_H__
#define __DIMU_H__
/** \file dexterind-imu.h
 * \brief Dexter Industries IMU Sensor driver
 *
 * dexterind-imu.h provides an API for the Dexter Industries IMU Sensor.\n
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
 * \date 07 August 2011
 * \version 0.1
 * \example dexterind-imu-test1.c
 * \example dexterind-imu-test2.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

//#define STRUCT_CODE_ENABLED 1

#ifdef STRUCT_CODE_ENABLED
#warn "You have enabled experimental code!"
#endif

#define DIMU_GYRO_I2C_ADDR      0xD2  /*!< Gyro I2C address */

#define DIMU_GYRO_RANGE_250     0x00  /*!< 250 dps range */
#define DIMU_GYRO_RANGE_500     0x10  /*!< 500 dps range */
#define DIMU_GYRO_RANGE_2000    0x20  /*!< 2000 dps range */
#define DIMU_CTRL4_BLOCKDATA    0x80

#define DIMU_GYRO_CTRL_REG1     0x20  /*!< CTRL_REG1 for Gyro */
#define DIMU_GYRO_CTRL_REG2     0x21  /*!< CTRL_REG2 for Gyro */
#define DIMU_GYRO_CTRL_REG3     0x22  /*!< CTRL_REG3 for Gyro */
#define DIMU_GYRO_CTRL_REG4     0x23  /*!< CTRL_REG4 for Gyro */
#define DIMU_GYRO_CTRL_REG5     0x24  /*!< CTRL_REG5 for Gyro */

#define DIMU_GYRO_ALL_AXES      0x28  /*!< All Axes for Gyro */
#define DIMU_GYRO_X_AXIS        0x2A  /*!< X Axis for Gyro */
#define DIMU_GYRO_Y_AXIS        0x28  /*!< Y Axis for Gyro */
#define DIMU_GYRO_Z_AXIS        0x2C  /*!< Z Axis for Gyro */

#define DIMU_ACC_I2C_ADDR       0x3A  /*!< Accelerometer I2C address */
#define DIMU_ACC_RANGE_2G       0x04  /*!< Accelerometer 2G range */
#define DIMU_ACC_RANGE_4G       0x08  /*!< Accelerometer 4G range */
#define DIMU_ACC_RANGE_8G       0x00  /*!< Accelerometer 8G range */
#define DIMU_ACC_MODE_STBY      0x00  /*!< Accelerometer standby mode */
#define DIMU_ACC_MODE_MEAS      0x01  /*!< Accelerometer measurement mode */
#define DIMU_ACC_MODE_LVLD      0x02  /*!< Accelerometer level detect mode */
#define DIMU_ACC_MODE_PLSE      0x03  /*!< Accelerometer pulse detect mode */
#define DIMU_ACC_X_AXIS         0x00  /*!< X Axis for Accel */
#define DIMU_ACC_Y_AXIS         0x02  /*!< Y Axis for Accel */
#define DIMU_ACC_Z_AXIS         0x04  /*!< Z Axis for Accel */

#define DIMUreadGyroXAxis(X) DIMUreadGyroAxis(X, DIMU_GYRO_X_AXIS)
#define DIMUreadGyroYAxis(X) DIMUreadGyroAxis(X, DIMU_GYRO_Y_AXIS)
#define DIMUreadGyroZAxis(X) DIMUreadGyroAxis(X, DIMU_GYRO_Z_AXIS)

#define DIMUreadAccelXAxis10Bit(X) DIMUreadAccelAxis10Bit(X, DIMU_ACC_X_AXIS)
#define DIMUreadAccelYAxis10Bit(X) DIMUreadAccelAxis10Bit(X, DIMU_ACC_Y_AXIS)
#define DIMUreadAccelZAxis10Bit(X) DIMUreadAccelAxis10Bit(X, DIMU_ACC_Z_AXIS)

#define DIMUreadAccelXAxis8Bit(X) DIMUreadAccelAxis8Bit(X, DIMU_ACC_X_AXIS)
#define DIMUreadAccelYAxis8Bit(X) DIMUreadAccelAxis8Bit(X, DIMU_ACC_Y_AXIS)
#define DIMUreadAccelZAxis8Bit(X) DIMUreadAccelAxis8Bit(X, DIMU_ACC_Z_AXIS)

// for future use
#ifdef STRUCT_CODE_ENABLED
typedef struct
{
  tI2CData I2CData;
  float gyroDivisor;
  float accelDivisor;
  float axesAccel8Bit[3];
  float axesAccel10Bit[3];
  float axesGyro8Bit[3];
  float axesGyro10Bit[3];
} tDIMU, *tDIMUptr;
#endif

float DIMU_Gyro_divisor[4] = {0.0, 0.0, 0.0, 0.0}; /*!< Array to hold divisor data for 8 bit measurements */
float DIMU_Accel_divisor[4] = {0.0, 0.0, 0.0, 0.0}; /*!< Array to hold divisor data for 8 bit measurements */
float DIMU_Gyro_offset[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

tByteArray DIMU_I2CRequest;    /*!< Array to hold I2C command data */
tByteArray DIMU_I2CReply;      /*!< Array to hold I2C reply data */

bool DIMUconfigGyro(tSensors link, ubyte range, bool lpfenable=true);
float DIMUreadGyroAxis(tSensors link, ubyte axis);
void DIMUreadGyroAxes(tSensors link, float &_x, float &_y, float &_z);
void _DIMUcalcOffset(tSensors link);
bool DIMUconfigAccel(tSensors link, ubyte range);
float DIMUreadAccelAxis8Bit(tSensors link, ubyte axis);
bool DIMUsetAccelAxisOffset(tSensors link, ubyte drift_reg, ubyte drift_LSB, ubyte drift_MSB);
float DIMUreadAccelAxis10Bit(tSensors link, ubyte axis, bool calibrate = false);
void DIMUreadAccelAxes8Bit(tSensors link, float &_x, float &_y, float &_z);
void DIMUreadAccelAxes10Bit(tSensors link, float &_x, float &_y, float &_z);
void DIMUcalAccel(tSensors link);
bool DIMUconfigIMU(tSensors link, ubyte accelRange=DIMU_ACC_RANGE_8G, ubyte gyroRange=DIMU_GYRO_RANGE_250, bool lpfenable=true);

#ifdef STRUCT_CODE_ENABLED
bool initSensor(tDIMUptr sensor, tSensors link);
bool configGyro(tDIMUptr sensor, ubyte range, bool lpfenable=true);
void DIMUreadGyroAxes(tDIMUptr sensor);
bool DIMUconfigAccel(tDIMUptr sensor, ubyte range);
void DIMUreadAccelAxes8Bit(tDIMUptr sensor);
void DIMUreadAccelAxes10Bit(tDIMUptr sensor);
void DIMUcalAccel(tDIMUptr sensor);
bool DIMUconfigIMU(tDIMUptr sensor, ubyte accelRange=DIMU_ACC_RANGE_8G, ubyte gyroRange=DIMU_GYRO_RANGE_250, bool lpfenable=true);
#endif

/**
 * Configure the gyro
 * @param link the port number
 * @param range the operating range of the gyro
 * @param lpfenable Enable built-in Low Pass Filter to reduce spikes in data, optional, defaults to true.
 * @return true if no error occured, false if it did
 */
bool DIMUconfigGyro(tSensors link, ubyte range, bool lpfenable){
  memset(DIMU_I2CRequest, 0, sizeof(DIMU_I2CRequest));

  // Setup the size and address, same for all requests.
  DIMU_I2CRequest[0] = 3;    // Sending address, register, value. Optional, defaults to true
  DIMU_I2CRequest[1] = 0xD2; // I2C Address of gyro.

  // Write CTRL_REG2
  // No High Pass Filter
  DIMU_I2CRequest[2] = DIMU_GYRO_CTRL_REG2;
  DIMU_I2CRequest[3] = 0x00;
  if (!writeI2C(link, DIMU_I2CRequest))
    return false;

  // Write CTRL_REG3
  // No interrupts.  Date ready.
  ////////////////////////////////////////////////////////////////////////////
  DIMU_I2CRequest[2] = DIMU_GYRO_CTRL_REG3;      // Register address of CTRL_REG3
  DIMU_I2CRequest[3] = 0x08;      // No interrupts.  Date ready.
  if(!writeI2C(link, DIMU_I2CRequest))
    return false;

  // Write CTRL_REG4
  // Full scale range.
  DIMU_I2CRequest[2] = DIMU_GYRO_CTRL_REG4;
  DIMU_I2CRequest[3] = range + DIMU_CTRL4_BLOCKDATA;
  writeI2C(link, DIMU_I2CRequest);

  //Write CTRL_REG5
  DIMU_I2CRequest[2] = DIMU_GYRO_CTRL_REG5;      // Register address of CTRL_REG5
  DIMU_I2CRequest[3] = (lpfenable) ? 0x02 : 0x00;      // filtering - low pass
  if (!writeI2C(link, DIMU_I2CRequest))
    return false;

  // Write CTRL_REG1
  // Enable all axes. Disable power down.
  DIMU_I2CRequest[2] = DIMU_GYRO_CTRL_REG1;
  DIMU_I2CRequest[3] = 0x0F;
  if (!writeI2C(link, DIMU_I2CRequest))
    return false;

  // Set DIMU_Gyro_divisor so that the output of our gyro axis readings can be turned
  // into scaled values.
  ///////////////////////////////////////////////////////////////////////////
  if(range == 0)
    DIMU_Gyro_divisor[link] = 8.75/1000;  // Full scale range is 250 dps.
  else if (range == 0x10)
    DIMU_Gyro_divisor[link] = 17.5/1000;	// Full scale range is 500 dps.
  else if (range == 0x20)
    DIMU_Gyro_divisor[link] = 70/1000;    // Full scale range is 2000 dps.

	_DIMUcalcOffset(link);

  return true;
}

/**
 * Retrieve the axis data
 * @param link the port number
 * @param axis the specified axis
 * @return the axis data in degrees per second
 */
float DIMUreadGyroAxis(tSensors link, ubyte axis){
  // ubyte _msb = 0;
  // ubyte _lsb = 0;

  DIMU_I2CRequest[0] = 2;                   // Message size
  DIMU_I2CRequest[1] = DIMU_GYRO_I2C_ADDR;  // I2C Address
  DIMU_I2CRequest[2] = axis + 0x80;            // Register address

  if (!writeI2C(link, DIMU_I2CRequest, DIMU_I2CReply, 2)) {
    writeDebugStreamLine("error write");
    return 0;
  }

  return (DIMU_I2CReply[0]+((long)(DIMU_I2CReply[1]<<8)))/DIMU_Gyro_divisor[link];
}

/**
 * Read all three axes of the gyro
 * @param link the port number
 * @param _x data for x axis in degrees per second
 * @param _y data for y axis in degrees per second
 * @param _z data for z axis in degrees per second
 * @return true if no error occured, false if it did
 */
void DIMUreadGyroAxes(tSensors link, float &_x, float &_y, float &_z){
  DIMU_I2CRequest[0] = 2;                   // Message size
  DIMU_I2CRequest[1] = DIMU_GYRO_I2C_ADDR;  // I2C Address
  DIMU_I2CRequest[2] = DIMU_GYRO_ALL_AXES + 0x80;            // Register address

  if (!writeI2C(link, DIMU_I2CRequest, DIMU_I2CReply, 6)) {
    writeDebugStreamLine("error write");
    return;
  }

  _y = (DIMU_I2CReply[0]+((word)(DIMU_I2CReply[1]<<8)))*DIMU_Gyro_divisor[link];
  _x = (DIMU_I2CReply[2]+((word)(DIMU_I2CReply[3]<<8)))*DIMU_Gyro_divisor[link];
  _z = (DIMU_I2CReply[4]+((word)(DIMU_I2CReply[5]<<8)))*DIMU_Gyro_divisor[link];

  _x -= DIMU_Gyro_offset[(link*3)+0];
  _y -= DIMU_Gyro_offset[(link*3)+1];
  _z -= DIMU_Gyro_offset[(link*3)+2];

}


// Internal function.
void _DIMUcalcOffset(tSensors link)
{
	const int numSamples = 10;

	float totalX, totalY, totalZ = 0;
	float x, y, z;

	for (int i = 0; i < numSamples; i++)
	{
		DIMUreadGyroAxes(link, x, y, z);
		totalX += x;
		totalY += y;
		totalZ += z;
		sleep(50);
	}

	totalX /= numSamples;
	totalY /= numSamples;
	totalZ /= numSamples;

	DIMU_Gyro_offset[(link*3)+0] = x;
	DIMU_Gyro_offset[(link*3)+1] = y;
	DIMU_Gyro_offset[(link*3)+2] = z;
}


/**
 * Wait for the I2C bus to be ready for the next message
 * @param link the port number
 * @param range the range at which to operate the Accelerometer, can be 2, 4 and 8G
 * @return true if no error occured, false if it did
 */
bool DIMUconfigAccel(tSensors link, ubyte range) {
  switch(range) {
    case DIMU_ACC_RANGE_2G: DIMU_Accel_divisor[link] = 64.0; break;
    case DIMU_ACC_RANGE_4G: DIMU_Accel_divisor[link] = 32.0; break;
    case DIMU_ACC_RANGE_8G: DIMU_Accel_divisor[link] = 16.0; break;
  }

  DIMU_I2CRequest[0] = 3;                 // Sending address, register, value.
  DIMU_I2CRequest[1] = DIMU_ACC_I2C_ADDR; // I2C Address of Accelerometer.

  //Set the Mode Control - P.25 of Documentation
  ////////////////////////////////////////////////////////////////////////////
  DIMU_I2CRequest[2] = 0x16;                   // Register address of Mode Control
  DIMU_I2CRequest[3] = range | DIMU_ACC_MODE_MEAS;
  if (!writeI2C(link, DIMU_I2CRequest))     // (Port 1, Message Array, Reply Size)
    return false;

  DIMUcalAccel(link);
  return true;
}

/**
 * Read the specified accelerometer axis, returns an 8 bit answer
 * @param link the port number
 * @param axis the specific axis
 * @return gravity in G with 8 bit accuracy
 */
float DIMUreadAccelAxis8Bit(tSensors link, ubyte axis){
  short sensorReading = 0;
  DIMU_I2CRequest[0] = 2;      // Sending address, register.
  DIMU_I2CRequest[1] = DIMU_ACC_I2C_ADDR;   // I2C Address of accl.

  switch (axis) {
    case DIMU_ACC_X_AXIS: DIMU_I2CRequest[2] = 0x06; break;
    case DIMU_ACC_Y_AXIS: DIMU_I2CRequest[2] = 0x07; break;
    case DIMU_ACC_Z_AXIS: DIMU_I2CRequest[2] = 0x08; break;
  }

  if (!writeI2C(link, DIMU_I2CRequest, DIMU_I2CReply, 1))
    return 0;

  sensorReading = (short)DIMU_I2CReply[0];
  return ((sensorReading > 128) ? sensorReading - 256 : sensorReading) / DIMU_Accel_divisor[link];
}

/**
 * Confgures an offset register for the accelerometer.
 * @param link the port number
 * @param drift_reg the register to write to
 * @param drift_LSB drift value LSB
 * @param drift_MSB drift value MSB
 * @return true if no error occured, false if it did
 */
bool DIMUsetAccelAxisOffset(tSensors link, ubyte drift_reg, ubyte drift_LSB, ubyte drift_MSB){
  DIMU_I2CRequest[0] = 3;                     // Sending address, register, value.
  DIMU_I2CRequest[1] = DIMU_ACC_I2C_ADDR;     // I2C Address of accl.

  DIMU_I2CRequest[2] = drift_reg;             // Register of the data we're writing to.
  DIMU_I2CRequest[3] = drift_LSB;             // The drift value to write for calibration.
  if (!writeI2C(link, DIMU_I2CRequest))
    return false;

  DIMU_I2CRequest[2] = drift_reg + 1;
  DIMU_I2CRequest[3] = drift_MSB;
  return writeI2C(link, DIMU_I2CRequest);
}

/**
 * Read the specified accelerometer axis, returns an 10 bit answer
 * @param link the port number
 * @param axis the specific axis
 * @param calibrate optional argument, if set to to true, the sensor will calibrate this axis first.
 * @return gravity in G with 10 bit accuracy
 */
float DIMUreadAccelAxis10Bit(tSensors link, ubyte axis, bool calibrate){
  short ureading = 0;  // unsigned sensor data
  short sreading = 0;  // signed sensor data
  short drift_offset = 0;

  if (calibrate == true) {
    DIMUsetAccelAxisOffset(link, 0x10 + axis, 0x00, 0x00);
    sleep(50);
  }

  DIMU_I2CRequest[0] = 2;          // Sending address, register.
  DIMU_I2CRequest[1] = DIMU_ACC_I2C_ADDR;  // I2C Address of accl.
  DIMU_I2CRequest[2] = axis;        // First Register of the data we're requesting.

  if (!writeI2C(link, DIMU_I2CRequest, DIMU_I2CReply, 2))
    return 0;

  ureading = DIMU_I2CReply[0] + (DIMU_I2CReply[1] << 8) & 0x3FF;
  sreading = (ureading > 511) ? ureading - 1024 : ureading;
  //sreading = (ureading & 0x200) ? -(((~ureading) & 0x3FF)+1) : ureading;

  if (calibrate == true) {
    sleep(50);
    switch (axis) {
      case DIMU_ACC_X_AXIS: drift_offset = (  0 - sreading ) * 2; break;
      case DIMU_ACC_Y_AXIS: drift_offset = (  0 - sreading ) * 2; break;
      case DIMU_ACC_Z_AXIS: drift_offset = ( 64 - sreading ) * 2; break;
    }
    DIMUsetAccelAxisOffset(link, 0x10 + axis, drift_offset & 0x00ff, (drift_offset & 0xff00 ) >> 8);
  }

  return sreading / 64.0;
}

/**
 * Read the specified accelerometer axis, returns an 8 bit answer
 * @param link the port number
 * @param _x variable to hold X axis data
 * @param _y variable to hold Y axis data
 * @param _z variable to hold Z axis data
 */
void DIMUreadAccelAxes8Bit(tSensors link, float &_x, float &_y, float &_z){
  _x = DIMUreadAccelAxis8Bit(link, DIMU_ACC_X_AXIS);
  _y = DIMUreadAccelAxis8Bit(link, DIMU_ACC_Y_AXIS);
  _z = DIMUreadAccelAxis8Bit(link, DIMU_ACC_Z_AXIS);
}

/**
 * Read the specified accelerometer axis, returns an 10 bit answer
 * @param link the port number
 * @param _x variable to hold X axis data
 * @param _y variable to hold Y axis data
 * @param _z variable to hold Z axis data
 */
void DIMUreadAccelAxes10Bit(tSensors link, float &_x, float &_y, float &_z){
  _x = DIMUreadAccelAxis10Bit(link, DIMU_ACC_X_AXIS);
  _y = DIMUreadAccelAxis10Bit(link, DIMU_ACC_Y_AXIS);
  _z = DIMUreadAccelAxis10Bit(link, DIMU_ACC_Z_AXIS);
}

/**
 * Calibrate the Accelerometer.  The sensor must be stationary and assumes the Z axis is facing up.
 * @param link the port number
 */
void DIMUcalAccel(tSensors link){
  DIMUreadAccelAxis10Bit(link, DIMU_ACC_X_AXIS, true);      // Get x axis data.
  DIMUreadAccelAxis10Bit(link, DIMU_ACC_Y_AXIS, true);      // Get y axis data.
  DIMUreadAccelAxis10Bit(link, DIMU_ACC_Z_AXIS, true);      // Get z axis data.
  sleep(100);
}

bool DIMUconfigIMU(tSensors link, ubyte accelRange, ubyte gyroRange, bool lpfenable)
{
  if (!DIMUconfigGyro(link, gyroRange, lpfenable))
    return false;

  return DIMUconfigAccel(link, accelRange);
}

#ifdef STRUCT_CODE_ENABLED
bool DIMUconfigGyro(tDIMUptr sensor, ubyte range, bool lpfenable)
{
  // Setup the size and address, same for all requests.
  sensor->I2CData.request[0] = 3;    // Sending address, register, value. Optional, defaults to true
  sensor->I2CData.request[1] = 0xD2; // I2C Address of gyro.

  // Write CTRL_REG2
  // No High Pass Filter
  sensor->I2CData.request[2] = DIMU_GYRO_CTRL_REG2;
  sensor->I2CData.request[3] = 0x00;
  if (!writeI2C(&sensor->I2CData))
    return false;

  // Write CTRL_REG3
  // No interrupts.  Date ready.
  ////////////////////////////////////////////////////////////////////////////
  sensor->I2CData.request[2] = DIMU_GYRO_CTRL_REG3;      // Register address of CTRL_REG3
  sensor->I2CData.request[3] = 0x08;      // No interrupts.  Date ready.
  if (!writeI2C(&sensor->I2CData))
    return false;

  // Write CTRL_REG4
  // Full scale range.
  sensor->I2CData.request = DIMU_GYRO_CTRL_REG4;
  sensor->I2CData.request[3] = range + DIMU_CTRL4_BLOCKDATA;
  if (!writeI2C(&sensor->I2CData))

  //Write CTRL_REG5
  sensor->I2CData.request[2] = DIMU_GYRO_CTRL_REG5;      // Register address of CTRL_REG5
  sensor->I2CData.request[3] = (lpfenable) ? 0x02 : 0x00;      // filtering - low pass
  if (!writeI2C(&sensor->I2CData))
    return false;

  // Write CTRL_REG1
  // Enable all axes. Disable power down.
  sensor->I2CData.request[2] = DIMU_GYRO_CTRL_REG1;
  sensor->I2CData.request[3] = 0x0F;
  if (!writeI2C(&sensor->I2CData))
    return false;

  // Set DIMU_Gyro_divisor so that the output of our gyro axis readings can be turned
  // into scaled values.
  ///////////////////////////////////////////////////////////////////////////
  if(range == 0)
    DIMU_Gyro_divisor[link] = 8.75/1000;      // Full scale range is 250 dps.
  else if (range == 0x10)
    DIMU_Gyro_divisor[link] = 17.5/1000;       // Full scale range is 500 dps.
  else if (range == 0x30)
    DIMU_Gyro_divisor[link] = 70/1000;       // Full scale range is 2000 dps.

  return true;
}

void DIMUreadGyroAxes(tDIMUptr sensor)
{

}
bool DIMUconfigAccel(tDIMUptr sensor, ubyte range)
{

}
void DIMUreadAccelAxes8Bit(tDIMUptr sensor)
{

}
void DIMUreadAccelAxes10Bit(tDIMUptr sensor)
{

}
void DIMUcalAccel(tDIMUptr sensor)
{

}
bool DIMUconfigIMU(tDIMUptr sensor, ubyte accelRange, ubyte gyroRange, bool lpfenable)
{

}
#endif

#endif // __DIMU_H__

/* @} */
/* @} */
