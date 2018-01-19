#include "Agave.hpp"
#include <iostream>

struct Wavefolder : Module {

	enum ParamIds {
		FOLDS_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		SIGNAL_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		FOLDED_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float resistance = 33.0e3;
	float satCurrent = 2.52e-9;
	float thermalVolt = 25.864e-3;
	float idealityFactor = 1.752;

	Wavefolder() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) { }

	void step() override;

	// TODO: reset()

};


void Wavefolder::step() {

	// Scale input to be within [-1 1]
	float input = 0.20 * inputs[SIGNAL_INPUT].value;

	outputs[FOLDED_OUTPUT].value = 5.0 * input;

}

WavefolderWidget::WavefolderWidget() {
	Wavefolder *module = new Wavefolder();
	setModule(module);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/TestGeneratorPanel.svg")));
		addChild(panel);
	}

	// SIGNAL INPUT
	addInput(createInput<PJ301MPort>(Vec(18, 30), module, Wavefolder::SIGNAL_INPUT));

	// FOLD CONTROL
	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 90), module, Wavefolder::FOLDS_PARAM, 0.0, 3.0, 0.0));

	// FOLDED OUTPUT
	addOutput(createOutput<PJ301MPort>(Vec(18, RACK_GRID_HEIGHT-50), module, Wavefolder::FOLDED_OUTPUT));

}
