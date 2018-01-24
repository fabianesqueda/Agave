// MULTIPLE FILTER CLASSES
//
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - NOVEMBER 2016
// 
// ADAPTED FOR VCV RACK JANUARY 2018
// 
// TODO: 
// 		ADD MORE STANDARD FILTERS
#pragma once
#ifndef RCFILTER_H
#define RCFILTER_H

class RCFilter {
// THIS CLASS IMPLEMENTES AN LTI FIRST-ORDER LOWPASS FILTER DERIVED FROM THE TRANSFER FUNCTION 
// OF A SIMPLE PASSIVE RC FILTER. THE FILTER IS PARAMETRIZED BY SETTING THE CUTOFF FREQUENCY IN HZ
// I.E. fc = 1 / (2*pi*R*C)
// 
// Usage example:
// 	RCFilter filter(100.0f,44100.0f);
// 	filter.process(x);
// 	filter.getLowpassOutput();
// 
private:
	float sampleRate = 44100.0;
	float fc = 1.0e3;
	float previousInput = 0.0;
	float wc = 0.0;
	float lowpassOutput = 0.0;
	float highpassOutput = 0.0;

public:

	// Constructor/Destrcutor
	RCFilter() {}
	RCFilter(float cutoffFrequency, float SR) { 
		fc = cutoffFrequency;
		sampleRate = SR;
		setCutoff();
	}
	~RCFilter() {}

	// Other member functions
	void setSampleRate(float SR);
	void setCutoff();
	void process(float input);
	float getLowpassOutput() { return lowpassOutput; }
	float getHighpassOutput() { return highpassOutput; }
};

#endif

class DCBlocker {

// THIS CLASS IMPLEMENTES AN LTI IIR DC BLOCKER BASED ON J. PEKONEN'S DESIGN, DESCRIBED IN
// "FILTER-BASED ALIAS REDUCTION FOR DIGITAL CLASSICAL WAVEFORM SYNTHESIS" (ICASSP 2008)

private: 

	// Default parameters. Use constructor to overwrite.
	float sampleRate = 44100.0;
	float fc = 10.0;

	float xState = 0.0;
	float yState = 0.0;
	float p = 0.0;

	float output = 0.0;


public:

	// Constructor/Destrcutor
	DCBlocker() { setPole(); }
	DCBlocker(float cutoffFrequency, float SR) { 
		fc = cutoffFrequency;
		sampleRate = SR;
		setPole();
	}
	~DCBlocker() {}

	// Other member functions
	void setSampleRate(float SR);
	void setPole();
	void process(float input);
	float getFilteredOutput() { return output; }

};

