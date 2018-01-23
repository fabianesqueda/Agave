// THIS MODULE GENERATES "METALLIC NOISE" SIMILAR TO THAT USED IN THE 
// ROLAND TR-808 AND TR-606 RHYTHM COMPOSERS TO CREATE THE CYMBAL AND 
// HI-HAT VOICES. 
// 
// RECOMMENDED USE: METALLIC_NOISE -> HPF -> VCA WITH SNAPPY ENVELOPES
// 
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - AUGUST 2017
// 
// ADAPTED FOR VCV RACK JANUARY 2018
// 
// TODO: 
// 		ADD MORE NOISE SOURCES: DIGITAL NOISE, DR-110 & KR-55 NOISES


#include "Agave.hpp"
#include <iostream>
#include <array>

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
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float sampleRate = engineGetSampleRate();

	// Declare 2 arrays of oscillators
	std::array<DPWSquare, 6> squareWaves808;
	std::array<DPWSquare, 6> squareWaves606;

	// Define fundamental frequencies
	std::array<float, 6> oscFrequencies808 = {{205.3, 369.4, 304.4, 522.3, 800.0, 540.4}};
	std::array<float, 6> oscFrequencies606 = {{244.4, 304.6, 364.5, 412.1, 432.4, 604.1}};

	MetallicNoise() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		for(auto & squareWave : squareWaves808)
			squareWave.setSampleRate(sampleRate);
		for(auto & squareWave : squareWaves606)
			squareWave.setSampleRate(sampleRate);
	}

	void step() override;
	void onSampleRateChange() override;
	// TODO: reset()

};


void MetallicNoise::step() {

	float output808 = 0.0;
	for (int i=0; i<6; i++) {
		squareWaves808[i].generateSamples(oscFrequencies808[i]);
		output808 += squareWaves808[i].getSquareWaveform();
	}
	outputs[NOISE_808_OUTPUT].value = 5.0f * 0.1666f * output808;

	float output606 = 0.0;
	for (int i=0; i<6; i++) {
		squareWaves606[i].generateSamples(oscFrequencies606[i]);
		output606 += squareWaves606[i].getSquareWaveform();
	}
	outputs[NOISE_606_OUTPUT].value = 5.0f * 0.1666f * output606;

}

void MetallicNoise::onSampleRateChange() {
	sampleRate = engineGetSampleRate();
	for(auto & squareWave : squareWaves808)
		squareWave.setSampleRate(sampleRate);
	for(auto & squareWave : squareWaves606)
		squareWave.setSampleRate(sampleRate);
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
