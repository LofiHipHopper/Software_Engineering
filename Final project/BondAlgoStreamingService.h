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
#include "Generators.h"
#include "BondHistoricalDataService.h"
#include "BondExecutionService.h"
#include "BondAlgoExecutionService.h"
#include "BondPricingService.h"

/*

	Allows to create streaming file

*/

using namespace std;

//Define streaming service

class BondAlgoStreamingService : public StreamingService<Bond>
{
public:

	BondPricingService* service;

	BondAlgoStreamingService() : service(BondPricingService::Add()) {};
	
	static BondAlgoStreamingService* Add()
	{
		static BondAlgoStreamingService serv;
		return &serv;
	}


	void PublishPrice(PriceStream<Bond>& priceStream)
	{
		(*BondHistoricalDataService::Add()).Persist_Streaming(priceStream);
	}

	virtual void OnMessage(PriceStream<Bond>& stream) {};
	virtual PriceStream<Bond>& GetData(string code) {};

};

