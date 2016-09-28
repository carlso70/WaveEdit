#pragma once
class Filter
{
	friend class WaveFile;
public:
	Filter();
	
	virtual void transform() = 0;

	~Filter();
};

