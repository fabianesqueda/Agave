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
		BLINK_LIGHT,
		OUTPUT_LIGHT,
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

	float input = inputs[SIGNAL_INPUT].value;

	lights[BLINK_LIGHT].value = (input > 5.0) ? 1.0 : 0.0;

	// Scale input to be within [-1 1]
	input = 0.20 * inputs[SIGNAL_INPUT].value + params[FOLDS_PARAM].value;

	input = clampf(input, -1.0, 1.0);

	// lights[OUTPUT_LIGHT].value = (input > 5.0) ? 1.0 : 0.0;

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
	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 90), module, Wavefolder::FOLDS_PARAM, 0.0, 10.0, 0.5));

	// FOLDED OUTPUT
	addOutput(createOutput<PJ301MPort>(Vec(18, RACK_GRID_HEIGHT-50), module, Wavefolder::FOLDED_OUTPUT));

	// LED Lights
	addChild(createLight<MediumLight<RedLight>>(Vec(25, 15), module, Wavefolder::BLINK_LIGHT));
	addChild(createLight<MediumLight<RedLight>>(Vec(25, RACK_GRID_HEIGHT-65), module, Wavefolder::OUTPUT_LIGHT));

}
