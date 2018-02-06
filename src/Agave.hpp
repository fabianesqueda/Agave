#include "rack.hpp"


using namespace rack;

extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct AgaveTestEngineWidget : ModuleWidget {
	AgaveTestEngineWidget();
};

struct LowpassFilterBankWidget : ModuleWidget {
	LowpassFilterBankWidget();
};

struct SharpWavefolderWidget : ModuleWidget {
	SharpWavefolderWidget();
};

struct MetallicNoiseWidget : ModuleWidget {
	MetallicNoiseWidget();
};

struct MS20VCFWidget : ModuleWidget {
	MS20VCFWidget();
};

