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

	Allows to calculate PV01 risk (no sigle security/batch risk implemented)

*/


using namespace std;


//Define risk service

class BondRiskService : public RiskService<Bond>
{


public:

	//Map format <code: pv01>
	map<string, double> riskMap;
	vector<Bond> bonds;

	BondRiskService() {};

	static BondRiskService* Add()
	{
		static BondRiskService serv;
		return &serv;
	}
	

	void AddPosition(Position<Bond>& pos)
	{

		int n = riskMap.size();
		
		//Could not find PV01's, used random
		if (n == 0)
		{
			riskMap["91282CFS5"] = 0.011;
			riskMap["91282CGA3"] = 0.025;
			riskMap["91282CFZ9"] = 0.013;
			riskMap["91282CFY2"] = 0.021;
			riskMap["91282CFV8"] = 0.016;
			riskMap["912810TM0"] = 0.028;
			riskMap["912810TL2"] = 0.017;
		}

		
		double pv01_risk = riskMap[pos.GetProduct().GetProductId()];
		
		long volume = pos.GetAggregatePosition();
		
		double total = volume * pv01_risk;
		bonds.push_back(pos.GetProduct());

		PV01<Bond> risk(bonds, total, volume);
		
		(*BondHistoricalDataService::Add()).Persist_Risk(risk);
		(*BondHistoricalDataService::Add()).Persist_Position();
	}

	


	const PV01<BucketedSector<Bond>>& GetBucketedRisk(const BucketedSector<Bond>& sector) const {};
	virtual void OnMessage(PV01<Bond>& data) override {};
};
