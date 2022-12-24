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
#include "BondHistoricalDataService.h"
#include "BondExecutionService.h"
#include "BondAlgoExecutionService.h"

/*

	Allows to execute when spread is low

*/






//Varying position direction for execution
int PosDirection = 1;

using namespace std;


//Define execution service and execution listener 

class BondAlgoExecutionService : public BondExecutionService
{
public:

	BondExecutionService* service;

	BondAlgoExecutionService() : service(BondExecutionService::Add()) {};

	static BondAlgoExecutionService* Add() { static BondAlgoExecutionService service; return &service; }

	virtual void OnMessage(OrderBook<Bond>& book)
	{


		Bond bond = book.GetProduct();
		string code;

		double bPrice = book.GetBidStack()[0].GetPrice();
		double oPrice = book.GetOfferStack()[0].GetPrice();
		int volume;

		double spread = oPrice - bPrice;


		if (PosDirection % 2 == 0 and spread < 1 / 127.0)
		{
			const Order& bidOrder = book.GetBidStack()[0];

			volume = bidOrder.GetQuantity();

			//Use market order to "cross" the spread
			ExecutionOrder<Bond> order(bond, BID, to_string(PosDirection), MARKET, bPrice, volume, 0.0, " ", false);

			(*service).ExecuteOrder(order, BROKERTEC);

			//Varying trade direction
			PosDirection += 1;

		}
		if (PosDirection % 2 == 1 and spread < 1 / 127.0)
		{
			const Order& offerOrder = book.GetOfferStack()[0];

			volume = offerOrder.GetQuantity();

			ExecutionOrder<Bond> order(bond, BID, to_string(PosDirection), MARKET, oPrice, volume, 0.0, " ", false);

			(*service).ExecuteOrder(order, BROKERTEC);

			//Varying trade direction
			PosDirection += 1;

		}



	}

};



class BondAlgoExecutionListener : public ServiceListener<OrderBook<Bond>>
{

public:
	
	BondAlgoExecutionService* service;

	BondAlgoExecutionListener() : service(BondAlgoExecutionService::Add()) {};

	static BondAlgoExecutionListener* Add() { static BondAlgoExecutionListener listener; return &listener; }

	virtual void ProcessAdd(OrderBook<Bond>& book) {};
	virtual void ProcessRemove(OrderBook<Bond>& book) {};
	virtual void ProcessUpdate(OrderBook<Bond>& book) { (*service).OnMessage(book); };


};