// THIS MODULE IMPLEMENTS A FOUR-STAGE WAVEFOLDER. THE FOLDING
// FUNCTION USED IS VERY SHARP, SIMILAR TO THAT OF THE BUCHLA 
// 259 TIMBRE SECTION. 
// 
// THIS CODE IS PROVIDED "AS-IS", WITH NO GUARANTEE OF ANY KIND.
// 
// CODED BY F. ESQUEDA - DECEMBER 2017
// 
// ADAPTED FOR VCV RACK JANUARY 2018
// 

#include "Agave.hpp"
#include <iostream>
#include <array>

#include "dsp/Waveshaping.hpp"

struct SharpWavefolder : Module {

	enum ParamIds {
		FOLDS_PARAM,
		FOLD_ATT_PARAM,
		SYMM_PARAM,
		SYMM_ATT_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		SIGNAL_INPUT,
		FOLD_CV_INPUT,
		SYMM_CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		FOLDED_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		OUTPUT_LIGHT,
		NUM_LIGHTS
	};

	float sampleRate = engineGetSampleRate();

	std::array<Wavefolder, 4> folder;

	HardClipper clipper;

	SharpWavefolder() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void step() override;
	void onSampleRateChange() override;

};

void SharpWavefolder::step() {

	// Scale input to be within [-1 1]
	float input = 0.2f * inputs[SIGNAL_INPUT].value;
	lights[BLINK_LIGHT].value = (input > 1.0f) ? 1.0f : 1.0f;

	// Read fold cv control
	float foldLevel = params[FOLDS_PARAM].value + params[FOLD_ATT_PARAM].value*std::abs(inputs[FOLD_CV_INPUT].value);
	foldLevel = clampf(foldLevel, -10.0f, 10.0f);

	// Read symmetry cv control
	float symmLevel = params[SYMM_PARAM].value + 0.5f*params[SYMM_ATT_PARAM].value*inputs[SYMM_CV_INPUT].value;
	symmLevel = clampf(symmLevel, -5.0f, 5.0f);

	// Implement wavefolders
	float foldedOutput = input*foldLevel +  symmLevel;
	for (int i=0; i<4; i++) {
		folder[i].process(foldedOutput);
		foldedOutput = folder[i].getFoldedOutput();
	}

	// Saturator
	lights[OUTPUT_LIGHT].value = (foldedOutput >= 1.0f) ? 1.0f : 0.0f;
	clipper.process(foldedOutput);
	foldedOutput = clipper.getClippedOutput();

	// Send samples to output
	outputs[FOLDED_OUTPUT].value = 5.0f * foldedOutput;

}

void SharpWavefolder::onSampleRateChange() {
	sampleRate = engineGetSampleRate();
}

SharpWavefolderWidget::SharpWavefolderWidget() {
	SharpWavefolder *module = new SharpWavefolder();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/TestGeneratorPanel.svg")));
		addChild(panel);
	}

	// KNOBS
	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 70), module, SharpWavefolder::FOLDS_PARAM, 0.90, 10.0, 0.90));
	addParam(createParam<Trimpot>(Vec(20, 120), module, SharpWavefolder::FOLD_ATT_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 200), module, SharpWavefolder::SYMM_PARAM, -5.0, 5.0, 0.0));
	addParam(createParam<Trimpot>(Vec(20, 250), module, SharpWavefolder::SYMM_ATT_PARAM, -1.0, 1.0, 0.0));

	// IN JACKS
	addInput(createInput<PJ301MPort>(Vec(18, 20), module, SharpWavefolder::SIGNAL_INPUT));
	addInput(createInput<PJ301MPort>(Vec(18, 150), module, SharpWavefolder::FOLD_CV_INPUT));
	addInput(createInput<PJ301MPort>(Vec(18, 280), module, SharpWavefolder::SYMM_CV_INPUT));

	// OUT JACKS
	addOutput(createOutput<PJ301MPort>(Vec(18, 340), module, SharpWavefolder::FOLDED_OUTPUT));

	// LEDs
	addChild(createLight<MediumLight<GreenLight>>(Vec(46, 20), module, SharpWavefolder::BLINK_LIGHT));
	addChild(createLight<MediumLight<RedLight>>(Vec(46, 340), module, SharpWavefolder::OUTPUT_LIGHT));

}
