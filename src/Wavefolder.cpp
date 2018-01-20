#include "Agave.hpp"
#include <iostream>


struct SergeWavefolder {

	double resistance = 33.0e3;
	double satCurrent = 2.52e-9;
	double thermalVolt = 25.864e-3;
	double idealityFactor = 1.752;

	double output = 0.0;

	SergeWavefolder() {}

	double lambertFunction(double x) {


		double w = 0.0;

		for (int i=0; i<30; i++) {

			double e = expf(w);
	        double p = w*e - x;
	        double r = (w + 1)*e;
	        double s = (w + 2)/(2*(w + 1));
	        double err = p/(r - p*s);

	        w = w - err;

	        if (fabs(err)<10e-12)
	   			break;

		}

		return w;

	}

	void process(double input) {

        // input = input;
        double lambda = 0.0;

        if (input>0.0) {
        	lambda = 1.0;
        }
        else if (input < 0.0) {
        	lambda = -1.0;
        }
        else if (input == 0.0) {
        	lambda = 0.0;
        }

        double u = ( (resistance*satCurrent)/(idealityFactor*thermalVolt) ) * expf( (lambda*input)/(idealityFactor*thermalVolt) );

        // std::cout << u << "\n";

        if (u>1.5e308)
        	std::cout << input << " OVERFLOW!\n";

        // std::cout << ( (resistance*satCurrent)/(idealityFactor*thermalVolt) ) * expf( (lambda*4.04893)/(idealityFactor*thermalVolt) ) << "\n";
        std::cout << expf(4.01854/(idealityFactor*thermalVolt)) << "\n";

        double Lu = lambertFunction(u);
        output = (input - 2.0*lambda*idealityFactor*thermalVolt*Lu);

	}


};

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

	Wavefolder() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) { }

	SergeWavefolder folder;

	void step() override;

	// TODO: reset()

};


void Wavefolder::step() {

	double input = inputs[SIGNAL_INPUT].value;

	// lights[BLINK_LIGHT].value = (input > 5.0) ? 1.0 : 0.0;

	// Scale input to be within [-1 1]
	input = 0.20 * inputs[SIGNAL_INPUT].value * params[FOLDS_PARAM].value;

	// input = clampf(input, -1.0, 1.0);
	folder.process(input);
	// folder.process(folder.output);
	// folder.process(folder.output);
	// folder.process(folder.output);
	// folder.process(folder.output);
	// folder.process(folder.output);

	// folder.output = clampf(folder.output, -1.0, 1.0);

	lights[OUTPUT_LIGHT].value = (folder.output > 1.0) ? 1.0 : 0.0;

	outputs[FOLDED_OUTPUT].value = 5.0 * folder.output;

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
	addParam(createParam<Davies1900hBlackKnob>(Vec(12, 90), module, Wavefolder::FOLDS_PARAM, 0.8, 20.0, 0.8));

	// FOLDED OUTPUT
	addOutput(createOutput<PJ301MPort>(Vec(18, RACK_GRID_HEIGHT-50), module, Wavefolder::FOLDED_OUTPUT));

	// LED Lights
	addChild(createLight<MediumLight<RedLight>>(Vec(25, 15), module, Wavefolder::BLINK_LIGHT));
	addChild(createLight<MediumLight<RedLight>>(Vec(25, RACK_GRID_HEIGHT-65), module, Wavefolder::OUTPUT_LIGHT));

}
