#include "stdafx.h"
#include "FilterSlowdown.h"


FilterSlowdown::FilterSlowdown()
{
}

WaveFile * FilterSlowdown::transform(float speed, WaveFile * oldWave)
{
	WaveFile *w2 = oldWave->multiply_freq(speed, 0);
	w2->updateHeader();

	return w2;
}


FilterSlowdown::~FilterSlowdown()
{
}
