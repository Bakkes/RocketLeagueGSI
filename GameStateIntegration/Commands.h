#pragma once
#include <string>
#include "wrappers/gamewrapper.h"
#include "Models.h"
using namespace std;
typedef string(*t_getData)(GameWrapper* gw);

string getPlayers(GameWrapper* gw);
string getCarData(GameWrapper* gw);

template<typename mod>
string toJson(mod jsm, string key = "") {
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	writer.SetMaxDecimalPlaces(3);
	writer.StartObject();
	if (!key.empty())
		writer.String(key.c_str());
	jsm.Serialize(writer);
	writer.EndObject();
	return buffer.GetString();
}

template<typename mod>
string toJson(vector<mod> jsms, string key = "") 
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	writer.SetMaxDecimalPlaces(3);
	writer.StartObject();
	if (!key.empty()) 
	{
		writer.String(key.c_str());
		writer.StartObject();
	}
	writer.StartArray();
	for (unsigned int i = 0; i < jsms.size(); i++)
	{
		jsms.at(i).Serialize(writer);
	}
	writer.EndArray();
	if (!key.empty())
	{
		writer.EndObject();
	}
	writer.EndObject();
	return buffer.GetString();
}
