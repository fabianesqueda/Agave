// THESE CLASSES USE THE WELL-KNOWN DPW ALGORITHM (FIRST-ORDER) TO GENERATE SAWTOOTH AND RECTANGULAR 
// OSCILLATORS WITH REDUCED ALIASING. BASED ON THE ARTICLE:
// 
// "ALIAS-SUPPRESSED OSCILLATORS BASED ON DIFFERENTIATED POLYNOMIAL WAVEFORMS" BY
// V. VÄLIMÄKI, JUHAN NAM AND JULIUS SMITH
// IEEE TRANS. AUDIO, SPEECH, LANGUAGE PROCESS. (MAY 2010)
// 
// NOTES: THE DPW METHOD IS SIMPLE AND WORKS FINE FOR MOST TRIVIAL APPLICATIONS. HOWEVER, IT'S
// NOT RECOMMENDED FOR APPLICATIONS REQUIRING TIME-VARYING PITCH MODULATION OR OSCILLATOR SYNCHRONIZATION. FOR
// MORE ROBUST ALGORITHMS SEE THE E-PTR AND POLYBLEP METHODS.
//
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - JULY 2013
// 
// ADAPTED FOR VCV RACK JANUARY 2018
// 
// TODO: 
// 		ADD PULSE WIDTH DEFINITON FOR SQUARE WAVE

#pragma once

class DPWSawtooth {

private:

	// Default sample rate. Use constructor to overwrite.
	float sampleRate = 44100.0;

	float state = 0.0;
	float phase = 0.0;
	float output = 0.0;

public:

	DPWSawtooth() {}
	DPWSawtooth(float SR) : sampleRate(SR) {}
	~DPWSawtooth() {}

	void setSampleRate(float SR) {
		sampleRate = SR;
	}

	void overridePhase(float newPhase) {
		phase = newPhase; 
	}

	void generateSamples(float f0) {

		// Implement DPW algorithm
		float delta = f0/sampleRate;
		float scalingFactor = sampleRate/(4.0f*f0);
		float modPhase = 2.0f*phase - 1.0f;
		float parWaveform = modPhase*modPhase;
		float dyWaveform = parWaveform - state;

		state = parWaveform;
		output = scalingFactor * dyWaveform; 

		phase += delta;
		if (phase >= 1.0f)
			phase -= 1.0f;

	}

	float getSawtoothWaveform() {
		return output;
	}

};

class DPWSquare {

private:

	float output = 0.0;

	DPWSawtooth sawtoothOne;
	DPWSawtooth sawtoothTwo;

public:

	DPWSquare() { sawtoothTwo.overridePhase(0.5f); }
	DPWSquare(float SR) {
		sawtoothTwo.overridePhase(0.5f);
		setSampleRate(SR); 
	}

	~DPWSquare() {}

	void setSampleRate(float SR) {
		sawtoothOne.setSampleRate(SR);
		sawtoothTwo.setSampleRate(SR);
	}

	void generateSamples(float f0) {

		// DPW Sawtooth is generated from two sawooth waveforms
		sawtoothOne.generateSamples(f0);
		sawtoothTwo.generateSamples(f0);
		output = sawtoothOne.getSawtoothWaveform() - sawtoothTwo.getSawtoothWaveform();
	}

	float getSquareWaveform() {
		return output;
	}

};

// EOF
