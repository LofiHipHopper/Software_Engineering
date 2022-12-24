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
#include "inquiryservice.hpp"
#include "riskservice.hpp"
#include "Generators.h"
#include "BondHistoricalDataService.h"
#include "BondExecutionService.h"
#include "BondAlgoExecutionService.h"
#include "BondPricingService.h"
#include "BondRiskService.h"



/*

	Allows to manage positions in different books

*/



using namespace std;

//Define position service and listener

class BondPositionServiceListener : public ServiceListener<Position<Bond>>
{
public:

	BondRiskService* service;

	BondPositionServiceListener() : service(BondRiskService::Add()) {};
	
	static BondPositionServiceListener* Add()
	{
		static BondPositionServiceListener example;
		return &example;
	}


	virtual void ProcessAdd(Position<Bond>& data) override
	{
		(*service).AddPosition(data);
	}


	virtual void ProcessRemove(Position<Bond>& data) override {};
	virtual void ProcessUpdate(Position<Bond>& data) override {};

	



};



class BondPositionService : public PositionService<Bond>
{
public:

	
	BondPositionService()
	{
		AddListener((BondPositionServiceListener::Add()));
	}

	static BondPositionService* Add()
	{
		static BondPositionService serv;
		return &serv;
	}

	virtual void OnMessage(Position<Bond>& data) override
	{
		for (auto listener : listeners)
		{
			(*listener).ProcessAdd(data);
		}
	}

	virtual void AddTrade(const Trade<Bond>& trade) {};



};
