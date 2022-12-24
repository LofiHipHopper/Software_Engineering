#pragma once
#include "string"
#include "iostream"
#include "fstream"
#include "random"
#include "Generators.h"
#include "vector"

#include "soa.hpp"
#include "products.hpp"
#include "marketdataservice.hpp"
#include "streamingservice.hpp"
#include "executionservice.hpp"
#include "Generators.h"
#include "BondHistoricalDataService.h"

/*

	Allows to execute to the execution file

*/

using namespace std;


//Define execution service
//Format in execution.txt: <code price>

class BondExecutionService : public ExecutionService<Bond>
{

public:


	BondHistoricalDataService* service;

	BondExecutionService() {};

	static BondExecutionService* Add()
	{
		static BondExecutionService serv;
		return &serv;
	};

	void ExecuteOrder(const ExecutionOrder<Bond>& order, Market market)
	{
		string code = order.GetProduct().GetProductId();

		service = BondHistoricalDataService::Add();

		string row = code + " " + to_string(order.GetPrice());

		(*service).Persist_Execution(row);
	}


	virtual void OnMessage(ExecutionOrder<Bond>& data) {};

};