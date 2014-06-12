Welcome to the ROBOTC Driver Suite Github repository.
--

What is this?
--------------
The Driver Suite is a collection of drivers for LEGO and 3rd party sensors for both NXT and EV3.  These drivers offer access to additional functionality, which is left unexposed if you use the standard built-in ROBOTC drivers. 

This repository contains a regularly updated work in progress of the migration of (almost) all drivers a new, more easy to use and uniform API.

How to install it?
--------------
If you're keen to try out these bleeding edge drivers, navigate to [this page] [howtopage] and follow the instructions.  Simply substitute the repository in that text with this current one.

You can simply [download as zip] [masterzip] and expand on your system, somewhere.

If you're a more advanced user, I would advise you to use something like [Git] [gitsite] or the more user friendly UI based [SourceTree] [sourcetreesite]

The new API
--------------
So what is so special about this new API?  It's a complete rethink of how the drivers handle sensor data.  All data (or as much as possible), is contained within a sensor specific struct.  For example, the struct that contains I2C data looks as follows:
```
typedef struct
{
  ubyte request[17];
  ubyte requestLen;
  ubyte reply[17];
  ubyte replyLen;
  ubyte address;
  tSensors port;
  TSensorTypes type;
} tI2CData, *tI2CDataPtr;
```
Global variables have been eliminated as much as possible, making the drivers less prone to weird behavioural issues, if multitasking is used.

A sensor specific struct will look more like this.  This is the one for the HiTechnic Angle Sensor:
```
typedef struct
{
  tI2CData I2CData;
  int angle;
  int accumlatedAngle;
  short rpm;
  ubyte _cmd;
  bool smux;
  tMUXSensor smuxport;
} tHTANG, *tHTANGPtr;
```
As you can see, the sensor's struct contains an I2CData struct as well, to keep all the data specific to that sensor in one place.

These functions below may not pertain to all drivers, but if the sensor offers this functionality, it will be accessible through that specific function.  It relies heavily on ROBOTC's function overloading to allow multiple functions with the same name to exist, but taking different arguments.
```
// Setup the sensor's port and struct
bool initSensor(tTIRPtr tirPtr, tSensors port)

// Populate the struct with data
bool readSensor(tTIRPtr tirPtr)

// Reset the sensor
bool resetSensor(tTIRPtr tirPtr)

// Set the offset for a sensor
bool sensorSetOffset(tHTMCPtr htmcPtr);

// Start calibration - not all sensors have a "stop" command
bool sensorCalibrate(tHTMCPtr htmcPtr);

// Stop calibration - not always required
bool sensorStopCalibrate(tHTMCPtr htmcPtr);

// Configure a sensor for a new mode
bool configSensor(tHTEOPDPtr hteopdPtr);
```
If you're interested in seeing an actual example of this, take a look at the [HiTechnic Angle Sensor example program] [htangleexample].

Contributing
--------------
If you want to contribute, simply [clone] [clonehelp] this repo, start tinkering with the files and submit a pull request. I encourage you to read through the examples that I have created and migrate some of the others.  You can see the current status of the drivers in the [Excel sheet] [status_sheet].  Modifications to this file will not be accepted.  If you want to start working on a driver, simply submit it as an [issue] [issuelink].  That will prevent multiple people working on the same drivers.  There is not much point in duplicating effort.

**Please note**: I do reserve the right to modify submitted code as I see fit, to keep it aligned with the rest of the Suite.

**A word of advice**: keep your commits small, this will make merging MUCH easier for me.
Contributors will be mentioned in a THANKYOU file, of course!

[issuelink]: https://github.com/botbench/robotcdriversuite/issues
[status_sheet]: https://github.com/botbench/robotcdriversuite/raw/master/status_drivers.xlsx
[clonehelp]: https://help.github.com/articles/fork-a-repo
[htangleexample]: https://github.com/botbench/robotcdriversuite/blob/master/examples/hitechnic-angle-test1.c
[gitsite]: http://git-scm.com/download/win
[souretreesite]: http://www.sourcetreeapp.com/
[masterzip]: https://github.com/github/linguist/archive/master.zip
[howtopage]: http://botbench.com/blog/2013/12/13/robotc-3-x-driver-suite-is-now-on-git/
