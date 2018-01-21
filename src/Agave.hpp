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

struct WavefolderWidget : ModuleWidget {
	WavefolderWidget();
};

struct MetallicNoiseWidget : ModuleWidget {
	MetallicNoiseWidget();
};