/*!@addtogroup mindsensors
 * @{
 * @defgroup Magic_Wand Magic Wand
 * Magic Wand
 * @{
 */

#ifndef __MSMW_H__
#define __MSMW_H__
/** \file mindsensors-magicwand.h
 * \brief Mindsensors Magic Wand
 *
 * mindsensors-magicwand.h provides additional functionality on top of the PCF8574 driver for
 * the Mindsensors Magic Wand
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Originally written by Mike Partain a.k.a. Spiked3 (http://www.spiked3.com/)
 * - Modified by Xander Soldaat (xander_at_botbench.com)
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

 * \author Mike Partain a.k.a. Spiked3 (http://www.spiked3.com/)
 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 29 July 2012
 * \version 0.1
 * \example mindsensors-magicwand-test1.c
 */

#ifndef __PCF8574_H__
#include "philips-pcf8574.h"
#endif

#define MSMWclearALL(X)   PCF8574sendBytes(X, 0xFF)
#define MSMWsetALL(X)     PCF8574sendBytes(X, 0x00)

/**
 * Turn a specific LED on or off.
 * @param link the Magic Wand port
 * @param ledToChange the index of the LED to change (0-7)
 * @param on stateof the LED, true = on, false = off
 * @return true if no error occured, false if it did
 */
bool MSMWsetLED(tSensors link, ubyte ledToChange, bool on)
{
  ubyte ledState;

  if (!PCF8574readBytes(link, ledState))
  {
    return false;
  }

  return PCF8574sendBytes(link, (on ? ledState & ~(1 << ledToChange) : ledState | (1 << ledToChange)) );
}

/**
 * Toggle a LED
 * @param link the Magic Wand port
 * @param ledToChange the index of the LED to change (0-7)
 * @return true if no error occured, false if it did
 */
bool MSMWtoggleLED(tSensors link, ubyte ledToChange)
{
  ubyte ledState;
  if (!PCF8574readBytes(link, ledState))
  {
    return false;
  }
  return PCF8574sendBytes(link, ledState ^ (1 << ledToChange));
}

/**
 * Flash the LEDs and turn them off again.  Nice effect.
 * @param link the Magic Wand port
 * @param count number of times to flash the LEDs
 * @return true if no error occured, false if it did
 */
bool MSMWflashAndClear(tSensors link, short count)
{
  PCF8574sendBytes(link, 0xFF);    // all off
  for (short i = 0; i < count; i++)
  {
    for (short j = 0; j < 8; j++) {
      if (!MSMWsetLED(link, j, true))
      {
        return false;
      }
      sleep(30);
    }
    for (short j = 0; j < 8; j++) {
      if (!MSMWsetLED(link, j, false))
      {
        return false;
      }
      sleep(30);
    }
    if (!PCF8574sendBytes(link, 0xFF))
    {
      return false;
    }
  }
  PCF8574sendBytes(link, 0xFF);
  return true;
}

#endif //  __MSMW_H__

/* @} */
/* @} */
