#include "stdafx.h"
#include "FilterToneChange.h"


FilterToneChange::FilterToneChange()
{
}

WaveFile* FilterToneChange::transform(int frequency, WaveFile* w) {
	WaveFile* w2 = new WaveFile(w->getNumChannels(), w->getSampleRate(), w->getbitsPerSample());
	int i = 0;
	for (i = 0; i < w->getLastSample(); i++) {
		float value = w->get_sample((int)i);
		float toneValue = 32767 * sin(3.1415 * frequency * i / w->getSampleRate());
		w2->add_sample(value * toneValue);
	}

	w2->updateHeader();

	return w2;
}

FilterToneChange::~FilterToneChange()
{
}
