#pragma once
#include "Filter.h"
class FilterSpeedup :
	public Filter
{
public:
	FilterSpeedup();
	static WaveFile* transform(float speed, WaveFile* oldWave);
	~FilterSpeedup();
};

