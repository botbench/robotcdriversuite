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

// short xscale(short x) - Scales x values from camera coordinates to screen coordinates.
long xscale(long x) {
  return ((x - 12) * 177) / 175;
}

// short yscale(short y) - Scales y values from camera coordinates to screen coordinates.
long yscale(long y) {
  return ((143 - y) * 127) / 143;
}

/*
 Main task
 */
task main () {

	// Set up the port alias for the camera, configuration is handled by NXTCAMint()
	tSensors cam = S1;

  blob_array _blobs;
  // combine all colliding blobs into one
  bool _condensed = true;

  //blob_array _blobs;
  long _l, _t, _r, _b;
  long _x, _y;
  short _nblobs;
  eraseDisplay();

  // Initialise the camera, pick one of the following modes
  NXTCAMinit(cam);				// Normal object (blob) tracking
  // NXTCaminitLine(cam);	// Line tracking
  // NXTCaminitEyes(cam);	// Eye tracking
  // NXTCaminitFace(cam);	// Face tracking

  while(true) {
    // Fetch all the blobs, have the driver combine all
    // the colliding blobs.
    _nblobs = NXTCAMgetBlobs(cam, _blobs, _condensed);
    eraseDisplay();
    for (short i = 0; i < _nblobs; i++) {
      // Draw the scaled blobs
      _l = xscale(_blobs[i].x1);
      _t = yscale(_blobs[i].y1);
      _r = xscale(_blobs[i].x2);
      _b = yscale(_blobs[i].y2);
      fillRect(_l, _t, _r, _b);
    }

    // draw a crosshair on the screen for the average centre of all blobs with
    // colour index 1 and that are larger than 200 pixels square
    NXTCAMgetAverageCenter(_blobs, _nblobs, 1, 200,_x, _y);
    _x = xscale(_x);
    _y = yscale(_y);
    eraseRect(_x -2, _y-10, _x+2, _y+10);
    eraseRect(_x -10, _y-2, _x+10, _y+2);
    drawLine(_x, _y+8, _x, _y-8);
    drawLine(_x+8, _y, _x-8, _y);
    displayTextLine(1, "%d", _nblobs);
    sleep(100);
  }
}
