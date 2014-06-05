#pragma config(Sensor, S1,     HTIRS2,              sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: hitechnic-irseeker-v2-test2.c $
 */

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

// global variables
long nNumbCyles;
long nInits = 0;
string sTextLines[8];

// function prototypes
void displayText(short nLineNumber, const string cChar, short nValueDC, short nValueAC);
void displayText3(short nLineNumber, const string cChar, short nValueDC, short nValueAC, short nValueEnh);
void displayInstructions();

// main task
task main ()
{
  short _dirDC = 0;
  short _dirAC = 0;
	short dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
	short acS1, acS2, acS3, acS4, acS5 = 0;
	short _dirEnh, _strEnh;

	// the default DSP mode is 1200 Hz.
  tHTIRS2DSPMode _mode = DSP_1200;

  // show the user what to do
  displayInstructions();

  while(true)
  {
    // You can switch between the two different DSP modes by pressing the
    // orange enter button

    playSound(soundBeepBeep);
    while(bSoundActive)
    {}
    eraseDisplay();
    nNumbCyles = 0;
    ++nInits;
    while (true)
    {
      if ((nNumbCyles & 0x04) == 0)
        displayTextLine(0, "Initializing...");
      else
        displayTextLine(0, "");
      displayCenteredBigTextLine(1, "IR Seekr");

      // set the DSP to the new mode
      if (HTIRS2setDSPMode(HTIRS2, _mode))
        break; // Sensor initialized

      ++nNumbCyles;
      playSound(soundShortBlip);
      displayTextLine(4, "Inits: %d / %d", nInits, nNumbCyles);
      displayCenteredTextLine(6, "Connect Sensor");
      displayCenteredTextLine(7, "to Port S1");
      sleep(100);
    }

    eraseDisplay();
    for (short i = 0; i < 8; ++i)
      sTextLines[i] = "";

    // display the current DSP mode
    if (_mode == DSP_1200)
      displayTextLine(0, "    DC 1200");
    else
      displayTextLine(0, "    DC  600");

    while (true)
    {
      ++nNumbCyles;
      if (nNxtButtonPressed == kEnterButton)
      {
        // "Enter" button has been pressed. Need to switch mode

        _mode = (_mode == DSP_1200) ?  DSP_600 : DSP_1200;
        while(nNxtButtonPressed == kEnterButton)
        {
          // Wait for "Enter" button release
        }
        break;
      }

      // Read the current non modulated signal direction
      _dirDC = HTIRS2readDCDir(HTIRS2);
      if (_dirDC < 0)
        break; // I2C read error occurred

      // read the current modulated signal direction
      _dirAC = HTIRS2readACDir(HTIRS2);
      if (_dirAC < 0)
        break; // I2C read error occurred

      // Read the individual signal strengths of the internal sensors
      // Do this for both unmodulated (DC) and modulated signals (AC)
      if (!HTIRS2readAllDCStrength(HTIRS2, dcS1, dcS2, dcS3, dcS4, dcS5))
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 ))
        break; // I2C read error occurred

      // Read the Enhanced direction and strength
			if (!HTIRS2readEnhanced(HTIRS2, _dirEnh, _strEnh))
        break; // I2C read error occurred


      displayText3(1, "D", _dirDC, _dirAC, _dirEnh);
      displayText(2, "0", dcS1, acS1);
      displayText(3, "1", dcS2, acS2);
      displayText3(4, "2", dcS3, acS3, _strEnh);
      displayText(5, "3", dcS4, acS4);
      displayText(6, "4", dcS5, acS5);
      displayTextLine(7, "Enter to switch");
    }
  }
}

// Display the instructions to the user
void displayInstructions()
{
  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "IR Seekr");
  displayCenteredTextLine(3, "Test 1");
  displayCenteredTextLine(5, "Press enter to");
  displayCenteredTextLine(6, "switch between");
  displayCenteredTextLine(7, "600 and 1200 Hz");
  sleep(2000);
}

// Minimize LCD screen flicker by only updating LCD when data has changed
void displayText(short nLineNumber, const string cChar, short nValueDC, short nValueAC)
{
  string sTemp;

	StringFormat(sTemp, "%4d %4d", nValueDC, nValueAC);

  // Check if the new line is the same as the previous one
  // Only update screen if it's different.
  if (sTemp != sTextLines[nLineNumber])
  {
    string sTemp2;

    sTextLines[nLineNumber] = sTemp;
    StringFormat(sTemp2, "%s:%s", cChar, sTemp);
    displayTextLine(nLineNumber, sTemp2);
  }
}

// Minimize LCD screen flicker by only updating LCD when data has changed
void displayText3(short nLineNumber, const string cChar, short nValueDC, short nValueAC, short nValueEnh)
{
  string sTemp;

  StringFormat(sTemp, "%4d %4d %3d", nValueDC, nValueAC, nValueEnh);

  // Check if the new line is the same as the previous one
  // Only update screen if it's different.
  if (sTemp != sTextLines[nLineNumber])
  {
    string sTemp2;

    sTextLines[nLineNumber] = sTemp;
    StringFormat(sTemp2, "%s:%s", cChar, sTemp);
    displayTextLine(nLineNumber, sTemp2);
  }
}

/*
 * $Id: hitechnic-irseeker-v2-test2.c $
 */
