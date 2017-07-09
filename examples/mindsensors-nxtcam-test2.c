/**
 * mindsensors-nxtcam.h provides an API for the Mindsensors NXTCam.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments
 *
 * Credits:
 * - Gordon Wyeth for writing the original driver and cam_display program
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 28 November 2009
 * version 0.2
 */

#define NXTCAMV5

#include "mindsensors-nxtcam.h"

/*
 Main task
 */
task main () {

	// Set up the port alias for the camera, configuration is handled by NXTCAMint()
	tSensors cam = S1;

  eraseDisplay();

  // Initialise the camera
  NXTCAMinit(cam);
  displayCenteredBigTextLine(2, "Insta-Pic");
  displayCenteredTextLine(6, "[LEFT] to take a still");
  displayCenteredTextLine(7, "[RIGHT] to take a short movie");

  while(true)
  {

		if (getXbuttonValue(xButtonLeft))
		{
			NXTCamCaptureStill(cam);
			playSound(soundBeepBeep);
			while (bSoundActive)
				sleep(10);
		}
		else if (getXbuttonValue(xButtonRight))
		{
			NXTCamCaptureMovie(cam);
			playSound(soundBeepBeep);
			while (bSoundActive)
				sleep(10);
			sleep(10000); // movie clips are 10s long
		}
  }
}
