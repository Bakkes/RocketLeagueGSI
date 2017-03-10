#include "Commands.h"


string getPlayers(GameWrapper * gw)
{
	ServerWrapper gew = gw->GetGameEventAsServer();
	auto players = gew.GetPlayers();
	ArrayModel<PlayerData> playerData;
	for (unsigned int i = 0; i < players.Count(); i++) {
		auto player = players.Get(i);
		PlayerData pd;
		pd.playerIdx = i;
		pd.playerName = player.GetOwnerName();
		playerData.arr.push_back(pd);
	}
	return toJson(playerData);
}
