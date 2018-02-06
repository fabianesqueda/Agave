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
	std::array<float, 6> oscFrequencies808 = {{205.3f, 369.4f, 304.4f, 522.3f, 800.0f, 540.4f}};
	std::array<float, 6> oscFrequencies606 = {{244.4f, 304.6f, 364.5f, 412.1f, 432.4f, 604.1f}};

	MetallicNoise() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		for(auto &squareWave : squareWaves808)
			squareWave.setSampleRate(sampleRate);
		for(auto &squareWave : squareWaves606)
			squareWave.setSampleRate(sampleRate);
	}

	void step() override;
	void onSampleRateChange() override;
	// TODO: reset()

};


void MetallicNoise::step() {

	// 808 Noise
	float output808 = 0.0f;
	for (int i=0; i<6; i++) {
		squareWaves808[i].generateSamples(oscFrequencies808[i]);
		output808 += squareWaves808[i].getSquareWaveform();
	}
	outputs[NOISE_808_OUTPUT].value = 5.0f * 0.1666f * output808;

	// 606 Noise
	float output606 = 0.0;
	for (int i=0; i<6; i++) {
		squareWaves606[i].generateSamples(oscFrequencies606[i]);
		output606 += squareWaves606[i].getSquareWaveform();
	}
	outputs[NOISE_606_OUTPUT].value = 5.0f * 0.1666f * output606;

}

void MetallicNoise::onSampleRateChange() {
	for(auto & squareWave : squareWaves808)
		squareWave.setSampleRate(engineGetSampleRate());
	for(auto & squareWave : squareWaves606)
		squareWave.setSampleRate(engineGetSampleRate());
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
