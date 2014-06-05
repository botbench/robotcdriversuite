

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
  tMotor testMotor = motorA;

  playSound(soundBeepBeep);
  displayBigTextLine(3, "Retract");
  FLACretractLA(testMotor, 100);
  sleep(500);

  // This stops the motor, handy in case of emergencies
  FLACstopLA(motorA);
  while(!isDone(testMotor)) sleep(50);
  sleep(200);
  nMotorEncoder[testMotor] = 0;
  sleep(1000);

  playSound(soundBeepBeep);
  eraseDisplay();
  displayBigTextLine(1, "Extend");
  displayTextLine(3, "ticks:  40");
  displayTextLine(4, "speed:  50");
  FLACextendLA(testMotor, 50, 40);
  while(!isDone(testMotor)) sleep(50);
  if (isStalled(testMotor))
    displayTextLine(7, "STALLED");
  sleep(1000);

  playSound(soundBeepBeep);
  eraseDisplay();
  displayBigTextLine(1, "Done");
  sleep(2000);
}
