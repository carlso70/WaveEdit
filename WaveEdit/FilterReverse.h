#pragma once
#include "Filter.h"
class FilterReverse :
	public Filter
{
public:
	FilterReverse();

	static WaveFile* transform(float speed, WaveFile* oldWave);

	~FilterReverse();
};

