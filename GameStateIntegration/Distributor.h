#pragma once
#include <vector>
#include "Listener.h"

using namespace std;
class Distributor
{
public:
	vector<int> listeners;
	string ip;
	int port;
	Distributor(string ipAddr, int portNo);
	~Distributor();
	virtual void Distribute();
};

