#pragma once
#include "string"
#include "iostream"
#include "fstream"
#include "random"
#include "Generators.h"
#include "vector"
#include <boost/thread/thread.hpp>
#include <chrono>
#include <ctime>  

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
#include "BondPricingService.h"



/*

	Allows to create the gui file and to print out data 

*/


using namespace std;


//Define Gui service

class GuiService : public PricingService<Bond>
{
public:

	BondStreamingService* service;

	GuiService() : service(BondStreamingService::Add()) {};

	static GuiService* Add()
	{
		static GuiService serv;
		return &serv;
	}

	virtual void OnMessage(Price<Bond>& data) override
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(300));
		(*GuiService::Add()).PublishPrice(data);
	}

	//Format <time code price spread>
	void PublishPrice(Price<Bond>& price)
	{
		cout << "Gui working:" << endl;

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());

		int time = (ms.count());


		string input = to_string(time) + " " + (price.GetProduct().GetProductId()) + " " + to_string(price.GetMid()) + " " + to_string(price.GetBidOfferSpread());

		ofstream file("gui.txt", ios_base::app);
		cout << input << endl;
		file << input << endl;
	}




};