

/**
 * firgelli-linearact.h provides an API for the Firgelli Linear Actuator.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * TODO:
 * - Add ramping support (being worked on, has a few bugs)
 *
 * Credits:
 * - Big thanks to Firgelli for providing me with a Linear Actuator to play with!
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 15 february 2010
 * version 0.1
 */

#include "firgelli-linearact.h"

task main () {
  tMotor testMotor = motorC;

  // playSound(soundBeepBeep);
  // displayBigTextLine(3, "Retract");
  // FLACtretractLA(testMotor, 100);
  // while(!isDone(testMotor)) sleep(50);
  // sleep(200);
  // nMotorEncoder[testMotor] = 0;
  // sleep(1000);

  playSound(soundBeepBeep);
  eraseDisplay();
  displayBigTextLine(1, "Extend");
  displayTextLine(3, "ticks:  1");
  displayTextLine(4, "speed:  50");
  FLACextendLA(testMotor, 50, 1);
  while(!isDone(testMotor)) sleep(50);
  if (isStalled(testMotor))
    displayTextLine(7, "STALLED");
  sleep(1000);

  // playSound(soundBeepBeep);
  // eraseDisplay();
  // displayBigTextLine(1, "Extend");
  // displayTextLine(3, "ticks: 100");
  // displayTextLine(4, "speed:  80");
  // FLACextendLA(testMotor, 80, 100);
  // while(!isDone(testMotor)) sleep(50);
  // if (isStalled(testMotor))
    // displayTextLine(7, "STALLED");
  // sleep(1000);
  // eraseDisplay();

  // playSound(soundBeepBeep);
  // displayBigTextLine(1, "Retract");
  // displayTextLine(3, "ticks: 120");
  // displayTextLine(4, "speed: 100");
  // FLACtretractLA(testMotor, 100, 120);
  // while(!isDone(testMotor)) sleep(50);
  // if (isStalled(testMotor))
    // displayTextLine(7, "STALLED");
  // sleep(1000);

  playSound(soundBeepBeep);
  displayBigTextLine(1, "Retract");
  displayTextLine(3, "ticks:  1");
  displayTextLine(4, "speed: 100");
  FLACtretractLA(testMotor, 100, 1);
  while(!isDone(testMotor)) sleep(50);
  if (isStalled(testMotor))
    displayTextLine(7, "STALLED");
  sleep(1000);

  playSound(soundBeepBeep);
  eraseDisplay();
  displayBigTextLine(1, "Done");
  sleep(2000);
}
