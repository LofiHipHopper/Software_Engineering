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
#include "Generators.h"
#include "BondAlgoExecutionService.h"
#include "BondHistoricalDataService.h"

/*

	Allows to collect market data for historical records (txt files) and for algo execution (through listeners)

*/

using namespace std;


//Define bond market data service and connector


class BondMarketDataService : public MarketDataService<Bond>
{


public:

	BondMarketDataService()
	{
		AddListener((BondAlgoExecutionListener::Add()));
		AddListener((BondHistoricalDataListener::Add()));
	};


	static BondMarketDataService* Add()
	{
		static BondMarketDataService serv;
		return &serv;
	}

	virtual void OnMessage(OrderBook<Bond>& bond)
	{
		for (auto listener : listeners)
		{
			(*listener).ProcessUpdate(bond);
		}
	}

	//Override to be able to instantiate an object
//	virtual const BidOffer& GetBestBidOffer(const string& Id) {};
//	virtual const OrderBook<Bond>& AggregateDepth(const string& Id) {};
//	virtual OrderBook<Bond>& GetData(string key) {};

	//virtual void AddListener(ServiceListener<OrderBook<Bond>>* listener) { listeners.push_back(&listener); };
	//virtual const vector< ServiceListener<OrderBook<Bond>>* >& GetListeners() const {};

	
};

class BondMarketDataConnector : public Connector<OrderBook<Bond>>
{

public:

	BondMarketDataService* service;


	BondMarketDataConnector() : service(BondMarketDataService::Add()) {};

	static BondMarketDataConnector* Add()
	{
		static BondMarketDataConnector connector;
		return &connector;
	}

	virtual void Publish(OrderBook<Bond>& book) {};

	void ReadData()
	{

		cout << "Market data read" << endl;

		string cur_row;
		ifstream file("marketdata.txt");

		vector<Order> oVec;
		vector<Order> bVec;

		getline(file, cur_row);
		while (getline(file, cur_row))
		{
			string code;
			double oPrice;
			int oVolume;

			double bPrice;
			int bVolume;

			auto cur_row_vec = read(cur_row);
			
			code = cur_row_vec[0];

			for (unsigned int i = 1; i < 10; i += 2)
			{
				bPrice = stod(cur_row_vec[i]);
				bVolume = stoi(cur_row_vec[i+1]);
				bVec.push_back(Order(bPrice, bVolume, BID));

			}

			for (unsigned int i = 11; i < 20; i += 2)
			{
				oPrice = stod(cur_row_vec[i]);
				oVolume = stoi(cur_row_vec[i + 1]);
				oVec.push_back(Order(bPrice, bVolume, OFFER));
			}
			

			OrderBook<Bond> OrderBook(bondMap[code], bVec, oVec);

			(*service).OnMessage(OrderBook);
	
		}

		cout << "End of market data read" << endl;

	}

};