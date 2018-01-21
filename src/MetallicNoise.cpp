#include "Agave.hpp"
#include <iostream>

#include "dsp/DPWOsc.hpp"

struct MetallicNoise : Module {

	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		NOISE_808_OUTPUT,
		NOISE_606_OUTPUT,
		// TODO: Add DR-110 & KR-55
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	DPWSquare squareWaves808[6];
	DPWSquare squareWaves606[6];

	MetallicNoise() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {	}

	void step() override;

	// TODO: reset()
	// TODO: onSampleRateChange()

};


void MetallicNoise::step() {

	float oscFrequencies808[6] = {205.3, 369.4, 304.4, 522.3, 800.0, 540.4};
	float oscFrequencies606[6] = {244.4, 304.6, 364.5, 412.1, 432.4, 604.1};

	float output808 = 0.0;
	for (int i=0; i<6; i++) {
		squareWaves808[i].generateSamples(oscFrequencies808[i]);
		output808 += squareWaves808[i].output;
	}
	outputs[NOISE_808_OUTPUT].value = 5.0 * 0.1666 * output808;

	float output606 = 0.0;
	for (int i=0; i<6; i++) {
		squareWaves606[i].generateSamples(oscFrequencies606[i]);
		output606 += squareWaves606[i].output;
	}
	outputs[NOISE_606_OUTPUT].value = 5.0 * 0.1666 * output606;

}

MetallicNoiseWidget::MetallicNoiseWidget() {
	MetallicNoise *module = new MetallicNoise();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/TestGeneratorPanel.svg")));
		addChild(panel);
	}

	// SINE OUTPUT
	addOutput(createOutput<PJ301MPort>(Vec(18, 100), module, MetallicNoise::NOISE_808_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 250), module, MetallicNoise::NOISE_606_OUTPUT));

}
