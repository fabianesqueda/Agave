#include "Agave.hpp"
#include <iostream>

#include "dsp/DPWOsc.hpp"


struct AgaveTestEngine : Module {

	enum ParamIds {
		PITCH_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		SINE_OUTPUT,
		SAW_OUTPUT,
		SQUARE_OUTPUT,
		NOISE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float sampleRate = engineGetSampleRate();

	float phase = 0.0;
	float blinkPhase = 0.0;

	DPWSawtooth sawtoothGenerator{sampleRate};
	DPWSquare squareWaveGenerator{sampleRate};

	AgaveTestEngine() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
	void onSampleRateChange() override;
};


void AgaveTestEngine::step() {


	// Implement a simple sine oscillator
	float deltaTime = 1.0f / sampleRate;

	// Compute the frequency from the pitch parameter
	float pitch = params[PITCH_PARAM].value;
	pitch = clampf(pitch, -4.0f, 4.0f);
	float freq = 440.0f * std::pow(2.0f, pitch);

	// Accumulate the phase
	phase += freq * deltaTime;
	if (phase >= 1.0f)
		phase -= 1.0f;

	// Compute the sine output
	float sine = (std::sin(2.0f * M_PI * phase));

	// Output sine
	outputs[SINE_OUTPUT].value = 5.0f * sine;

	// Output sawtooth
	sawtoothGenerator.generateSamples(freq);
	outputs[SAW_OUTPUT].value = 5.0f * sawtoothGenerator.getSawtoothWaveform();

	// Output square wave
	squareWaveGenerator.generateSamples(freq);
	outputs[SQUARE_OUTPUT].value = 5.0f * squareWaveGenerator.getSquareWaveform();

	// Generate white noise
	outputs[NOISE_OUTPUT].value = 5.0f * (2.0f * randomf() - 1.0f);

}

void AgaveTestEngine::onSampleRateChange() {
	sampleRate = engineGetSampleRate();
	sawtoothGenerator.setSampleRate(sampleRate);
	squareWaveGenerator.setSampleRate(sampleRate);
}

AgaveTestEngineWidget::AgaveTestEngineWidget() {
	AgaveTestEngine *module = new AgaveTestEngine();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/TestGeneratorPanel.svg")));
		addChild(panel);
	}

	// KNOB
	addParam(createParam<Davies1900hBlackKnob>(Vec(13, 87), module, AgaveTestEngine::PITCH_PARAM, -4.0, 4.0, 0.0));

	// SINE OUTPUT
	addOutput(createOutput<PJ301MPort>(Vec(18, 180), module, AgaveTestEngine::SINE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 230), module, AgaveTestEngine::SAW_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 280), module, AgaveTestEngine::SQUARE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 330), module, AgaveTestEngine::NOISE_OUTPUT));

}
