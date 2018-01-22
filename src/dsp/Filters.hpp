// THIS CLASS IMPLEMENTES A FIRST-ORDER LOWPASS FILTER DERIVED FROM THE TRANSFER FUNCTION OF A
// SIMPLE PASSIVE RC FILTER. THE FILTER IS PARAMETRIZED BY SETTING THE CUTOFF FREQUENCY IN HZ
// I.E. fc = 1 / (2*pi*R*C)
//
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - NOVEMBER 2016
// 
// ADAPTED FOR VCV RACK JANUARY 2018
// 
// TODO: 
// 		HANDLE CHANGES IN SAMPLE RATE
// 		ADD MORE STANDARD FILTERS

#pragma once

class RCFilter {

	// Class to implement a simple RC filter discretized using trapezoidal rule.
	// TODO: Investigate how does Rack deal with changes in Sampling Rate and find optimal location
	// pre-warping of the cutoff frequency.

private:
	float lowpassOutput = 0.0;
	float highpassOutput = 0.0;
	float previousInput = 0.0;
	float wc = 0.0;

	RCFilter() {}
	RCFilter(float cutoffFrequency, float sampleRate) { setCutoff(cutoffFrequency, sampleRate); }
	~RCFilter() { }

public:

	void setCutoff(const float &cutoffFrequency, const float &sampleRate) {

		float wa = 2*M_PI*cutoffFrequency; 			// analog cutoff freq
		wc = 2*atan(0.5*wa/sampleRate)*sampleRate;	// digital cutoff freq
	
	}

	void process(const float &input, const float &sampleRate) {

		float alpha = 2*sampleRate/wc;

		// Compute filter output
		lowpassOutput = ( (alpha - 1)*lowpassOutput + input + previousInput ) / (1 + alpha);
		highpassOutput = input - lowpassOutput;

		// Update State
		previousInput = input;

	}

	float getLowpassOutput() {
		return lowpassOutput;
	}

	float getHighpassOutput() {
		return highpassOutput;
	}
};