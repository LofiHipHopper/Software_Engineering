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
#include "pricingservice.hpp"
#include "Generators.h"
#include "BondHistoricalDataService.h"
#include "BondExecutionService.h"
#include "BondAlgoExecutionService.h"
#include "BondPricingService.h"
#include "BondRiskService.h"
#include "BondStreamingService.h"
#include "BondPositionService.h"


/*

	Allows to read trade data

*/

using namespace std;


//Define trading service, listener and connector

class BondTradingServiceListener : public ServiceListener<Trade<Bond>>
{
public:

	BondPositionService* service;

	BondTradingServiceListener() : service(BondPositionService::Add()) {};

	static BondTradingServiceListener* Add()
	{
		static BondTradingServiceListener example;
		return &example;
	}

	void ProcessAdd(Trade<Bond>& tradedata) override
	{
	
		string product_type = tradedata.GetProduct().GetProductId();

		long quantity = ((tradedata.GetSide() == BUY) ? tradedata.GetQuantity() : -tradedata.GetQuantity());

		posMap[product_type].AddPosition(tradedata.GetBook(), quantity);
		(*service).OnMessage(posMap[product_type]);
	}

	void ProcessRemove(Trade<Bond>& data) override {};
	void ProcessUpdate(Trade<Bond>& data) override {};





};


class BondTradingService : public Service<string, Trade<Bond>>
{
public:

	BondTradingService()
	{
		AddListener((BondTradingServiceListener::Add()));
	}

	void booktrade(Trade<Bond>& tradedata)
	{
		for (auto listener : listeners)
		{
			(*listener).ProcessAdd(tradedata);
		}
	}

	void OnMessage(Trade<Bond>& tradedata) override
	{
		booktrade(tradedata);
	}

	static BondTradingService* Add()
	{
		static BondTradingService serv;
		return &serv;
	}



};


class BondTradingServiceConnector : public Connector<Trade<Bond>>
{
public:

	BondTradingService* service;

	BondTradingServiceConnector() : service(BondTradingService::Add()) {};


	static BondTradingServiceConnector* Add()
	{
		static BondTradingServiceConnector example;
		return &example;
	}


	void ReadTradeData()
	{
		cout << "Trade data read" << endl;
		
		ifstream file("trades.txt");
		
		string code, Id, book, volume, side, row;

		double price;


		getline(file, row);
		while (getline(file, row))
		{
			
			vector<string> data = read(row);

			tie(code, Id, book, price, volume, side)
				= make_tuple(data[0], data[1], data[2], doublePrice(data[3]), data[4], data[5]);


			
			Bond& bond = bondMap[code];
			Side side_result;

			if (side == "BUY")
			{
				side_result = BUY;
			}
			else
			{
				side_result = SELL;
			}

			Trade<Bond> trade(bond, Id, price, book, stol(volume), side_result);

			(*service).OnMessage(trade);
		}
		cout << "End of trade data read" << endl << endl;
	}

	void Publish(Trade<Bond>& data) override {};


};
