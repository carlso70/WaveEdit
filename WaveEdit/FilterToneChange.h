#pragma once
#include "Filter.h"
class FilterToneChange :
	public Filter
{
public:
	FilterToneChange();

	static WaveFile* transform(int frequency, WaveFile* w);

	~FilterToneChange();
};

