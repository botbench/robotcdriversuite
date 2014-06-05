#pragma config(Sensor, S1,     MSIMU,                sensorI2CCustomFastSkipStates)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: mindsensors-imu-test3.c $
 */

/**
 * mindsensors-imu.h provides an API for the Mindsensors AbsoluteIMU Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 26 August 2012
 * version 0.1
 */

#include "mindsensors-imu.h"

task main(){

  short heading;
  short x_val, y_val, z_val;      // axis values

  displayCenteredTextLine(0, "Mindsensors");
  displayCenteredBigTextLine(1, "IMU");
  displayCenteredTextLine(3, "Test 3");
  displayCenteredTextLine(5, "Connect sensor");
  displayCenteredTextLine(6, "to S1");
  sleep(2000);
  eraseDisplay();

  while (true){

		// Read the Compass
    heading = MSIMUreadHeading(MSIMU);

		displayTextLine(1, "Heading: %d", heading);

		// Read the magnetic fields (they're used to calculate the heading)
		MSIMUreadMagneticFields(MSIMU, x_val, y_val, z_val);
    displayTextLine(3, "Magnetic fields:");
		displayTextLine(4, "X: %d", x_val);
		displayTextLine(5, "Y: %d", y_val);
		displayTextLine(6, "Z: %d", z_val);
		sleep(50);
  }
}

/*
 * $Id: mindsensors-imu-test3.c $
 */
