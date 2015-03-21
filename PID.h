#include <iostream>
#include <math.h>
#define MASS_WHEEL 3.2
#define MASS_BATTERY 1
#define LEVER_WHEEL 8
#define LEVER_BATTERY 6

#ifndef PID_H
#define PID_H


/*PID Controller Written by John Sinibaldi, 2015. Based primarily on work and code of Brett Beauregard, located at  http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/ */


//necessary variables

double ITerm, lastInput;
double Kp, Ki, Kd;
unsigned long lastTime;
int SampleTime = 100; //100 milliseconds, aka 0.1 seconds
double outMin = 15; //PWM signal of minimum (lowest position of battery)
double outMax = 200; //PWM signal of maximum (farthest position of battery)


void SetSampleTime(int newSampleTime) { //function to change sample time on the fly, for debugging
	
	if (newSampleTime > 0) {
		double ratio = (double)newSampleTime/(double)SampleTime;
		ki *= ratio;
		kd /= ratio;
		SampleTime = (unsigned long)NewSampleTime;
	}
}



double PID(double inVal, double desVal, double battPos, double kp, double ki, double kd) {
	
	double outVal;
	double SampleTimeSec = ((double)SampleTime)/1000);
	Kp = kp;
	Ki = ki * SampleTimeSec;
	Kd = kd / SampleTimeSec;

	unsigned long currentTime = millis(); //fetches the current time
	int changeTime = (currentTime - lastTime); //change in time
	
	double posWheel = inVal; //position of the wheel
	double desBatt; //desired position of the battery
	
	//following code will need to be tuned. This counts on the servo taking long enough
	//to come back that the wheel will finish tilting. If the wheel goes to far it will fall
	//If the wheel doesn't go far enough, nothing will happen.
	if (posWheel <= (desVal/4)) { //initially, move in the same direction as the wheel
		desBatt = (desVal/4); //set the battery position to temporary position
	}
	
	else { //after the wheel has tilted, counteract that tilt
		desBatt = asin(MASS_WHEEL * LEVER_WHEEL * sin(posWheel) / (LEVER_BATTERY * MASS_BATTERY)); //balances moments
	}
	
	if(changeTime >= SampleTime) { //if we've gone more than one sample time
		double error = desBatt - battPos; //where we want to be - where we are
		ITerm += (ki * error); //integral of error, corrects for changes in ki		
		if (ITerm > outmax) ITerm = outMax; //prevents a signal greater than the servo can handle from being sent
		else if (Iterm < outMin) ITerm = outMin; //same as before
		
				
	
		double dinput = (battPos - lastInput); //derivative of error, removes issues with changing desVal

		outVal = Kp * error + ki * errSum - kd *dinput; //value we want
		if(outVal > outMax) outVal = outMax;
		else if (outVal < outMin) outVal = outMin;


		lastInput = battPos; //holds the last position for the next run
		lastTime = currentTime; //resets the time
		
		return outVal;
	}
		
}

#endif
