#include "stdafx.h"
#include "FilterReverse.h"


FilterReverse::FilterReverse()
{
}

WaveFile* FilterReverse::transform(float speed, WaveFile* oldWave) {
	WaveFile* w2 = new WaveFile(oldWave->getNumChannels(), oldWave->getSampleRate(),
		oldWave->getbitsPerSample());

	int t = oldWave->getLastSample();
	while (t > 0) {
		float value = oldWave->get_sample((int)t);
		w2->add_sample((int)value);
		t--;
	}

	w2->updateHeader();

	return w2;
}

FilterReverse::~FilterReverse()
{
}
