#pragma once
#include "Filter.h"
class FilterSpeedup :
	public Filter
{
public:
	FilterSpeedup();
	static WaveFile* transform(double speed, WaveFile* oldWave);
	~FilterSpeedup();
};

