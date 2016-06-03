#include <iostream>
#include <thread>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

#include "Point.h"

using namespace std;

static vector<Point> points = vector<Point>(); 
 
// Get number of supported threads
auto num_threads = thread::hardware_concurrency();

string Trim(const string& str)
{
	const string& whitespace = " \t";
	const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

static void load(string name)
{ 
	try
	{ 
		ifstream ips (name);
		string line; 

		//skip header 
		getline(ips, line); 
		while (line != ("NODE_COORD_SECTION"))
		{ 
			getline(ips, line); 
		} 
			
		getline(ips, line);
			
		while (line != ("EOF"))
		{ 
			Point p = Point(); 
			p.x = atoi(Trim(line.substr(9, 14)).c_str()); 
			p.y = atoi(Trim(line.substr(15, 21)).c_str()); 
			//cout << line << " " << p.GetPointText() << endl;	// Output current position being read.
 
			points.push_back(p); 
			getline(ips, line);
 
		} 
		cout << "File loading complete" << endl;
		ips.close();
	} 
	catch (exception ex)
	{ 
		cout << "EXCEPTION OCCURRED";
	} 
} 
	
static double getDist(int city1, int city2)
{
	Point p1 = (Point) points[(city1)];
	Point p2 = (Point) points[(city2)]; 
	return Point::distance(p1.x, p1.y, p2.x, p2.y);
}

double fitness(vector<int> chromo)
{
	double distanceRequired = 0.0;
	// TODO: Optimise this by using multi-threading
#pragma omp parallel for num_threads(num_threads) reduction(+:distanceRequired)
	for(int i = 0; i < points.size() - 1; i++)
	{
		distanceRequired += getDist(chromo[i], chromo[i+1]); 
	}

	return distanceRequired;
}

vector<int> createRandomChromo()
{
	vector<int> chromo (points.size()); 
	//TODO add in random assignment code here

	for(int i = 0; i < 14051; i++)
	{
		chromo[i] = i;
	}
	return chromo;
}

vector<int> mutate(vector<int> chromo)
{
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::default_random_engine e(millis.count());
	std::uniform_int_distribution<int> distribution(0, points.size());
	int val = distribution(e);
	int val2 = distribution(e);
	
	while(val2 == val  || val < 0  || val2 < 0  || val >= points.size() || val2 >= points.size())	// Ensures mutation is swapping to a different location
	{ 
		val = distribution(e); 
		val2 = distribution(e); 
	}

	int x = chromo[val];
	int y = chromo[val2];
	chromo[val] = y;
	chromo[val2] = x;

	return chromo;
}

double hillClimbFitness()
{
	int tries = 0; 
	vector<int> chromo = createRandomChromo();

	double myFit = fitness(chromo);

	cout << "My original fitness is : " << myFit << endl;

	while(tries < 1000000) 
	{ 
		vector<int> newChromo = chromo;
		newChromo = mutate(newChromo);
		double newFit = fitness(newChromo);
	
		if (newFit < myFit) 
		{ 
			//Mutation has improved solution 
			chromo = newChromo; 
			myFit = newFit; 
			cout << "Improved - fitness is now " << myFit << endl ;
		} 
		tries++;
	} 

	cout << "Done - final fitness is " <<  myFit << endl;

	return myFit;
}

int main()
{
	load("brd14051.tsp");

	vector<int> chromo(points.size());

	for(int i = 0; i < 14051; i++)
	{
		chromo[i] = i;
	}
	
	double distance = fitness(chromo);

	hillClimbFitness();

	return 0;
}