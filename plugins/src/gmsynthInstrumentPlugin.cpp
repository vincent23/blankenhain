#include "gmsynthInstrumentPlugin.h"
#include "gmsynthInstrumentPluginEditor.h"
#include "gmsynthInstrument.h"
#include "gmDlsInfo.h"

gmsynthInstrumentPlugin::gmsynthInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new gmsynthInstrument)
{
	setUniqueID('bhGM');
	setEditor(new gmsynthPluginEditor(this));
	instruments = getInstrumentInfo();
}

gmsynthInstrumentPlugin::~gmsynthInstrumentPlugin()
{
	if (instruments != nullptr) {
		for (unsigned int i = 0; i < 235; i++) {
			if (instruments[i] != nullptr) {
				delete instruments[i];
			}
		}
		delete[] instruments;
	}
}

void gmsynthInstrumentPlugin::loadMidiInstrument(unsigned int index)
{
	static_cast<gmsynthInstrument*>(effect)->loadMidiInstrument(*instruments[index]);
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new gmsynthInstrumentPlugin(audioMaster);
}