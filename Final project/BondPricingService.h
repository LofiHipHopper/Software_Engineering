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
#include "GuiService.h"



/*

	Allows to collect pricing data

*/



using namespace std;


//Define pricing data service and connector

class BondPricingService : public PricingService<Bond>
{
public:

	BondPricingService() {};

	static BondPricingService* Add()
	{
		static BondPricingService serv;
		return &serv;
	}

	virtual void OnMessage(Price<Bond>& data) override
	{
		
		double spread = data.GetBidOfferSpread();
		
		double price = data.GetMid();

		double bPrice = price - spread / 2;
		double oPrice = price + spread / 2;
		

		PriceStreamOrder Offer(oPrice, 0, 0, PricingSide::OFFER);
		PriceStreamOrder Bid(bPrice, 0, 0, PricingSide::BID);

		PriceStream<Bond> price_stream(data.GetProduct(), Bid, Offer);

		(*BondStreamingService::Add()).PublishPrice(price_stream);
	}

	


	
};


class BondPricingConnector : public Connector<Price<Bond>>
{
public:

	BondPricingService* service;

	GuiService* gui;

	BondPricingConnector() : service(BondPricingService::Add()) {};

	static BondPricingConnector* Add()
	{
		static BondPricingConnector connector;
		return &connector;
	}

	virtual void Publish(Price<Bond>& data) override {};


	void ReadPriceData()
	{
		cout << "Pricing data read" << endl;

		ifstream file("prices.txt");
		string product_type, mid_price, bid_offer_spread, row;
		getline(file, row);
		while (getline(file, row))
		{
	
			vector<string> data = read(row);

			
			tie(product_type, mid_price, bid_offer_spread) = make_tuple(data[0], data[1], data[2]);

			 
			double mid = doublePrice(mid_price);
			double spread = doublePrice(bid_offer_spread);

			
			Bond& bond = bondMap[product_type];

			Price<Bond> bond_price_instance(bond, mid, spread);

			(*service).OnMessage(bond_price_instance);
			(*GuiService::Add()).OnMessage(bond_price_instance);
		}
		cout << "End of pricing data read" << endl << endl;
	}





};

