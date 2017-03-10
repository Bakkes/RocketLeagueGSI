#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include "plugin/bakkesmodplugin.h"

#include "Models.h"
class GameStateIntegrationPlugin : public bakkesmod::plugin::BakkesModPlugin
{
public:
	virtual void onLoad();
	virtual void onUnload();
};

