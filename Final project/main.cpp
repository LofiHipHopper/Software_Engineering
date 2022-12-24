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
#include "BondTradingService.h"
#include "BondMarketDataService.h"
#include "BondInquiryService.h"

using namespace std;

/*

	To emulate the trading system structure pointers were used
	Static Add() "constructors" allow to connect listeners with data sources
	In the end of function executions OnMessage is called so that listeners are run

	To further advance the implementation, many things can be done in parallel using threads

	Note: signicant pieces of code were taken from StackExchange and github
*/

void main()
{

	//data length to vary
	int n = 10;

	PriceGenerator(n);
	TradeGenerator();
	MarketdataGenerator(n);
	InquiryGenerator();

	MapGenerator();

	(*BondTradingServiceConnector::Add()).ReadTradeData();
	(*BondMarketDataConnector::Add()).ReadData();
	(*BondPricingConnector::Add()).ReadPriceData();
	(*BondInquiryConnector::Add()).ReadInquiryData();


}