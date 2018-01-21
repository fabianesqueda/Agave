#pragma once

class DPWSawtooth {

// This class generates a first-order DPW sawtooth suitable for testing.

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

// This class generates a first-order DPW sawtooth suitable for testing.

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