

/**
 * hitechnic-colour-v2.h provides an API for the HiTechnic Color V2 Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments
 * - 0.3: Removed common.h from includes
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 20 February 2011
 * version 0.3
 */

#include "hitechnic-colour-v2.h"

task main ()
{
  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "Color V2");
  displayCenteredTextLine(3, "Test 1");
  displayCenteredTextLine(5, "Connect sensor");
  displayCenteredTextLine(6, "to S1");
  sleep(2000);

  // Create struct to hold sensor data
  tHTCS2 colorSensor;

  // Initialise and configure struct and port
  initSensor(&colorSensor, S1);

  eraseDisplay();
  while (true)
  {
    // Read the currently detected colour and RGB/HSV data from the sensor
    if (!readSensor(&colorSensor)) {
      displayTextLine(4, "ERROR!!");
      sleep(2000);
      stopAllTasks();
    }

    displayCenteredTextLine(0, "Color: %d", colorSensor.color);
    displayCenteredBigTextLine(1, "R  G  B");

    eraseRect(0,10, 99, 41);
    fillRect( 0, 10, 30, 10 + (colorSensor.red+1)/8);
    fillRect(35, 10, 65, 10 + (colorSensor.green+1)/8);
    fillRect(70, 10, 99, 10 + (colorSensor.blue+1)/8);
    displayTextLine(7, " %3d   %3d   %3d", colorSensor.red, colorSensor.green, colorSensor.blue);
		writeDebugStreamLine("R: %d, G: %d, B: %d", colorSensor.red, colorSensor.green, colorSensor.blue);
    sleep(100);
  }
}
