/**
 * HTCS-driver.h provides an API for the HiTechnic Force Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 30 September 2012
 * version 0.1
 */

#include "hitechnic-force.h"

task main() {
  eraseDisplay();

  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "Force");
  displayCenteredTextLine(3, "Test 1");
  sleep(2000);
  eraseDisplay();

  // Create struct to hold sensor data
  tHTF forceSensor;

  // Initialise and configure struct and port
  initSensor(&forceSensor, S1);

  while(true) {
    // Read the sensor value
    readSensor(&forceSensor);

    displayClearTextLine(3);
    displayClearTextLine(4);
    displayTextLine(3, "Force:  %4d", forceSensor.force);
    sleep(50);
  }
}
