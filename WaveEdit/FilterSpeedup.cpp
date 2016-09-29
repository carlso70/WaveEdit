#include "stdafx.h"
#include "FilterSpeedup.h"


FilterSpeedup::FilterSpeedup()
{
}

WaveFile * FilterSpeedup::transform(double speed, WaveFile * oldWave)
{
	WaveFile* w2 = oldWave->multiply_freq(speed, 0);
	w2->updateHeader();

	return w2;
}

FilterSpeedup::~FilterSpeedup()
{	
}
