#pragma once

class RCFilter {

	// Class to implement a simple RC filter discretized using trapezoidal rule.
	// TODO: Investigate how does Rack deal with changes in Sampling Rate and find optimal location
	// pre-warping of the cutoff frequency.

public:

	float lowpassOutput = 0.0;
	float highpassOutput = 0.0;
	float previousInput = 0.0;
	float wc = 0.0;

	RCFilter() { }

	~RCFilter() { }

	void setCutoff(float cutoffFrequency, float sampleRate) {	
		
		wc = 2*M_PI*cutoffFrequency; 				// wc in radians/second
		wc = 2*atan(0.5*wc/sampleRate)*sampleRate;	// Cutoff pre-warping
	}

	void process(float input, float sampleRate) {

		float alpha = 2*sampleRate/wc;

		// Compute filter output
		lowpassOutput = ( (alpha - 1)*lowpassOutput + input + previousInput ) / (1 + alpha);
		highpassOutput = input - lowpassOutput;

		// Update State
		previousInput = input;

	}
};