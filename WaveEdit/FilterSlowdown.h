#pragma once
#include "Filter.h"

class FilterSlowdown :
	public Filter
{
public:
	FilterSlowdown();
	
	static WaveFile* transform(float speed, WaveFile* oldWave);

	~FilterSlowdown();
};