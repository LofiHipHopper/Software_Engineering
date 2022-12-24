#pragma once

#include "string"
#include "iostream"
#include "fstream"
#include "sstream"
#include "random"
#include "vector"
#include "map"
#include "ctime"

#include "products.hpp"
#include "positionservice.hpp"


/*

	Allows to generate maps, data and gives global codes for bonds

*/



using namespace std;


//the instruments
string codes[7] = { "91282CFS5", "91282CGA3", "91282CFZ9", "91282CFY2", "91282CFV8", "912810TM0", "912810TL2" };
//{ "2Y", "3Y", "5Y", "7Y", "10Y", "20Y", "30Y" };
//maturities: {2024-10-31, 2025-12-15, 2027-11-30, 2029-11-30, 2032-11-15, 2042-11-15, 2052-11-15}
//coupons further used are random, the CUSIP codes might not correspond to suitable bond (e.g. it might be a code for a floating rate bond)




//n - number of observations to generate
//format is <code price spread>
void PriceGenerator(int n)
{
	ofstream file;

	file.open("prices.txt");

	for (unsigned int i = 0; i < 7; i++)
	{

		for (unsigned int j = 0; j < n; j++)
		{
			int p1_n = rand() % 2 + 99;
			int p2_n = rand() % 32;
			int p3_n = rand() % 8;

			string p1_s = to_string(p1_n);
			string p2_s = to_string(p2_n);
			string p3_s = to_string(p3_n);

			if (p2_n < 10)
			{
				p2_s = "0" + to_string(p2_n);
			}


			string pFinal = p1_s + "-" + p2_s + p3_s;




			int s_n = rand() % 3 + 2;
			string sFinal;

			sFinal = "0-00" + to_string(s_n);


			file << codes[i] << " " << pFinal << " " << sFinal << endl;



		}
	}

	file.close();

}


//format is <code book price volume direction>
void TradeGenerator()
{
	ofstream file;

	file.open("trades.txt");

	for (unsigned int i = 0; i < 7; i++)
	{
		for (unsigned int j = 0; j < 10; j++)
		{
			


			int p1_n = rand() % 2 + 99;
			int p2_n = rand() % 32;
			int p3_n = rand() % 8;

			string p1_s = to_string(p1_n);
			string p2_s = to_string(p2_n);
			string p3_s = to_string(p3_n);

			if (p2_n < 10)
			{
				p2_s = "0" + to_string(p2_n);
			}

			string pFinal = p1_s + "-" + p2_s + p3_s;



			string dir;

			if (rand() % 2 == 0)
			{
				dir = "BUY";
			}
			else
			{
				dir = "SELL";
			}


			file << codes[i]  << " " << rand()%1000 << " " << "TRSY" << rand() % 3 + 1 << " " << pFinal << " " << j % 5 * 100000 + 100000 << " " << dir << endl;


		}
	}

	file.close();

}



//n - number of observations to generate
//format is <code bid1 b_pos1 bid2 b_pos2 ... offer1 o_pos1 offer2 o_pos2 ...>
void MarketdataGenerator(int n)
{
	ofstream file;

	file.open("marketdata.txt");


	for (unsigned int i = 0; i < 7; i++)
	{
		
		double spread =  1 / 256.0; //spread from mid price

		for (unsigned int j = 0; j < n; j++)
		{
			file << codes[i] << " ";

			int p1_n = rand() % 2 + 99;
			int p2_n = rand() % 32;
			int p3_n = rand() % 8;


			double pFinal = p1_n + p2_n / 32.0 + p3_n / 256.0;

			

			for (unsigned int k = 1; k < 6; k++)
			{
				file << pFinal - spread - k/256.0 << " " << 100000 * k << " ";
			}

			for (unsigned int k = 1; k < 6; k++)
			{

				file << pFinal + spread + k / 256.0 << " " << 100000 * k << " ";

			}
			
			if (j % 6 < 3)
			{
				spread += 1 / 256.0;
			}
			else
			{
				spread -= 1 / 256.0;
			}


			file << endl;
			
		}
	}

	file.close();

}



//format is <code price volume direction state>
void InquiryGenerator()
{
	ofstream file;

	file.open("inquiries.txt");

	for (unsigned int i = 0; i < 7; i++)
	{

		for (unsigned int j = 0; j < 10; j++)
		{
			int p1_n = rand() % 3 + 99;

			string dir;

			if (rand() % 2 == 0)
			{
				dir = "BUY";
			}
			else
			{
				dir = "SELL";
			}


			file << codes[i] << " " << p1_n << " " << (rand() % 5 + 1) * 10000  << " " << dir << " " << "RECEIVED" << endl;



		}
	}

	file.close();

}

map<string, Bond> bondMap;
map<string, Position<Bond>> posMap;

void MapGenerator()
{

	Bond bonds[] = { Bond(codes[0], CUSIP, "ticker1",  0.001  , date(2024,10,31)),
		Bond(codes[1], CUSIP, "ticker2",  0.0015  , date(2025,12,15)),
		Bond(codes[2], CUSIP, "ticker3",  0.0011  , date(2027,11,30)),
		Bond(codes[3], CUSIP, "ticker4",  0.0019  , date(2029,11,30)),
		Bond(codes[4], CUSIP, "ticker5",  0.003  , date(2032,11,15)),
		Bond(codes[5], CUSIP, "ticker6",  0.002  , date(2042,11,15)),
		Bond(codes[6], CUSIP, "ticker7",  0.0005  , date(2052,11,15))
	};

	for (unsigned int i = 0; i < 7; i++)
	{
		bondMap.insert(pair<string, Bond>(codes[i], bonds[i]));
		Position<Bond> posB(bonds[i]);
		posMap.insert(pair<string, Position<Bond>>(codes[i], posB));
	}

}






vector<string> read(string line)
{
	stringstream ss(line);
	vector<string> result;
	string curr;

	while (getline(ss, curr, ' '))
	{
		result.push_back(curr);
	}

	return result;
}

double doublePrice(string price)
{

	stringstream ss(price);
	vector<string> numbers;
	string curr;


	while (getline(ss, curr, '-'))
	{
		numbers.push_back(curr);
	}


	
	if (int(numbers[1][0]) - '0' == 0)
	{
		return stod(numbers[0]) + double(numbers[1][1] - '0') / 32.0 + double(numbers[1][2] - '0') / 256.0;
	}
	else
	{
		return stod(numbers[0]) + (double(numbers[1][0] -'0') * 10 + double(numbers[1][1] - '0')) / 32.0 + double(numbers[1][2] - '0') / 256.0;
	}
	
	

}













