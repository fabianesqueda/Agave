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
#ifndef FILTERS_H
#define FILTERS_H 

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

	float sampleRate  = 44.1e3f;
	float fc = 1.0e3f; 	// Cutoff frequency (in Hz)
	float wc;			// Cutoff frequency (in rad/sec)

	float previousInput = 0.0f;
	float lowpassOutput = 0.0f;
	float highpassOutput = 0.0f;

public:

	RCFilter() { setCutoff(); }
	RCFilter(float cutoffFrequency, float SR) { 
		fc = cutoffFrequency;
		sampleRate = SR;
		setCutoff();
	}
	~RCFilter() {}

	void setSampleRate(float SR) {
		sampleRate = SR;
		setCutoff();
	}

	void setCutoff() {
		float wa = 2.0f*M_PI*fc; // analog cutoff freq
		wc = 2.0f*std::atan(0.5f*wa/sampleRate)*sampleRate;	// digital cutoff freq
	}

	void process(float input) {

		float alpha = 2.0f*sampleRate/wc;

		// Compute filter output
		lowpassOutput = ( (alpha - 1.0f)*lowpassOutput + input + previousInput ) / (1.0f + alpha);
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

class DCBlocker {

// THIS CLASS IMPLEMENTES AN LTI IIR DC BLOCKER BASED ON J. PEKONEN'S DESIGN, DESCRIBED IN
// "FILTER-BASED ALIAS REDUCTION FOR DIGITAL CLASSICAL WAVEFORM SYNTHESIS" (ICASSP 2008)
// 
private: 

	// Default parameters. Use constructor to overwrite.
	float sampleRate = 44.1e3f;
	float fc = 1.0e3f;

	float xState = 0.0f;
	float yState = 0.0f;
	float p = 0.0f;
	float output = 0.0f;

public:

	DCBlocker() { setPole(); }
	DCBlocker(float cutoffFrequency, float SR) { 
		fc = cutoffFrequency;
		sampleRate = SR;
		setPole();
	}
	~DCBlocker() {}

	void setSampleRate(float SR) {
		sampleRate = SR;
		setPole();
	}

	void setPole() {
		p = std::tan(0.25f*M_PI - M_PI*fc/sampleRate); // Filter pole
	}

	void process(float input) {

		output = 0.5f*(1.0 + p) * ( input - xState + p*yState ); 

		// Update State
		xState = input;
		yState = output;

	}

	float getFilteredOutput() {
		return output;
	}
};

#endif 


