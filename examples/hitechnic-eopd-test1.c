/**
 * HTCS-driver.h provides an API for the HiTechnic EOPD Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Nicer intro
 * - 0.3: More comments
 * - 0.4: Removed common.h from includes
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 20 February 2011
 * version 0.4
 */

#include "hitechnic-eopd.h"

 /*
  =============================================================================
  main task with some testing code

 */
task main() {
#ifdef NXT
  nNxtButtonTask  = -2;
#endif

  eraseDisplay();

  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "EOPD");
  displayCenteredTextLine(3, "Test 1");
  displayCenteredTextLine(5, "Press enter to");
  displayCenteredTextLine(6, "switch between");
  displayCenteredTextLine(7, "ranges");
  sleep(2000);
  eraseDisplay();

  // Create struct to hold sensor data
  tHTEOPD eopdSensor;

  // Initialise and configure struct and port
  // Default mode for the EOPD is short range
  initSensor(&eopdSensor, S1);

  while(true) {
    if (time1[T1] > 1000) {
      if (eopdSensor.shortRange == false) {
        // set the sensor to short range and display this
      	eopdSensor.shortRange = true;
      	configSensor(&eopdSensor);
        displayClearTextLine(1);
        displayTextLine(1, "Short range");
      } else {
        // set the sensor to long range and display this
      	eopdSensor.shortRange = false;
      	configSensor(&eopdSensor);
        displayClearTextLine(1);
        displayTextLine(1, "Long range");
      }
      playSound(soundBeepBeep);
      while(bSoundActive)
      time1[T1] = 0;
    }

#ifdef NXT
    while(nNxtButtonPressed != kEnterButton) {
#else
	  while(!getButtonPress(buttonEnter)) {
#endif
      // Read the sensor values, both the raw and the processed one,
    	// which is linear with the distance detected.  Use the processed
      // value when you want to determine distance to an object
    	readSensor(&eopdSensor);

      displayClearTextLine(3);
      displayClearTextLine(4);
      displayTextLine(4, "Proc:  %4d", eopdSensor.processed);
      displayTextLine(3, "Raw :  %4d", eopdSensor.raw);
      sleep(50);
    }
  }
}
