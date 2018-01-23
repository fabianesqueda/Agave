// THIS MODULE IMPLEMENTS A STATIC LOWPASS FILTER BANK. RECOMMENDED FOR
// WIDEBAND INPUTS, SUCH AS WHITE/PINK/RED/ETC NOISE
// 
// 
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - JANUARY 2018
#include <array>
#include <iostream>

#include "Agave.hpp"

#include "dsp/Filters.hpp"

struct LowpassFilterBank : Module {

	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		SIGNAL_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		FILTER_LOW_OUTPUT,
		FILTER_198_OUTPUT,
		FILTER_373_OUTPUT,
		FILTER_692_OUTPUT,
		FILTER_1411_OUTPUT,
		FILTER_HIGH_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float sampleRate = engineGetSampleRate();

	// Declare array of filters
	std::array<RCFilter, NUM_OUTPUTS> filters;

	// In Hz
	std::array<float, 6> cutoffFrequencies = {{78.0, 198.0, 373.0, 692.0, 1411.0, 3.0e3}};

	LowpassFilterBank() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		for (int i = 0; i < NUM_OUTPUTS; i++) { 
			filters[i] = RCFilter(cutoffFrequencies[i], sampleRate); 
		}
	}

	void step() override;
	void onSampleRateChange() override;
	// TODO: reset()

};


void LowpassFilterBank::step() {

	float input = inputs[SIGNAL_INPUT].value;

	for (int i=0; i<NUM_OUTPUTS; i++) {
		filters[i].process(input);
		outputs[i].value = filters[i].getLowpassOutput();
	}

}

void LowpassFilterBank::onSampleRateChange() {
	sampleRate = engineGetSampleRate();
	for(auto & filter : filters)
		filter.setSampleRate(sampleRate);
}

LowpassFilterBankWidget::LowpassFilterBankWidget() {
	LowpassFilterBank *module = new LowpassFilterBank();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/TestGeneratorPanel.svg")));
		addChild(panel);
	}

	// SIGNAL INPUT
	addInput(createInput<PJ301MPort>(Vec(18, 20), module, LowpassFilterBank::SIGNAL_INPUT));

	// SINE OUTPUT
	addOutput(createOutput<PJ301MPort>(Vec(18, 80), module, LowpassFilterBank::FILTER_LOW_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 130), module, LowpassFilterBank::FILTER_198_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 180), module, LowpassFilterBank::FILTER_373_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 230), module, LowpassFilterBank::FILTER_692_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 280), module, LowpassFilterBank::FILTER_1411_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(18, 330), module, LowpassFilterBank::FILTER_HIGH_OUTPUT));

}
