#include "Commands.h"
#include "wrappers/priwrapper.h"

string getPlayers(GameWrapper * gw)
{
	ServerWrapper gew = gw->GetGameEventAsServer2();
	auto players = gew.GetPRIs();
	ArrayModel<PlayerData> playerData;
	for (unsigned int i = 0; i < players.Count(); i++) {
		auto player = players.Get(i);
		PlayerData pd;
		pd.playerIdx = i;
		
		pd.playerName = player.GetPlayerName();
		pd.goals = player.GetGoals();
		pd.assists = player.GetAssists();
		pd.saves = player.GetSaves();
		//pd.playerIdx = player.GetPlayerID();
		pd.teamNum = player.GetTeamIndex();
		playerData.arr.push_back(pd);
	}
	return toJson(playerData, "players");
}

string getCarData(GameWrapper * gw)
{
	ServerWrapper gew = gw->GetGameEventAsServer2();
	auto players = gew.GetPRIs();
	ArrayModel<CarData> carData;
	for (unsigned int i = 0; i < players.Count(); i++) 
	{
		CarData cd;
		auto player = players.Get(i);
		auto car = player.GetCar();
		if (player.IsSpectator() || car.IsNull())
			continue;

		cd.FromWrapper(car);
		cd.playerIdx = i;
		carData.arr.push_back(cd);
	}
	return toJson(carData, "cardata");
}

