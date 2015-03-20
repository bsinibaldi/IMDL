#include <iostream>
#ifndef PID_H
#define PID_H

/*PID Controller Written by John Sinibaldi, 2015. Based primarily on work and code of Brett Beauregard, located at  http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/ */


//necessary variables

double ITerm, lastInput;
double Kp, Ki, Kd;
unsigned long lastTime;
int SampleTime = 100; //100 milliseconds, aka 0.1 seconds
double outMin, outMax;


void SetSampleTime(int newSampleTime) { //function to change sample time on the fly, for debugging
	
	if (newSampleTime > 0) {
		double ratio = (double)newSampleTime/(double)SampleTime;
		ki *= ratio;
		kd /= ratio;
		SampleTime = (unsigned long)NewSampleTime;
	}
}



double PID(double inVal, double desVal, double kp, double ki, double kd) {
	
	double outVal;
	double SampleTimeSec = ((double)SampleTime)/1000);
	Kp = kp;
	Ki = ki * SampleTimeSec;
	Kd = kd / SampleTimeSec;

	unsigned long currentTime = millis(); //fetches the current time
	int changeTime = (currentTime - lastTime); //change in time
	
	if(changeTime >= SampleTime) { //if we've gone more than one sample time
		double error = desVal - inVal; //where we want to be - where we are
		ITerm += (ki * error); //integral of error, corrects for changes in ki		
		if (ITerm > outmax) ITerm = outMax; //prevents a signal greater than the servo can handle from being sent
		else if (Iterm < outMin) ITerm = outMin; //same as before
		
				
	
		double dinput = (inVal - lastInput); //derivative of error, removes issues with changing desVal

		outVal = Kp * error + ki * errSum - kd *dinput; //value we want
		if(outVal > outMax) outVal = outMax;
		else if (outVal < outMin) outVal = outMin;


		lastInput = inVal; //holds the error for the next run
		lastTime = currentTime; //resets the time
		
		return outVal;
	}
		
}

#endif
