#include "GameStateIntegrationPlugin.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "wrappers/carwrapper.h"
#include <iostream>
#include <string>
#include <mutex>

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
vector<std::string> awaitingCommands;
std::mutex mtx;

void cb(ActorWrapper aw, std::string e) 
{
	cons->log(e);
}

std::vector<connection_ptr> conns;


void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	connection_ptr con = s->get_con_from_hdl(hdl);
	if (std::find(conns.begin(), conns.end(), con) == conns.end())
		conns.push_back(con);
	try {
		std::string payload = msg->get_payload();
		mtx.lock();
		awaitingCommands.push_back(payload);
		mtx.unlock();
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
		auto car = gw->GetLocalCar();
		cons->log("Sending car data to " + to_string(conns.size()) + " connections");
		for (unsigned int i = 0; i < conns.size(); i++) {

			StringBuffer buffer;
			Writer<StringBuffer> writer(buffer);

			CarData c;
			c.FromWrapper(car);
			writer.StartObject();
			c.Serialize(writer);
			writer.EndObject();
			conns.at(i)->send(buffer.GetString());
		}
	}
}

void GameStateIntegrationPlugin::onLoad()
{
	gw = gameWrapper;
	cons = console;
	cons->registerNotifier("gsi_start", onListenAdd);
	cons->registerNotifier("gsi_debug_car", onListenAdd);
	cons->registerCvar("gsi_interval", "50");

	if (ws_server == NULL)
	{
		ws_server = new server();
	}
	else
	{
		ws_server->stop();
	}
	run_server();
}

void GameStateIntegrationPlugin::onUnload()
{
	if (ws_server != NULL) 
	{
		ws_server->stop();
	}
}
