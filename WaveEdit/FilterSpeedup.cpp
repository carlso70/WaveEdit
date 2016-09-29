#include "stdafx.h"
#include "FilterSpeedup.h"


FilterSpeedup::FilterSpeedup()
{
}

WaveFile * FilterSpeedup::transform(float speed, WaveFile * oldWave)
{
	WaveFile *w2 = new WaveFile(oldWave->getNumChannels(), oldWave->getSampleRate() * speed, 
		oldWave->getbitsPerSample());

	int t = 0;
	while (t < oldWave->getLastSample()) {
		float value = oldWave->get_sample((int)t);
		w2->add_sample((int)value);
		t++;
	}

	w2->updateHeader();

	return w2;
}


FilterSpeedup::~FilterSpeedup()
{
}
