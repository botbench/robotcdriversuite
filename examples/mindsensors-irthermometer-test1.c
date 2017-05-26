/**
 * mindsensors-irthermometer.h provides an API for the Mindsensors IR Thermometer Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.53 AND HIGHER

 * Xander Soldaat (xander_at_botbench.com)
 * 18 May 2016
 * version 0.1
 */

#include "mindsensors-irthermometer.h"

task main () {
	tSensors MSIR = S1;
	tMSIR irThermometer;

  displayCenteredTextLine(0, "Mindsensors");
  displayCenteredBigTextLine(1, "IR Thermo");
  displayCenteredTextLine(3, "Test 1");
  sleep(2000);

  eraseDisplay();

  // Create struct to hold sensor data
  // tMSIR irThermometer;

  // Initialise and configure struct and port
  // Set this to false to read in Farenheit
  bool bUseCelcius = true;

  initSensor(&irThermometer, MSIR, bUseCelcius);

  // You can omit bUseCelcius if you only want to measure in Celcius.
  // initSensor(&irThermometer, MSIR);

  while (true)
  {
    if (!readSensor(&irThermometer))
      writeDebugStreamLine("ERROR reading sensor!!");

    displayTextLine(0,"MSIR Test 1");

		displayTextLine(2, "Ambient: %4.2f%s", irThermometer.ambient, irThermometer.bUseCelcius ? "C" : "F");
		displayTextLine(3, "Target:  %4.2f%s", irThermometer.target, irThermometer.bUseCelcius ? "C" : "F");

    sleep(100);
  }
}
