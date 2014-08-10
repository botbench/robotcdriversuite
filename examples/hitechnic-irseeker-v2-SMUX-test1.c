/**
 * hitechnic-irseeker-v2.h provides an API for the HiTechnic IR Seeker V2.  This program
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
 * Dick Swan
 * Gus Jansson
 * 04 October 2012
 * version 0.1
 */

#include "hitechnic-irseeker-v2.h"

// main task
task main ()
{
  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "IRSeekr2");
  displayCenteredTextLine(3, "SMUX Test");
  displayCenteredTextLine(5, "Connect SMUX to");
  displayCenteredTextLine(6, "S1 and sensor to");
  displayCenteredTextLine(7, "SMUX Port 1");
  sleep(2000);

  // Create struct to hold sensor data
  tHTIRS2 irSeeker;

	// The sensor is connected to the first port
	// of the SMUX which is connected to the NXT port S1.
	// To access that sensor, we must use msensor_S1_1.  If the sensor
	// were connected to 3rd port of the SMUX connected to the NXT port S4,
	// we would use msensor_S4_3

  // Initialise and configure struct and port
  initSensor(&irSeeker, msensor_S1_1);

  while(true)
  {
    // You can switch between the two different DSP modes by pressing the
    // orange enter button

    playSound(soundBeepBeep);
    while(bSoundActive)
    {}
    eraseDisplay();

    displayTextLine(0, "    DC 1200");

    while (true)
    {
      // Read the sensor data.
      readSensor(&irSeeker);
      displayTextLine(1, "D:%4d %4d 3%d", irSeeker.dcDirection, irSeeker.acDirection, irSeeker.enhDirection);
      displayTextLine(2, "0:%4d %d", irSeeker.dcValues[0], irSeeker.acValues[0]);
      displayTextLine(3, "0:%4d %4d", irSeeker.dcValues[1], irSeeker.acValues[1]);
      displayTextLine(4, "0:%4d %4d %3d", irSeeker.dcValues[2], irSeeker.acValues[2], irSeeker.enhStrength);
      displayTextLine(5, "0:%4d %4d", irSeeker.dcValues[3], irSeeker.acValues[3]);
      displayTextLine(6, "0:%4d %4d", irSeeker.dcValues[4], irSeeker.acValues[4]);
      displayTextLine(7, "Enter to switch");
    }
  }
}
