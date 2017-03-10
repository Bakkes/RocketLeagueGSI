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
	float boostAmount;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		ActorData::Serialize(writer);
		writer.String("boostamount");
		writer.Double(boostAmount);
	}

	void FromWrapper(CarWrapper cw);
};

class PlayerData : public JsonModel {
public:
	int playerIdx;
	string playerName;

	template <typename Writer>
	void Serialize(Writer& writer)
	{
		ActorData::Serialize(writer);
		writer.BeginObject();
		writer.String("playerindex");
		writer.Int(playerIdx);
		writer.String("name");
		writer.String(playerName);
		writer.EndObject();
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
