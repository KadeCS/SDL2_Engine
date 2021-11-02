#pragma once
#include "Object.h"

class Particles : public Object
{
public:
	Particles(int v1, int v2, int n, int msTime);
	~Particles() = default;


	void update(Events::updateEvent event) override;

};

