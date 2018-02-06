// 
// 
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - JANUARY 2018
#include <iostream>
#include <iomanip>

#include "Agave.hpp"
#include "dsp/MS20Filter.hpp"

struct MS20VCF : Module {

	enum ParamIds {
		FREQ_PARAM,
		CV_ATT_PARAM,
		RES_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		SIGNAL_INPUT,
		FREQ_CV_PARAM,
		NUM_INPUTS
	};
	enum OutputIds {
		SIGNAL_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float sampleRate = engineGetSampleRate();

	MS20Filter filter{engineGetSampleRate()};

	MS20VCF() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
	void onSampleRateChange() override;
	void reset() override {
		filter.reset();
	}

};


void MS20VCF::step() {

	// Expects inputs between [-5, 5] but we allow a bit extra ;-)
	float input = inputs[SIGNAL_INPUT].value;
	input = clampf(input, -6.0f, 6.0f);

	// Add -60dB noise to bootstrap self-oscillation
	input += 1.0e-2f * (2.0f*randomf() - 1.0f);

	// Original MS20 used 4.0V pkk
	input *= 1.0f * 0.2f;

	// Set cutoff frequency
	float cutoffCV = params[FREQ_PARAM].value + params[CV_ATT_PARAM].value*inputs[FREQ_CV_PARAM].value*0.2f;
	cutoffCV = clampf(cutoffCV, 0.0f, 1.0f);
	const float minCutoff = 50.0;
	const float maxCutoff = 15.0e3;
	float fc = minCutoff * powf(maxCutoff / minCutoff, cutoffCV);

	// Read resonance
	float k = params[RES_PARAM].value;

	// Push samples into the filter
	filter.process(input, fc, k);

	// Send samples to output buffer 
	outputs[SIGNAL_OUTPUT].value = 1.0f * filter.getOutput();

}

void MS20VCF::onSampleRateChange() {
	filter.setSampleRate(engineGetSampleRate());
}

MS20VCFWidget::MS20VCFWidget() {
	MS20VCF *module = new MS20VCF();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/TestGeneratorPanel.svg")));
		addChild(panel);
	}

	addInput(createInput<PJ301MPort>(Vec(18, 20), module, MS20VCF::SIGNAL_INPUT));

	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 70), module, MS20VCF::FREQ_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<Trimpot>(Vec(20, 120), module, MS20VCF::CV_ATT_PARAM, -1.0, 1.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(18, 150), module, MS20VCF::FREQ_CV_PARAM));

	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 200), module, MS20VCF::RES_PARAM, 0.0, 2.0, 0.0));

	addOutput(createOutput<PJ301MPort>(Vec(18, 340), module, MS20VCF::SIGNAL_OUTPUT));


}
