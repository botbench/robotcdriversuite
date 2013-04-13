/*!@addtogroup HiTechnic
 * @{
 * @defgroup htmmux Motor MUX
 * HiTechnic Motor MUX
 * @{
 */

/*
 * $Id: hitechnic-motormux.h $
 */

#ifndef __HTMMUX_H__
#define __HTMMUX_H__
/** \file hitechnic-motormux.h
 * \brief HiTechnic Motor MUX driver
 *
 * hitechnic-motormux.h provides an API for the HiTechnic Motor MUX driver.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.59 AND HIGHER. 

 * \author Xander Soldaat (xander_at_botbench.com)
 * \date 15 July 2009
 * \version 0.1
 * \example hitechnic-motormux-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#ifndef __MMUX_H__
#include "common-MMUX.h"
#endif

#define HTMMUX_I2C_ADDR         0x10  /*!< HTMMUX I2C device address */

#define HTMMUX_TARG_ENC         0x00  /*!< Target encoder value */
#define HTMMUX_POWER            0x04  /*!< Motor power */
#define HTMMUX_MODE_STAT        0x05  /*!< Mode/status */
#define HTMMUX_CURR_ENC         0x06  /*!< Current encoder value */
#define HTMMUX_CMD_OFFSET       0x40  /*!< Command register offset */
#define HTMMUX_ENTRY_SIZE       0x0A  /*!< Number of registers per motor channel */
#define REG_ADDR(CHAN, REG)     HTMMUX_CMD_OFFSET + (HTMMUX_ENTRY_SIZE * CHAN) + REG

// Motor mode commands
#define HTMMUX_CMD_BRAKE        0x00  /*!< Off - brake command */
#define HTMMUX_CMD_RUN_CSPEED   0x01  /*!< Run with constant speed command */
#define HTMMUX_CMD_RUN_CPOWER   0x02  /*!< Run with constant power command */
#define HTMMUX_CMD_FLOAT        0x03  /*!< Off - coast command */
#define HTMMUX_CMD_RUN_TO_POS   0x04  /*!< Run to and hold position command */
#define HTMMUX_CMD_RESET_ENC    0x05  /*!< Reset current encoder command */

// Motor status fields
#define HTMMUX_STAT_UNAVAIL     0x10  /*!< This motor channel cannot be used status */
#define HTMMUX_STAT_BATT        0x40  /*!< No battery voltage detected status */
#define HTMMUX_STAT_BUSY        0x80  /*!< Motor is currently executing a Run to and hold position status*/

#define HTMMUX_MOTOR1           0x01  /*!< Motor connected to channel 1 */
#define HTMMUX_MOTOR2           0x02  /*!< Motor connected to channel 2 */
#define HTMMUX_MOTOR3           0x03  /*!< Motor connected to channel 3 */
#define HTMMUX_MOTOR4           0x04  /*!< Motor connected to channel 4 */

tByteArray HTMMUX_I2CRequest;    /*!< Array to hold I2C command data */
tByteArray HTMMUX_I2CReply;      /*!< Array to hold I2C reply data */


// Function prototypes
void HTMMUXinit();
ubyte HTMMUXreadStatus(tSensors link, ubyte channel);
bool HTMMUXreadStatus(tSensors link, ubyte channel, ubyte statusbit);
bool HTMMUXsendCommand(tSensors link, ubyte channel, byte command);
bool HTMMUXMotor(tSensors link, ubyte channel, ubyte power, ubyte command);
bool HTMMUXMotorStop(tSensors link, ubyte channel, bool brake);
bool HTMMUXMotorEncoderTarget(tSensors link, ubyte channel, long target);
bool HTMMUXMotorEncoderReset(tSensors link, ubyte channel);
long HTMMUXMotorEncoder(tSensors link, ubyte channel);

// Simplified API for MMUX
void MMotor(tMUXmotor muxmotor, byte power);
void MMotorEncoderTarget(tMUXmotor muxmotor, long target);
long MMotorEncoder(tMUXmotor muxmotor);
void MMotorEncoderReset(tMUXmotor muxmotor);
bool MMotorBusy(tMUXmotor muxmotor);
void MMotorSetBrake(tMUXmotor muxmotor);
void MMotorSetFloat(tMUXmotor muxmotor);
void MMotorSetPIDSpeedCtrl(tMUXmotor muxmotor, bool constspeed);


/*
 * Initialise the mmuxData array needed for keeping track of motor settings
 */
void HTMMUXinit(){
  for (int i = 0; i < 4; i++) {
    memset(mmuxData[i].runToTarget[0], false, 4);
    memset(mmuxData[i].brake[0], true, 4);
    memset(mmuxData[i].pidcontrol[0], true, 4);
    mmuxData[i].initialised = true;
  }
}


/**
 * Read the status and mode of the MMUX
 *
 * The status byte is made up of the following bits:
 *
 * | D7 | D6 | D4 | D3 | D2 | D1 | D1 |
 * - D1:D2 - Used for motor mode commands
 * - D5 - HTMMUX_STAT_UNAVAIL: This motor channel cannot be used
 * - D6 - HTMMUX_STAT_BATT: No battery voltage detected status
 * - D7 - HTMMUX_STAT_BUSY: Motor is currently executing a Run to and hold position
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel we want to check the status of
 * @return the status byte
 */
ubyte HTMMUXreadStatus(tSensors link, ubyte channel) {
  memset(HTMMUX_I2CRequest, 0, sizeof(tByteArray));

  HTMMUX_I2CRequest[0] = 2;               // Message size
  HTMMUX_I2CRequest[1] = HTMMUX_I2C_ADDR; // I2C Address
  HTMMUX_I2CRequest[2] = REG_ADDR(channel, HTMMUX_MODE_STAT);

  if (!writeI2C(link, HTMMUX_I2CRequest, HTMMUX_I2CReply, 1))
    return -1;

  return HTMMUX_I2CReply[0];
}


/**
 * Check if a status bit is set or not for a specified motor channel.
 *
 * The status byte is made up of the following bits:
 *
 * | D7 | D6 | D4 | D3 | D2 | D1 | D1 |
 * - D1:D2 - Used for motor mode commands
 * - D5 - HTMMUX_STAT_UNAVAIL: This motor channel cannot be used
 * - D6 - HTMMUX_STAT_BATT: No battery voltage detected status
 * - D7 - HTMMUX_STAT_BUSY: Motor is currently executing a Run to and hold position
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel we want to check the status of
 * @param statusbit he bit we wish to check
 * @return true if the statusbit is set, false if it isn't
 */
bool HTMMUXreadStatus(tSensors link, ubyte channel, ubyte statusbit) {
  if (HTMMUXreadStatus(link, channel) & statusbit)
    return true;
  else
    return false;
}


/**
 * Send a command to the MMUX.
 *
 * - HTMMUX_CMD_BRAKE: Off - brake command
 * - HTMMUX_CMD_RUN_CSPEED: Run with constant speed command
 * - HTMMUX_CMD_RUN_CPOWER: Run with constant power command
 * - HTMMUX_CMD_FLOAT: Off - coast command
 * - HTMMUX_CMD_RUN_TO_POS: Run to and hold position command
 * - HTMMUX_CMD_RESET_ENC: Reset current encoder command
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel the command should apply to
 * @param command the command to be sent to the motor
 * @return true if no error occured, false if it did
 */
bool HTMMUXsendCommand(tSensors link, ubyte channel, byte command) {
  memset(HTMMUX_I2CRequest, 0, sizeof(tByteArray));

  HTMMUX_I2CRequest[0] = 3;               // Message size
  HTMMUX_I2CRequest[1] = HTMMUX_I2C_ADDR; // I2C Address
  HTMMUX_I2CRequest[2] = REG_ADDR(channel, HTMMUX_MODE_STAT);
  HTMMUX_I2CRequest[3] = command;

  return writeI2C(link, HTMMUX_I2CRequest);
}


/**
 * Run motor with specified speed and command
 *
 * Speed is a number from -25 to +25.  All numbers over 25 are treated as 25.
 * - HTMMUX_CMD_BRAKE: Off - brake command
 * - HTMMUX_CMD_RUN_CSPEED: Run with constant speed command
 * - HTMMUX_CMD_RUN_CPOWER: Run with constant power command
 * - HTMMUX_CMD_FLOAT: Off - coast command
 * - HTMMUX_CMD_RUN_TO_POS: Run to and hold position command
 * - HTMMUX_CMD_RESET_ENC: Reset current encoder command
 *
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel the command should apply to
 * @param power the amount of power to apply to the motor, value between -25 and +25
 * @param command the command to issue to the motor
 * @return true if no error occured, false if it did
 */
bool HTMMUXMotor(tSensors link, ubyte channel, ubyte power, ubyte command) {
  memset(HTMMUX_I2CRequest, 0, sizeof(tByteArray));

  HTMMUX_I2CRequest[0] = 4;               // Message size
  HTMMUX_I2CRequest[1] = HTMMUX_I2C_ADDR; // I2C Address
  HTMMUX_I2CRequest[2] = REG_ADDR(channel, HTMMUX_POWER);
  HTMMUX_I2CRequest[3] = power;
  HTMMUX_I2CRequest[4] = command;

  return writeI2C(link, HTMMUX_I2CRequest);
}

/**
 * Set target encoder for specified motor channel
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel to set target encoder for
 * @param target the target encoder value
 * @return true if no error occured, false if it did
 */
bool HTMMUXMotorEncoderTarget(tSensors link, ubyte channel, long target) {
  memset(HTMMUX_I2CRequest, 0, sizeof(tByteArray));

  HTMMUX_I2CRequest[0] = 6;               // Message size
  HTMMUX_I2CRequest[1] = HTMMUX_I2C_ADDR; // I2C Address
  HTMMUX_I2CRequest[2] = REG_ADDR(channel, HTMMUX_TARG_ENC);
  HTMMUX_I2CRequest[3] = (target >> 24) & 0xFF;
  HTMMUX_I2CRequest[4] = (target >> 16) & 0xFF;
  HTMMUX_I2CRequest[5] = (target >>  8) & 0xFF;
  HTMMUX_I2CRequest[6] = (target >>  0) & 0xFF;

  return writeI2C(link, HTMMUX_I2CRequest);
}

/**
 * Reset target encoder for specified motor channel, use only at
 * the start of your program.  If you are using the standard NXT wheels
 * you will not run into problems with a wrap-around for the first 500kms
 * or so.
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel the command should apply to
 * @return true if no error occured, false if it did
 */
bool HTMMUXMotorEncoderReset(tSensors link, ubyte channel) {
  return HTMMUXsendCommand(link, channel, HTMMUX_CMD_RESET_ENC);
}


/**
 * Fetch the current encoder value for specified motor channel
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel to set target encoder for
 * @return the current value of the encoder
 */
long HTMMUXMotorEncoder(tSensors link, ubyte channel) {
  memset(HTMMUX_I2CRequest, 0, sizeof(tByteArray));

  HTMMUX_I2CRequest[0] = 2;               // Message size
  HTMMUX_I2CRequest[1] = HTMMUX_I2C_ADDR; // I2C Address
  HTMMUX_I2CRequest[2] = REG_ADDR(channel, HTMMUX_CURR_ENC);

  writeI2C(link, HTMMUX_I2CRequest, HTMMUX_I2CReply, 4);

  return ((int)HTMMUX_I2CReply[0] << 24) +
         ((int)HTMMUX_I2CReply[1] << 16) +
         ((int)HTMMUX_I2CReply[2] <<  8) +
          (int)HTMMUX_I2CReply[3];
}


/**
 * Stop the specified motor
 *
 * Note: this is part of the advanced MMUX API.
 * @param link the MMUX port number
 * @param channel the channel to set target encoder for
 * @param brake use braking or floating to stop the motor
 * @return the current value of the encoder
 */
bool HTMMUXMotorStop(tSensors link, ubyte channel, bool brake) {
  if (brake)
    return HTMMUXsendCommand(link, channel, HTMMUX_CMD_BRAKE);
  else
    return HTMMUXsendCommand(link, channel, HTMMUX_CMD_FLOAT);
}


/**
 * Run motor with specified speed.
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 * @param power power the amount of power to apply to the motor, value between -25 and +25
 */
void MMotor(tMUXmotor muxmotor, byte power) {
  bool brake;
  if (power == 0) {
    brake = mmuxData[SPORT(muxmotor)].brake[MPORT(muxmotor)];
    HTMMUXMotorStop((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor), brake);
    return;
  }

  if (mmuxData[SPORT(muxmotor)].runToTarget[MPORT(muxmotor)]) {
    HTMMUXMotor((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor), power, HTMMUX_CMD_RUN_TO_POS);
    mmuxData[SPORT(muxmotor)].runToTarget[MPORT(muxmotor)] = false;
  } else if (mmuxData[SPORT(muxmotor)].pidcontrol[MPORT(muxmotor)]) {
    HTMMUXMotor((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor), power, HTMMUX_CMD_RUN_CSPEED);
  } else {
    HTMMUXMotor((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor), power, HTMMUX_CMD_RUN_CPOWER);
  }
}


/**
 * Set encoder target for specified motor channel.
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 * @param target the encoder target value
 */
void MMotorEncoderTarget(tMUXmotor muxmotor, long target) {
  HTMMUXMotorEncoderTarget((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor), target);
  mmuxData[SPORT(muxmotor)].runToTarget[MPORT(muxmotor)] = true;
}


/**
 * Fetch the current encoder value for specified motor channel
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 * @return the current value of the encoder
 */
long MMotorEncoder(tMUXmotor muxmotor) {
  return HTMMUXMotorEncoder((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor));
}


/**
 * Reset target encoder for specified motor channel, use only at
 * the start of your program.  If you are using the standard NXT wheels
 * you will not run into problems with a wrap-around for the first 500kms
 * or so.
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 */
void MMotorEncoderReset(tMUXmotor muxmotor) {
  HTMMUXMotorEncoderReset((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor));
}


/**
 * Check if the specified motor is still busy doing a "run to and hold".
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 * @return true if the motor is still busy doing a "run to and hold", false if it's done
 */
bool MMotorBusy(tMUXmotor muxmotor) {
  return HTMMUXreadStatus((tSensors)SPORT(muxmotor), (ubyte)MPORT(muxmotor), HTMMUX_STAT_BUSY);
}


/**
 * Set the stopping method for the specified motor to brake.
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 */
void MMotorSetBrake(tMUXmotor muxmotor) {
  mmuxData[SPORT(muxmotor)].brake[MPORT(muxmotor)] = true;
}


/**
 * Set the stopping method for the specified motor to float.
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 */
void MMotorSetFloat(tMUXmotor muxmotor) {
  mmuxData[SPORT(muxmotor)].brake[MPORT(muxmotor)] = false;
}


/**
 * Set the motor speed control.
 *
 * Note: this is part of the simplified MMUX API.
 * @param muxmotor the MMUX motor
 * @param constspeed if set to true, use constant speed control, if set to false use power control only
 */
void MMotorSetPIDSpeedCtrl(tMUXmotor muxmotor, bool constspeed) {
  mmuxData[SPORT(muxmotor)].pidcontrol[MPORT(muxmotor)] = constspeed;
}

#endif //  __HTMMUX_H__

/*
 * $Id: hitechnic-motormux.h $
 */
/* @} */
/* @} */
