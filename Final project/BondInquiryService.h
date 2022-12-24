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

/*

	Allows to create inquiry file

*/

using namespace std;


//Define bond inquiry service and connector

class BondInquiryService : public InquiryService<Bond>
{
public:

	BondInquiryService() {};

	static BondInquiryService* Add()
	{
		static BondInquiryService serv;
		return &serv;
	}


	//Update data
	virtual void OnMessage(Inquiry<Bond>& data) override
	{
		data.State(InquiryState::QUOTED);

		(*BondHistoricalDataService::Add()).Persist_Inquiry(data);
	}

	void SendQuote(const string& inquiryId, double price) {};
	void RejectInquiry(const string& inquiryId) {};

	
};



class BondInquiryConnector : public Connector<OrderBook <Bond>>
{
public:

	BondInquiryService* service;

	BondInquiryConnector() : service(BondInquiryService::Add()) {};

	static BondInquiryConnector* Add()
	{
		static BondInquiryConnector connector;
		return &connector;
	}



	void ReadInquiryData()
	{
		cout << "Inquiry data read" << endl;

		//index for different inquiry id's
		static int inquiry_index = 1;
		ifstream file("inquiries.txt");

		string row;

		getline(file, row);
		while (getline(file, row))
		{
			string product_type, price, quantity, sSide, sState;

			vector<string> result = read(row);

			tie(product_type, price, quantity, sSide, sState) = make_tuple(result[0], result[1],
				result[2], result[3], result[4]);


			string sId = to_string(inquiry_index);


			Bond& bond = bondMap[product_type];
			Side side;


			if (sSide == "BUY")
			{
				side = BUY;
			}
			else
			{
				side = SELL;
			}


			long _quantity = stod(quantity);
			double _price = stod(price);

			InquiryState state;


			if (sState == "RECEIVED")
			{
				state = InquiryState::RECEIVED;
			}
			else
			{
				state = InquiryState::DONE;
			}


			Inquiry<Bond> inquiry_object(sId, bond, side, _quantity, _price, state);
			(*service).OnMessage(inquiry_object);
			inquiry_index++;
		}
		inquiry_index++;
		cout << "End of inquiry data read" << endl << endl;
	}

	virtual void Publish(OrderBook<Bond>& data) override {};




};
