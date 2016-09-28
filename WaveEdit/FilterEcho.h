#pragma once
#include "Filter.h"
class FilterEcho :
	public Filter
{
public:
	FilterEcho();

	void transform();

	~FilterEcho();
};

