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



/*

	Allows to create the streaming file

*/


using namespace std;


//Define streaming service

class BondStreamingService : public Service<string, PriceStream<Bond>>
{

public:

	BondStreamingService() {};


	static BondStreamingService* Add()
	{
		static BondStreamingService serv;
		return &serv;
	}

	
	void PublishPrice(PriceStream<Bond>& priceStream)
	{
		(*BondHistoricalDataService::Add()).Persist_Streaming(priceStream);
	}

	virtual void OnMessage(PriceStream<Bond>& data) override {};




};
