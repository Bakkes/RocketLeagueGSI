#include "GameStateIntegrationPlugin.h"
BAKKESMOD_PLUGIN(GameStateIntegrationPlugin, "GameStateIntegration plugin", "0.1", -1337)

GameWrapper* gw;
ConsoleWrapper* cons;
void cb(ActorWrapper aw, string e) 
{
	cons->log(e);
}
void onListenAdd(std::vector<std::string> params)
{
	string command = params.at(0);
	if (command.compare("gsi_listen_player") == 0)
	{
		
	}
}

bool wentUpAgain = true;

void test(GameWrapper* gw) {
	if (!gw->IsInTutorial() && !gw->IsInCustomTraining() && gw->GetGameEvent().IsNull() && gw->GetGameEvent().GetBall().IsNull()) {
		gw->SetTimeout(test, 100);
		return;
	}
	BallWrapper b = gw->GetGameEvent().GetBall();
	if (b.GetLocation().Z < 95) {
		if (wentUpAgain) {
			cons->executeCommand("debug ball");
			wentUpAgain = false;
		}
	}
	else {
		wentUpAgain = true;
	}
	gw->SetTimeout(test, 1);
}

void GameStateIntegrationPlugin::onLoad()
{
	gw = gameWrapper;
	cons = console;
	cons->registerNotifier("gsi_listen_player", onListenAdd);
	cons->registerCvar("gsi_ip", "127.0.0.1");
	cons->registerCvar("gsi_port", "43594");
	gw->SetTimeout(test, 1);
}

void GameStateIntegrationPlugin::onUnload()
{
}
