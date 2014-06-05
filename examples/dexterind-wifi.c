#pragma config(Sensor, S1,     COLOUR,         sensorColorNxtNONE)
#pragma config(Sensor, S3,     DTMP,           sensorAnalogInactive)
#pragma config(Motor,  motorA,          M1,            tmotorNXT, openLoop, encoder)
#pragma config(MotorPidSetting,  motorB,  255, 255, 255, 65535, 255, 255,   65535, 65535, 65535,   65535, 65535, 65535)
#pragma config(MotorPidSetting,  motorC,  255, 255, 255, 65535, 255, 255,   65535, 65535, 65535,   65535, 65535, 65535)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define __RS485_DEBUG__ 1



#include "common.h"

//tMassiveArray tmp_array;

#include "dexterind-wifi.h"
#include "dexterind-temp.h"

//ubyte rxbuffer[1500];

void sendHeader(short cid) {
  ubyte linebuff[20];
  short index = 0;
  string tmpString;

  linebuff[0] = 27; // escape;
  linebuff[1] = 'S'; // the CID;
  linebuff[2] = (ubyte)cid + 48; // the CID;
  index = RS485appendToBuff(RS485txbuffer, index, linebuff, 3);

  tmpString = "HTTP/1.1 200 OK";
  //memcpy(linebuff, tmpString, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, tmpString, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, linefeed, sizeof(linefeed));

  tmpString = "Content-Type:";
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, linebuff, strlen(tmpString));

  tmpString = " text/plain";
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, linebuff, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, linefeed, sizeof(newline));
  index = RS485appendToBuff(RS485txbuffer, index, linefeed, sizeof(newline));
  linebuff[0] = 27; // escape;
  linebuff[1] = 'E'; // the CID;
  index = RS485appendToBuff(RS485txbuffer, index, endmarker, 2);
  RS485write(RS485txbuffer, index);
}


void genResponse(short cid) {
  short power = motor[motorA];
  float temp = 0.0;
  string tmpString;
  short index = 0;
  ubyte linebuff[20];
  StringFromChars(tmpString, &RS485rxbuffer[0]);
  index = StringFind(tmpString, "/");
  StringDelete(tmpString, 0, index);
  index = StringFind(tmpString, "HTTP");
  StringDelete(tmpString, index, strlen(tmpString));
  writeDebugStreamLine("Request:%s", tmpString);
  displayTextLine(2, "Request: ");
  displayTextLine(3, tmpString);
  if (StringFind(tmpString, "MOTA") > 0) {
    StringDelete(tmpString, 0, 6);
    index = StringFind(tmpString, " ");
  if (index > -1)
      StringDelete(tmpString, index, strlen(tmpString));
    //power = RC_atoix(tmpString);
    power = clip(atoi(tmpString), -100, 100);
    writeDebugStreamLine("Power:%d", power);
  } else {
    writeDebugStreamLine("NO POWER: %s", tmpString);
  }

  sendHeader(cid);
  while(nxtHS_Status == HS_SENDING) sleep(5);

  sleep(100);

  index = 0;
  linebuff[0] = 27; // escape;
  linebuff[1] = 'S'; // the CID;
  linebuff[2] = (ubyte)cid + 48; // the CID;
  index = RS485appendToBuff(RS485txbuffer, index, linebuff, 3);
  StringFormat(tmpString, "MotorA=%d\n", power);
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, linebuff, strlen(tmpString));
  DTMPreadTemp(DTMP, temp);
  StringFormat(tmpString, "Temp: %2.2f C", temp);
  memcpy(linebuff, tmpString, strlen(tmpString));
  index = RS485appendToBuff(RS485txbuffer, index, linebuff, strlen(tmpString));
  linebuff[0] = 27; // escape;
  linebuff[1] = 'E'; // the CID;
  index = RS485appendToBuff(RS485txbuffer, index, endmarker, 2);
  RS485write(RS485txbuffer, index);
  if (power != 0) nMotorEncoderTarget[motorA] = 2000;
  motor[motorA] = power;
  if (power > 0)
    SensorType[COLOUR] = sensorColorNxtGREEN;
  else if (power < 0)
    SensorType[COLOUR] = sensorColorNxtBLUE;
  else if (nMotorRunState[motorA] == runStateIdle)
    SensorType[COLOUR] = sensorColorNxtRED;
  else
    SensorType[COLOUR] = sensorColorNxtRED;
  sleep(300);
  RS485clearRead();
  DWIFIClose(1);
  memset(RS485rxbuffer, 0, sizeof(RS485rxbuffer));
  //sleep(100);
  //RS485read(RS485rxbuffer, sizeof(RS485rxbuffer));
  //clear_read_buffer();
  RS485clearRead();
}


void parseInput()
{
  tMassiveArray tmp_array;
  //char *b_marker = &beginmarker;
  //char *e_marker = &endmarker;
  short len;
  short bpos;
  short epos;
  bool parsed = true;
  writeDebugStreamLine("Beging parsing...");
  //ubyte currByte[] = {0};
  //ubyte prevByte[] = {0};
  ubyte conn[] = {0};
  //short cid;
  //string tmpString;
  short index = 0;
  memset(&tmp_array[0], 0, sizeof(tMassiveArray));
  while (true)
  {
    RS485read(RS485rxbuffer, len, 100);
    if (len > 0)
    {
      writeDebugStreamLine("Appending to buffer: %d", len);
      parsed = false;
      if (index == 0)
        memset(&tmp_array[0], 0, sizeof(tMassiveArray));

      index = RS485appendToBuff(tmp_array, index, &RS485rxbuffer[0], len);
    }
    else if ((parsed == false) && (len == 0))
    {
      writeDebugStreamLine("Parsing buffer: %s", &tmp_array[0]);
      bpos = StringFind((char *)&tmp_array[0], &beginmarker[0]);
      epos = StringFind((char *)&tmp_array[0], &endmarker[0]);
      // we've found a start and end marker
      if (bpos > -1)
        writeDebugStreamLine("begin marker found: %d", bpos);

      if (epos > -1)
        writeDebugStreamLine("end marker found: %d", epos);

      if (bpos > -1 && epos > -1)
      {
        writeDebugStreamLine("bpos: %d, epos: %d", bpos, epos);
      }
      else
      {
        writeDebugStreamLine("no markers found");
      }
      parsed = true;
      index = 0;
      sleep(2000);
      playSound(soundBeepBeep);
    }
	}
}



void startDemon() {
  DWIFIClose();
  sleep(500);
  DWIFITCPOpenServer(80);
  RS485clearRead();
  parseInput();
}

task main()
{
	char *ssid = "Bazinga!";
	char *wpa_psk = "xammy4ever";

	writeDebugStreamLine("ssid: %s", ssid);
	writeDebugStreamLine("psk: %s", wpa_psk);
  short len = 100;
  eraseDisplay();
  bNxtLCDStatusDisplay = true; // Enable top status line display
  writeDebugStream("Scanning for wifi sensor: ");

  // You can play with these to see if they work for you.
  // I tend to use the 460800 rate as it's the fastest speed
  // that I can use reliably.
  //DWIFIsetBAUDRate(9600);
  DWIFIsetBAUDRate(230400);

  DWIFIsetWPAPSK((char *)ssid, (char *)wpa_psk);
  // set_verbose(false);
  // Receive();
  sleep(100);
  playSound(soundBeepBeep);
  //time1[T1] = 0;
  //while (time1[T1] < 30000)
  //{
  //  RS485read(RS485rxbuffer, len, 100);
  //  sleep(500);
  //}
  startDemon();

  sleep(1000);
  RS485clearRead();
  SensorType[COLOUR] = sensorColorNxtRED;
  RS485clearRead();
  parseInput();

  startDemon();
}
