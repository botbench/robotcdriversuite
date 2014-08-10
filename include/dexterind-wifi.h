
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                 Setup for Wifi Sensor
//                 Sets up flow control
// Script for connecting to internet
//
// ate0     // Turn off the echo.
// at&k1    // Enable software flow control.
//
// at+wauth=1 // Authentication mode: open
// at+wwep1=0fb3ba79eb  // Set WEP key
// at+wa=NPX97    // Connect to the SSID
// at+ndhcp=1   // Enable DHCP <Dynamic Host Configuration Protocol>
//
// at+dnslookup=www.dexterindustries.com  // Lookup the DNS configuration.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#ifndef __RS485_H__
#include "common-rs485.h"
#endif

#define DEBUG_WIFI 1

long nRcvChars = 0;
ubyte BytesRead[8];
char linefeed[] = {0x0A};
char beginmarker[] = {27, 'S', 0};
char endmarker[] = {27, 'E', 0};

long DWIFI_baudRates[] = {9600, 19200, 38400, 57600, 115200, 230400,460800, 921600};
string DWIFIscratchString;
tMassiveArray DWIFIscratchArray;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Setup High Speed on Port 4.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DWIFIcheckResult(tMassiveArray &readdata, char * caller = NULL)
{
  short len;
  // Read the RX buffer, but only wait 50ms max.
  if (!RS485read(readdata, len, 50))
    return false;

  if (len < 1)
    return false;

  if ((stringFind((char *)readdata, "OK") > -1) || (stringFind((char *)readdata, "0") > -1))
  {
    if (caller != NULL)
      writeDebugStreamLine("DWIFIcheckResult %s SUCCESS", caller);
    else
      writeDebugStreamLine("DWIFIcheckResult SUCCESS");
    return true;
  }
  else
  {
    if (caller != NULL)
      writeDebugStreamLine("DWIFIcheckResult %s FAILURE", caller);
    else
      writeDebugStreamLine("DWIFIcheckResult FAILURE");
    return false;
  }
}

bool DWIFIcheckResult(tMassiveArray &readdata, short timeout, char * caller = NULL)
{
  short len;
  // Read the RX buffer, but only wait 50ms max.

  // writeDebugStreamLine("DWIFIcheckResult with extra timeout");

  if (!RS485read(readdata, len, timeout))
    return false;

  if (len < 1)
    return false;

  if ((stringFind((char *)readdata, "OK") > -1) || (stringFind((char *)readdata, "0") > -1))
  {
    if (caller != NULL)
      writeDebugStreamLine("DWIFIcheckResult %s SUCCESS", caller);
    else
      writeDebugStreamLine("DWIFIcheckResult SUCCESS");
    return true;
  }
  else
  {
    if (caller != NULL)
      writeDebugStreamLine("DWIFIcheckResult %s FAILURE", caller);
    else
      writeDebugStreamLine("DWIFIcheckResult FAILURE");
    return false;
  }
}

short checkFailure() {
  ubyte currByte[] = {0};
  ubyte prevByte[] = {0};

  while (nxtGetAvailHSBytes() == 0) sleep(5);

  while (nxtGetAvailHSBytes() > 0) {
    nxtReadRawHS(&currByte[0], 1);
    if ((prevByte[0] == 27) && (currByte[0] == 'F'))
      return 1;
    else if ((currByte[0] > '0') && (currByte[0] < '7'))
      return 1;
    else if (currByte[0] == '9')
      return 2;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Echo All Input Off - Turns off the echo effect on the wifi.
//      Sending the serial command "ate0" which turns off the echo effect.
//      Sends one single byte at a time, pauses.
//      Drains receiver with a read each time.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void DWIFIsetEcho(bool on)
{
  writeDebugStreamLine("DWIFIsetEcho");

  ubyte nData[] = {'A','T','E','0',0x0A};
  if (on)
  {
    nData[3] = '1';
  }

  for(short i = 0; i < 5; i++){
    nxtWriteRawHS(&nData[i], 1);            // Send the command, byte by byte.
    nxtReadRawHS(&BytesRead[0], 8);         // Clear out the echo.
    sleep(100);
  }
  RS485clearRead();
}

bool DWIFIsetVerbose(bool on)
{
  bool res;
  writeDebugStreamLine("DWIFIsetVerbose");

  if (on)
    res = RS485sendString("ATV1\n");
  else
    res = RS485sendString("ATV0\n");

  if (!res)
    return false;

  return DWIFIcheckResult(RS485rxbuffer, "DWIFIsetVerbose");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Software Flow Control On
//      Send string "at&k1" and carriage return.
//      Shouldn't need the wait or read now that we've got the echo off.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DWIFIsetSWControl()
{
  //DWIFIscratchString = "AT&K1\n";
  //memcpy(RS485txbuffer, DWIFIscratchString, strlen(DWIFIscratchString));
  //return RS485write(RS485txbuffer, strlen(DWIFIscratchString));
  if (!RS485sendString("AT&K1\n"))
    return false;

  return DWIFIcheckResult(RS485rxbuffer, "DWIFIsetSWControl");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Scan for networks
//      Send string "AT+WS" and carriage return.
//      Don't really need to do this; gets back a tonne of data.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DWIFIscanNetworks()
{
  //DWIFIscratchString = "AT+ws\n";
  //memcpy(RS485txbuffer, DWIFIscratchString, strlen(DWIFIscratchString));
  //return RS485write(RS485txbuffer, strlen(DWIFIscratchString));
  return RS485sendString("AT+ws\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      WIFI:  Set Authentication Mode
//             Send string "at+wauth=<n>" and carriage return.
//              n = 0 --> Off
//              n = 1 --> On
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DWIFIsetAuthMode(short state)
{
  if (state == 0)
    return RS485sendString("AT+WAUTH=0\n");
  else
    return RS485sendString("AT+WAUTH=1\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      WIFI:  Set WEP key.
//             Send string "at+WWEPn=<m>" and carriage return.
//
//                  m = WEP Key.
//                  n = type of WEP key.
//                        n= 0
//                        n= 1
//                        n= 2
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DWIFIsetSSID(char *_ssid)
{
  writeDebugStreamLine("DWIFIsetSSID");
  char *ssid_cmd = &DWIFIscratchArray;
  memset(ssid_cmd, 0, sizeof(tHugeByteArray));
  sprintf(ssid_cmd, "AT+WA=%s\n", _ssid);
  RS485sendString(ssid_cmd);
  return DWIFIcheckResult(RS485rxbuffer, 500, "DWIFIsetSSID");
}

bool DWIFIsetWEPKey(short keyindex, char *_wep_key)
{
  writeDebugStreamLine("DWIFIsetWEPKey");
  char *set_wep_psk_cmd = &DWIFIscratchArray;
  memset(set_wep_psk_cmd, 0, sizeof(tHugeByteArray));
  sprintf(set_wep_psk_cmd, "AT+WWEP%d=%s\n", keyindex, _wep_key);
  return RS485sendString(set_wep_psk_cmd);
}

bool DWIFIsetWPAPSK(char *ssid, char *_wpa_key)
{
  short len;
  writeDebugStreamLine("DWIFIsetWPAPSK");

  char *set_wpa_psk_cmd = &DWIFIscratchArray;
  memset(set_wpa_psk_cmd, 0, sizeof(tHugeByteArray));
  sprintf(set_wpa_psk_cmd, "AT+WPAPSK=%s,%s\n", ssid, _wpa_key);
  if (!RS485sendString(set_wpa_psk_cmd))
    return false;
  //RS485read(RS485rxbuffer, len, 50);
  sleep(100);
  RS485clearRead();
  return DWIFIcheckResult(RS485rxbuffer, 30000, "DWIFIsetWPAPSK");
}

bool DWIFIsetDHCP(bool useDHCP)
{
  if (useDHCP)
    RS485sendString("AT+NDHCP=1\n");
  else
    RS485sendString("AT+NDHCP=0\n");

  return DWIFIcheckResult(RS485rxbuffer, 1000, "DWIFIsetDHCP");
}

bool getFW()
{
  writeDebugStreamLine("getFW");
  return RS485sendString("AT+VER=?\n");
}

bool getInfo() {
  writeDebugStreamLine("getInfo");
  return RS485sendString("AT+NSTAT=?\n");
}

bool getInfoWLAN() {
  writeDebugStreamLine("getInfo");
  return RS485sendString("AT+WSTATUS\n");
}

bool DWIFIClose(short cid = -1) {
  short len;
  bool res;

  writeDebugStreamLine("DWIFIClose");

  if (cid < 0)
    res = RS485sendString("AT+NCLOSEALL\n");
  else
  {
    stringFormat(DWIFIscratchString, "AT+NCLOSE=%d\n", cid);
    res = RS485sendString(DWIFIscratchString);
  }

  if (!res)
    return false;

  return DWIFIcheckResult(RS485rxbuffer, "DWIFIClose");
  //writeDebugStreamLine("closeAllCons");
  //return RS485sendString("AT+NCLOSEALL\n");
}

//bool closeConn(short cid) {
//  writeDebugStreamLine("closeConn");
//  stringFormat(DWIFIscratchString, "AT+NCLOSE=%d\n", cid);
//  return RS485sendString(DWIFIscratchString);
//}

bool DWIFIsaveConfig() {
  short len;
  writeDebugStreamLine("save config");
  RS485sendString("AT&W0\n");
  DWIFIcheckResult(RS485rxbuffer, "DWIFIsaveConfig W0");
  sleep(500);
  RS485sendString("AT&Y0\n");
  return DWIFIcheckResult(RS485rxbuffer, "DWIFIsaveConfig Y0");
}

void DWIFIresetConfig()
{
  RS485sendString("AT&F\n");
  sleep(100);
  RS485clearRead();
}

void DWIFITCPOpenServer(long port) {
  short index = 0;
  short len = 0;

  string listen_cmd;
  stringFormat(listen_cmd, "AT+NSTCP=%d\n", port);
  RS485sendString(listen_cmd);
  RS485read(RS485rxbuffer, len, 100);
}

bool checkBaudRate(long baudrate)
{
  //short len = 0;
  //char *tmpbuff;
  string tmpString;
  writeDebugStreamLine("testing baudrate: %d", baudrate);
  nxtDisableHSPort();
  sleep(10);
  nxtEnableHSPort();
  nxtSetHSBaudRate(baudrate);
  nxtHS_Mode = hsRawMode;
  RS485clearRead();
  sleep(100);
  RS485sendString("+++\n");
  sleep(1000);
  RS485clearRead();
  sleep(100);
  RS485sendString("AT\n");
  return DWIFIcheckResult(RS485rxbuffer, "checkBaudRate");
}

long DWIFIscanBaudRate() {
  for (short i = 0; i < 8; i++) {
    if (checkBaudRate(DWIFI_baudRates[i]))
    {
      RS485clearRead();
      return DWIFI_baudRates[i];
    }
  }
  RS485clearRead();
  return 0;
}

void DWIFIsetBAUDRate(long baudrate) {
  short index = 0;
  long current_baudrate = 0;

  string baud_cmd;

  if (checkBaudRate(baudrate))
  {
    writeDebugStreamLine("DWIFIsetBAUDRate: already correct");
    return;
  }

  current_baudrate = DWIFIscanBaudRate();
  if (current_baudrate == 0)
  {
    writeDebugStreamLine("DWIFIsetBAUDRate could not determine baudrate");
    return;
  }

  stringFormat(baud_cmd, "ATB=%d\n", baudrate);
  RS485sendString(baud_cmd);

  sleep(100);
  nxtDisableHSPort();
  sleep(10);
  nxtEnableHSPort();
  nxtSetHSBaudRate(baudrate);
  nxtHS_Mode = hsRawMode;
  sleep(10);
  RS485clearRead();
  sleep(10);

  // Verify the speed to ensure everything went OK
  if (checkBaudRate(baudrate))
  {
    writeDebugStreamLine("DWIFIsetBAUDRate: reconfigured succesfully");
  }
  else
  {
    writeDebugStreamLine("DWIFIsetBAUDRate: reconfigured unsuccesfully");
  }
}

/*
    IP              SubNet         Gateway
 192.168.178.26: 255.255.255.0: 192.168.178.1
 */
void DWIFIwaitForIP()
{
  string IPscratch;
  char *e_marker = "Gateway";
  char *token_begin = " ";
  char *token_end = ":";

  tMassiveArray tmp_array;
  short len;
  short bpos;
  short epos;
  bool parsed = true;
  short startIPinfo = 0;
  short nextTokenStart = 0;
  writeDebugStreamLine("Beging parsing...");
  ubyte conn[] = {0};
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
      epos = stringFind((char *)&tmp_array[0], &e_marker[0]);
      // we've found a start and end marker

      if (epos > -1)
      {
        writeDebugStreamLine("end marker found: %d", epos);
        for (short i = 0; i < 3; i++)
        {
          writeDebugStreamLine("nextTokenStart: %d", nextTokenStart);
          nextTokenStart = epos;
          memmove((char *)&tmp_array[0], (char *)&tmp_array[nextTokenStart], sizeof(tMassiveArray) - nextTokenStart);
          bpos = stringFind((char *)&tmp_array[0], token_begin);
          epos = stringFind((char *)&tmp_array[0], token_end);
          if (bpos > -1 && epos > -1)
          {
            writeDebugStreamLine("IP bpos: %d, epos: %d", bpos, epos);
            memcpy(IPscratch, (char *)&tmp_array[bpos], epos - bpos);
            writeDebugStreamLine("IPinfo: %s", IPscratch);
          }
        }
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
