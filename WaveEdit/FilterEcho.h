#pragma once
#include "Filter.h"

class FilterEcho :
	public Filter
{
public:
	FilterEcho();

	static WaveFile* transform(float echoAmount, float delayms, WaveFile* oldWave);

	~FilterEcho();
};

