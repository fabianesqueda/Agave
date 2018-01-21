// THESE CLASSES USE THE WELL-KNOWN DPW ALGORITHM (FIRST-ORDER) TO GENERATE SAWTOOTH AND RECTANGULAR 
// OSCILLATORS WITH REDUCED ALIASING. BASED ON THE ARTICLE:
// 
// "ALIAS-SUPPRESSED OSCILLATORS BASED ON DIFFERENTIATED POLYNOMIAL WAVEFORMS" BY
// V. VÄLIMÄKI, JUHAN NAM AND JULIUS SMITH
// IEEE TRANS. AUDIO, SPEECH, LANGUAGE PROCESS. (MAY 2010)
// 
// NOTES: THE DPW IS SIMPLE AND WORKS FINE FOR SIMPLE APPLICATIONS. HOWEVER, IT'S NOT SUITABLE
// FOR APPLICATIONS REQUIRING TIME-VARYING PITCH MODULATION OR OSCILLATOR SYNCHRONIZATION. FOR
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
// 		HANDLE CHANGES IN SAMPLE RATE

#pragma once

class DPWSawtooth {

public:

	float sampleRate{engineGetSampleRate()};
	float state = 0.0;
	float phase = 0.0;
	float output = 0.0;

	DPWSawtooth() {}

	~DPWSawtooth() {}

	void overridePhase(float ph) { phase = ph;	}

	void generateSamples(float f0) {

		float delta = f0/sampleRate;
		float scalingFactor = sampleRate/(4.0*f0*(1.0-delta));
		float modPhase = 2.0*phase - 1.0;
		float parWaveform = modPhase*modPhase;
		float dyWaveform = parWaveform - state;

		state = parWaveform;
		output = scalingFactor * dyWaveform; 

		phase += delta;
		if (phase >= 1.0)
			phase -= 1.0;
	}

};

class DPWSquare {

public:

	float output = 0.0;

	DPWSawtooth sawtoothOne;
	DPWSawtooth sawtoothTwo;

	DPWSquare() { sawtoothTwo.overridePhase(0.5); }
	~DPWSquare() {}

	void generateSamples(float f0) {

		sawtoothOne.generateSamples(f0);
		sawtoothTwo.generateSamples(f0);
		output = sawtoothOne.output - sawtoothTwo.output;
	}

};