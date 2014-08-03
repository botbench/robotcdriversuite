

/**
 * hitechnic-colour-v2.h provides an API for the HiTechnic Color V2 Sensor.  This program
 * demonstrates how to use that API.

 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments
 * - 0.3: Now uses HTSMUX driver which has been split from common.h\n
 *        HTSMUX initialisation and scanning functions have been removed\n
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

task main () {
  string _tmp;

  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "COLOR V2");
  displayCenteredTextLine(3, "SMUX Test");
  displayCenteredTextLine(5, "Connect SMUX to");
  displayCenteredTextLine(6, "S1 and CS to");
  displayCenteredTextLine(7, "SMUX Port 1");
  sleep(2000);

  // Create struct to hold sensor data
  tHTCS2 colorSensor;

  // Initialise and configure struct and port
  // The sensor is connected to the first port
  // of the SMUX which is connected to the NXT port S1.
  // To access that sensor, we must use msensor_S1_1.  If the sensor
  // were connected to 3rd port of the SMUX connected to the NXT port S4,
  // we would use msensor_S4_3
  initSensor(&colorSensor, msensor_S1_1);

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
    stringFormat(_tmp, " %3d   %3d", colorSensor.red, colorSensor.green);
    displayTextLine(7, "%s   %3d", _tmp, colorSensor.blue);

    sleep(100);
  }
}
