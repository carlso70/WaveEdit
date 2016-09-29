#include "stdafx.h"
#include "FilterEcho.h"


FilterEcho::FilterEcho()
{
}

WaveFile* FilterEcho::transform(float echoAmount, float delayms, WaveFile* oldWave) {
	WaveFile * w2 = new WaveFile(oldWave->getNumChannels(), oldWave->getSampleRate(),
		oldWave->getbitsPerSample());

	int i = 0;
	int t = 0;

	int delayInSamples = oldWave->getSampleRate() * (delayms / 1000);
	while (t < oldWave->getLastSample()) {
		float value = oldWave->get_sample((int)t);
		float delayed = 0;
		if (t - delayInSamples >= 0) {
			delayed = w2->get_sample((int)t - delayInSamples);
		}
		w2->add_sample((int)((1 - echoAmount)*value + echoAmount * delayed));
		t++;
	}

	w2->updateHeader();

	return w2;
}

FilterEcho::~FilterEcho()
{
}
