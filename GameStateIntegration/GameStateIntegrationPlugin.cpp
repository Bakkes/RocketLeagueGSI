#include "GameStateIntegrationPlugin.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "wrappers/carwrapper.h"
#include <iostream>
#include <string>
#include <mutex>
#include "Commands.h"
#include <utility>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;
typedef server::connection_ptr connection_ptr;

BAKKESMOD_PLUGIN(GameStateIntegrationPlugin, "GameStateIntegration plugin", "0.1", -1337)
//extern void run_server();
GameWrapper* gw;
ConsoleWrapper* cons;
server* ws_server;

struct ConnectionData {
	vector<std::string> awaitingCommands;
	std::mutex mtx;
	connection_ptr con;
};

std::map<std::string, t_getData>* availableCommands;
std::map<connection_ptr, ConnectionData*>* connections;
std::map<std::string, vector<connection_ptr>*>* subscriptions;

void cb(ActorWrapper aw, std::string e) 
{
	cons->log(e);
}

void checkCommands(GameWrapper* gameWrapper) 
{
	for (auto connectionIt = connections->begin(); connectionIt != connections->end(); connectionIt++) 
	{
		connectionIt->second->mtx.lock();
		for (auto it = connectionIt->second->awaitingCommands.begin(); it != connectionIt->second->awaitingCommands.end(); it++)
		{
			auto mapIt = availableCommands->find(*it);
			if (mapIt != availableCommands->end())
			{
				string resultJson = mapIt->second(gameWrapper);
				if (!resultJson.empty()) 
				{
					connectionIt->first->send(resultJson);
				}
			}
		}
		connectionIt->second->awaitingCommands.clear();
		connectionIt->second->mtx.unlock();
	}

	for (auto subscriptionIt = subscriptions->begin(); subscriptionIt != subscriptions->end(); subscriptionIt++)
	{
		if (subscriptionIt->first.size() == 0)
			continue;
		auto mapIt = availableCommands->find(subscriptionIt->first);
		if (mapIt != availableCommands->end())
		{
			string resultJson = mapIt->second(gameWrapper);
			for (auto subscriberIt = subscriptionIt->second->begin(); subscriberIt != subscriptionIt->second->end(); subscriberIt++) {
				(*subscriberIt)->send(resultJson);
			}
			
		}
	}
	gw->SetTimeout(&checkCommands, 50);
}


void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	connection_ptr con = s->get_con_from_hdl(hdl);
	if (connections->find(con) == connections->end()) 
	{
		ConnectionData* conData = new ConnectionData();
		conData->con = con;
		connections->insert(std::pair<connection_ptr, ConnectionData*>(con, conData));
	}
	try {
		std::string payload = msg->get_payload();
		ConnectionData* conData = connections->at(con);
		if (payload.find("subscribe ") == 0) {
			string subscribeTo = payload.substr(10);
			if (subscriptions->find(subscribeTo) == subscriptions->end()) 
			{
				subscriptions->insert(std::pair<std::string, vector<connection_ptr>*>(subscribeTo, new vector<connection_ptr>()));
			}
			subscriptions->at(subscribeTo)->push_back(conData->con);
		} else if (payload.find("unsubscribe ") == 0) {
			string unsubscribeTo = payload.substr(12);
			vector<connection_ptr>* subscribz = subscriptions->at(unsubscribeTo);
			auto item = std::find(subscribz->begin(), subscribz->end(), conData->con);
			if (item != subscribz->end())
			{
				subscriptions->at(unsubscribeTo)->erase(item);
			}
		}
		else {
			conData->mtx.lock();
			conData->awaitingCommands.push_back(payload);
			conData->mtx.unlock();
		}
		//auto input = parseConsoleInput(msg->get_payload());
		//
		//string payload = msg->get_payload();
		//gw->Execute([payload](GameWrapper* gw) {
		//	cons->executeCommand(payload);
		//});

		//delete input;
	}
	catch (const websocketpp::lib::error_code& e) {
		std::cout << "Echo failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
	}
}

void run_server() {
	try {
		// Set logging settings
		//ws_server.set_access_channels(websocketpp::log::alevel::all);
		//ws_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize Asio
		ws_server->init_asio();

		// Register our message handler
		ws_server->set_message_handler(bind(&on_message, ws_server, ::_1, ::_2));

		// Listen on port
		ws_server->listen(8765);

		// Start the server accept loop
		ws_server->start_accept();

		// Start the ASIO io_service run loop
		ws_server->run();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
	}
}

void onListenAdd(std::vector<std::string> params)
{
	std::string command = params.at(0);
	if (command.compare("gsi_start") == 0)
	{
		
	}
	else if (command.compare("gsi_debug_car") == 0) 
	{
	}
}



void GameStateIntegrationPlugin::onLoad()
{
	gw = gameWrapper;
	cons = console;
	cons->registerNotifier("gsi_start", onListenAdd);
	cons->registerNotifier("gsi_debug_car", onListenAdd);
	cons->registerCvar("gsi_interval", "50");

	availableCommands = new std::map<std::string, t_getData>();
	connections = new std::map<connection_ptr, ConnectionData*>();
	subscriptions = new std::map<std::string, vector<connection_ptr>*>();

	availableCommands->insert(std::pair<std::string, t_getData>("players", &getPlayers));
	availableCommands->insert(std::pair<std::string, t_getData>("cardata", &getCarData));
	gw->SetTimeout(&checkCommands, 50);

	if (ws_server == NULL)
	{
		ws_server = new server();
	}
	else
	{
		ws_server->stop();
	}
	run_server();
	//DONT PUT CODE HERE AS IT'LL NEVER GET EXECUTED
}

void GameStateIntegrationPlugin::onUnload()
{
	delete availableCommands;
	delete connections;
	delete subscriptions;
	if (ws_server != NULL) 
	{
		ws_server->stop();
	}
}
