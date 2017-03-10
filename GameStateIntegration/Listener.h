#pragma once
#include "wrappers/gamewrapper.h"
class Listener
{
public:
	Listener();
	~Listener();

	string GetListenData(GameWrapper* gw);
};

