

/**
 * hitechnic-compass.h provides an API for the HiTechnic Compass Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 25 November 2009
 * version 0.2
 */

#include "hitechnic-compass.h"

task main () {
  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "Compass");
  displayCenteredTextLine(3, "Test 1");
  displayTextLine(5, "Press enter");
  displayTextLine(6, "to set target");

  sleep(2000);
  eraseDisplay();

  // Create struct to hold sensor data
  tHTMC compass;

  // Initialise and configure struct and port
  initSensor(&compass, S1);

  time1[T1] = 0;
  while(true) {
    // Reset the target no more than once a second
    // This also helps with debouncing the [enter] button.
    if (time1[T1] > 1000) {
      eraseDisplay();
      displayTextLine(1, "Changing");
      displayTextLine(2, "target");
      sleep(500);

      // Read the data from the sensor
      readSensor(&compass);

      // Set the current heading as the value for the offset to be used as the
      // new zero-point for the relative heading returned by
      // compass.relativeHeading
      compass.offset = compass.heading;

      playSound(soundBlip);
      while(bSoundActive) sleep(1);
      time1[T1] = 0;
    }

    // Get the true heading and relative heading from the sensor and
    // display them on the screen.
    while(!getXbuttonValue(xButtonEnter)) {
       // Read the data from the sensor
      readSensor(&compass);
      eraseDisplay();
      displayTextLine(1, "Reading");
      displayTextLine(2, "Target: %4d", compass.offset);
      displayTextLine(4, "Abs:   %4d", compass.heading);
      displayTextLine(5, "Rel:   %4d", compass.relativeHeading);
      displayTextLine(6, "Press enter");
      displayTextLine(7, "to set target");
      sleep(100);
    }
  }
}
