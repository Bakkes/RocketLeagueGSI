#pragma once
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <iostream>
#include "wrappers/actorwrapper.h"
#include "wrappers/carwrapper.h"
using namespace rapidjson;

class JsonModel {
public:
	template <typename Writer>
	void Serialize(Writer& writer) {};
};

class VectorData : public JsonModel {
public:
	Vector data;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		writer.StartObject();
		writer.String("x");
		writer.Double(data.X);
		writer.String("y");
		writer.Double(data.Y);
		writer.String("z");
		writer.Double(data.Z);
		writer.EndObject();
	}

	void FromWrapper(Vector v);
};

class RotatorData : public JsonModel {
public:
	Rotator data;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		writer.StartObject();
		writer.String("pitch");
		writer.Double(data.Pitch);
		writer.String("yaw");
		writer.Double(data.Yaw);
		writer.String("roll");
		writer.Double(data.Roll);
		writer.EndObject();
	}

	void FromWrapper(Rotator r);
};


class ActorData : public JsonModel {
public:
	VectorData location;
	VectorData velocity;
	RotatorData rotation;

	template <typename Writer>
	void Serialize(Writer& writer) 
	{
		writer.String("actordata");
		writer.StartObject();

		writer.String("location");
		location.Serialize(writer);

		writer.String("velocity");
		velocity.Serialize(writer);

		writer.String("rotation");
		rotation.Serialize(writer);
		
		writer.EndObject();
	}

	void FromWrapper(ActorWrapper cw);
};

class CarData : public ActorData {
public:
	float boostAmount = 0.0f;
	int playerIdx = 0;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		writer.StartObject();
		writer.String("playerindex");
		writer.Int(playerIdx);
		ActorData::Serialize(writer);
		writer.String("boostamount");
		writer.Double(boostAmount);
		writer.EndObject();
	}

	void FromWrapper(CarWrapper cw);
};

class PlayerData : public JsonModel {
public:
	int playerIdx = 0;
	string playerName = "null";
	int teamNum = 0;
	int goals = 0;
	int saves = 0;
	int assists = 0;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		//ActorData::Serialize(writer);
		writer.StartObject();
		writer.String("playerindex");
		writer.Int(playerIdx);
		writer.String("name");
		writer.String(playerName.c_str());
		writer.String("team");
		writer.Int(teamNum);
		writer.String("goals");
		writer.Int(goals);
		writer.String("saves");
		writer.Int(saves);
		writer.String("assists");
		writer.Int(assists);
		writer.EndObject();
	}
};

class BaseProperty : public JsonModel {
public:
	string propertyName = "";

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		//ActorData::Serialize(writer);
		writer.String(propertyName.c_str());
	}
};

class IntProperty : public BaseProperty {
public:
	int value = 0;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		BaseProperty::Serialize(writer);
		writer.Int(value);
	}
};

class StringProperty : public BaseProperty {
public:
	string value = 0;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		BaseProperty::Serialize(writer);
		writer.String(value);
	}
};

class DoubleProperty : public BaseProperty {
public:
	float value = 0;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		BaseProperty::Serialize(writer);
		writer.Double(value);
	}
};

class EventModel : public JsonModel {
public:
	string eventName = "";
	std::vector<BaseProperty> props;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		//ActorData::Serialize(writer);
		writer.String("event");
		writer.StartObject();
		writer.String("name");
		writer.String(eventName.c_str());
		writer.String("params");
		writer.StartObject();
		for (auto it = props.begin(); it != props.end(); it++) {
			it->Serialize(writer);
		}
		writer.EndObject();
		writer.EndObject();
	}
};

template <class A_Type>
class ArrayModel : public JsonModel 
{
public:
	vector<A_Type> arr;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		writer.StartArray();
		for (auto it = arr.begin(); it != arr.end(); it++) 
		{
			it->Serialize(writer);
		}
		writer.EndArray();
	}
};

//struct ActorData {
//	Vector location;
//	Vector velocity;
//	Rotator rotation;
//};


struct GoalScored {
	int scoredPlayer;
};
class hurr {
	GoalScored score;
};
