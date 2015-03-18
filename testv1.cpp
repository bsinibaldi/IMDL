/******************************************************************************************
* Test Program: Mac OSX / Unix / Linux C++ Interface for Razor AHRS v1.4.2
* 9 Degree of Measurement Attitude and Heading Reference System
* for Sparkfun "9DOF Razor IMU" and "9DOF Sensor Stick"
*
* Released under GNU GPL (General Public License) v3.0
* Copyright (C) 2013 Peter Bartz [http://ptrbrtz.net]
* Copyright (C) 2011-2012 Quality & Usability Lab, Deutsche Telekom Laboratories, TU Berlin
* Written by Peter Bartz (peter-bartz@gmx.de)
*
* Infos, updates, bug reports, contributions and feedback:
*     https://github.com/ptrbrtz/razor-9dof-ahrs
******************************************************************************************
* Modified for use by John Sinibaldi, 2015
* 
* Monowheel Control Code
* IMDL Lab
* University of Florida
* 
*****************************************************************************************/


#include <iostream>   // cout()
#include <iomanip>    // setprecision() etc.
#include <stdexcept>  // runtime_error
#include <cstdio>     // getchar()
#include <wiringPi.h> // wiringPi library for I/O operations & 
#include "RazorAHRS.h"
#define TILT_ZERO 90 //angle read by IMU when upright. 

using namespace std; //leaving here because of existing code, really don't like this
float tilt = 0; //global variable to determine the angle of the wheel
int direction = 0; //global variable to determine the desired angle of the wheel

// Set your serial port here!
//const string serial_port_name = "/dev/tty.FireFly-6162-SPP"; 
//const string serial_port_name = "/dev/tty.usbserial-A700eEhN";
const string serial_port_name = "/dev/ttyUSB0"; // a good guess on linux


// Razor error callback handler
// Will be called from (and in) Razor background thread!
void on_error(const string &msg)
{
  cout << "  " << "ERROR: " << msg << endl;
  
  // NOTE: make a copy of the message if you want to save it or send it to another thread. Do not
  // save or pass the reference itself, it will not be valid after this function returns! 
}

// Razor data callback handler
// Will be called from (and in) Razor background thread!
// 'data' depends on mode that was set when creating the RazorAHRS object. In this case 'data'
// holds 3 float values: yaw, pitch and roll.
void on_data(const float data[])
{
  cout << "  " << fixed << setprecision(1) 
  << "Yaw = " << setw(6) << data[0] << "      Pitch = " << setw(6) << data[1] << "      Roll = " << setw(6) << data[2] << endl;

  // NOTE: make a copy of the yaw/pitch/roll data if you want to save it or send it to another
  // thread. Do not save or pass the pointer itself, it will not be valid after this function
  // returns!
  
  // If you created the Razor object using RazorAHRS::ACC_MAG_GYR_RAW or RazorAHRS::ACC_MAG_GYR_CALIBRATED
  // instead of RazorAHRS::YAW_PITCH_ROLL, 'data' would contain 9 values that could be printed like this:
  
  // cout << "  " << fixed << setprecision(1)
  // << "ACC = " << setw(6) << data[0] << ", " << setw(6) << data[1] << ", " << setw(6) << data[2]
  // << "        MAG = " << setw(7) << data[3] << ", " << setw(7) << data[4] << ", " << setw(7) << data[5]
  // << "        GYR = " << setw(7) << data[6] << ", " << setw(7) << data[7] << ", " << setw(7) << data[8] << endl;

}

void getData (void) { //Function to actually retrieve the data. Seperated into it's own function for threading purposes. 
  try
  {
    tilt = data[2]; //given our setup, the roll is the angle we need to take into account 	
    // Create Razor AHRS object. Serial I/O will run in background thread and report
    // errors and data updates using the callbacks on_data() and on_error().
    // We want to receive yaw/pitch/roll data. 
    razor = new RazorAHRS(serial_port_name, on_data, on_error, RazorAHRS::YAW_PITCH_ROLL);

  }
  catch(runtime_error &e)
  {
    cout << "  " << (string("Could not create tracker: ") + string(e.what())) << endl;
    cout << "  " << "Did you set your serial port in Example.cpp?" << endl;
    return 0;
  }
}


PI_THREAD(DataAcquisition) //thread to get the angle from the IMU
{
	getData;
}

PI_THREAD(MotorControl) //takes IMU angle, and sensor information, and calculates where the battery should be moved to. 
{			//Uses PID controller code to send out PWM signal to motor
			//Additionally, signal to ESC is sent from here as well (need to figure that out)

	switch(direction) {
		case 0: { //go forward
			}
		case 1: { //go forward and right
			} 
		case 2: { //go forward and left
			}
		case 3: { //go backwards
			}
		case 4: { //go back and right
			}
		case 5: { //go back and left
			}
		case 6: { //stand still (primarily test code)
			}
	}

	
}

PI_THREAD(WatchSensors) //monitors sensors, and decides what direction the robot should be going
//less important right now, will be fleshed out later
{
	SenFL //Front left sensor
	SenFR //Front right sensor
	SenRL //Rear left sensor
	SenRR //Rear right sensor
}

RazorAHRS *razor;

int main()
{
  cout << endl;
  cout << "  " << "Razor AHRS C++ test" << endl;
  cout << "  " << "Press RETURN to connect to tracker. When you're done press RETURN again to quit." << endl;
  getchar();  // wait RETURN
  cout << "  " << "Connecting..." << endl << endl;
  
  x = piThreadCreate(DataAcquisition);
  y = piThreadCreate(WatchSensors);
  z = piThreadCreate(MotorControl); 
  
 
  getchar();  // wait for RETURN key
  return 0;
}
