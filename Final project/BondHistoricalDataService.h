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
#include "historicaldataservice.hpp"
#include "Generators.h"


/*

	Combines publishers of data

*/

using namespace std;

//Define historical data service with connectors and their publishing methods

class InquiryConnector : public Connector<string>
{
public:

	InquiryConnector() {};

	virtual void Publish(string& row) 
	{
		ofstream file("allinquiries.txt", ios_base::app);
		file << row << endl;
	}


	static InquiryConnector* Add()
	{
		static InquiryConnector connector;
		return &connector;
	}
};

class ExecutionConnector : public Connector<string>
{
public:

	ExecutionConnector() {};


	virtual void Publish(string& row) 
	{
		ofstream file("execution.txt", ios_base::app);
		file << row << endl;
	}

	static ExecutionConnector* Add()
	{
		static ExecutionConnector connector;
		return &connector;
	}
};

class RiskConnector : public Connector<string>
{
public:

	RiskConnector() {};

	virtual void Publish(string& row) 
	{
		ofstream file("risk.txt", ios_base::app);
		file << row << endl;
	}

	static RiskConnector* Add()
	{
		static RiskConnector connector;
		return &connector;
	}
};

class StreamingConnector : public Connector<string>
{
public:

	StreamingConnector() {};


	virtual void Publish(string& row) 
	{
		ofstream file("streaming.txt", ios_base::app);
		file << row << endl;
	}

	static StreamingConnector* Add()
	{
		static StreamingConnector connector;
		return &connector;
	}
};

class PositionConnector : public Connector<string>
{
public:

	PositionConnector() {};


	virtual void Publish(string& row)
	{
		ofstream file("positions.txt");
		
		map<string, Position<Bond>>::iterator it;

		for (it = posMap.begin(); it != posMap.end(); it++)
		{
			for (unsigned int j = 1; j < 4; j++)
			{
				string book = "TRSY" + to_string(j);
				file << it->first << ": " << book << " " << it->second.GetPosition(book) << std::endl;
			}
			
		}
		
		//file << endl;
	}

	static PositionConnector* Add()
	{
		static PositionConnector connector;
		return &connector;
	}
};





class BondHistoricalDataService : public HistoricalDataService<Bond>
{
public:

	BondHistoricalDataService() : iconnector(InquiryConnector::Add()), econnector(ExecutionConnector::Add()), rconnector(RiskConnector::Add()), sconnector(StreamingConnector::Add()), pconnector(PositionConnector::Add()) {};

	static BondHistoricalDataService* Add()
	{
		static BondHistoricalDataService serv;
		return &serv;
	}



	InquiryConnector* iconnector;
	ExecutionConnector* econnector;
	RiskConnector* rconnector;
	StreamingConnector* sconnector;
	PositionConnector* pconnector;

	virtual void OnMessage(Bond& data) {};


	//Format: <code: book position> 
	void Persist_Position() 
	{
		cout << "Persisting Position data" << endl;

		string s = "";

		(*pconnector).Publish(s);
	};

	//Format: <PV01 risk> 
	void Persist_Risk(PV01<Bond>& risk)
	{
		cout << "Persisting Risk data" << endl;
		string pv01_data = to_string(risk.GetPV01());
		string input = string("PV01") + string(" ") + pv01_data;

		(*rconnector).Publish(input);
	}

	
	void Persist_Execution(string row)
	{
		cout << "Persisting Execution" << endl;

		(*econnector).Publish(row);
	}


	//Format: <code bid offer> 
	void Persist_Streaming(PriceStream<Bond>& stream)
	{
		cout << "Streaming" << endl;

		string productId = stream.GetProduct().GetProductId();
		string oPrice = to_string(stream.GetOfferOrder().GetPrice());
		string bPrice = to_string(stream.GetBidOrder().GetPrice());
		string input = productId + " " + bPrice + " " + oPrice;

		

		(*sconnector).Publish(input);
	}


	//Format: <productId inquiryId price quantity> 
	void Persist_Inquiry(Inquiry<Bond>& data)
	{

		cout << "Persisting Inquiry" << endl;

		string price = to_string(data.GetPrice());
		string volume = to_string(data.GetQuantity());
		string iId = data.GetInquiryId();
		string pId = data.GetProduct().GetProductId();
		

		string side;

		if (data.GetSide() == Side::BUY)
		{
			side = "BUY";
		}
		else
		{
			side = "SELL";
		}

		string state;

		if (data.GetState() == InquiryState::RECEIVED)
		{
			state = "RECEIVED";
		}
		else
		{
			state = "OTHERS";
		}
		string input = pId + " " + iId + " " + price + " " + volume;
		(*iconnector).Publish(input);
	}



	void PersistData(string persistKey, const Bond& data) {};
};


class BondHistoricalDataListener : public ServiceListener<OrderBook<Bond>>
{
public:

	BondHistoricalDataService* service;

	BondHistoricalDataListener() : service(BondHistoricalDataService::Add()) {};

	static BondHistoricalDataListener* Add()
	{
		static BondHistoricalDataListener listener;
		return &listener;
	}

	virtual void ProcessAdd(OrderBook<Bond>& data) {};
	virtual void ProcessRemove(OrderBook<Bond>& data) {};
	virtual void ProcessUpdate(OrderBook<Bond>& data) {};

	
};
