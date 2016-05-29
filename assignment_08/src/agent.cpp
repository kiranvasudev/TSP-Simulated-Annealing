/*
 * Daniel Vázquez, Kiran Vasudev, Patrick Nagel
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 7
 *
 * agent.cpp
 * */
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <agent.hpp>
#include <matplotlibcpp.h>
#include <math.h>
#include <sstream>
#include <random>

#include <chrono>

using namespace std;
using namespace std::chrono;

Agent::Agent(vector<City> cities) :
		cities(cities), shortestDistance(0), shortestRoute(), iteration(1) {

}

Agent::~Agent() {

}

void Agent::run() {
	//example
	//print_cities(cities);
	//save_cities_image(cities, 1);

	choose_solving_approach();
//	random_restart_hill_climb();
}

void Agent::print_cities(vector<City>& cities) {

	std::vector<double> x, y;
	for (int i = 0; i < cities.size(); i++) {
		x.push_back(cities[i].get_x());
		y.push_back(cities[i].get_y());
	}

	// Convert the shortestDistance into a string to print it as title of the graph.
	std::ostringstream strs;
	strs << shortestDistance;
	std::string distance = strs.str();

	// Plot line from given x and y data.
	matplotlibcpp::plot(x, y);
	matplotlibcpp::title(distance + "km");
	matplotlibcpp::show();
}

void Agent::save_cities_image(vector<City>& cities, int file_counter) {

	std::vector<double> x, y;
	for (int i = 0; i < cities.size(); i++) {
		x.push_back(cities[i].get_x());
		y.push_back(cities[i].get_y());
	}

	// Plot line from given x and y data.
	matplotlibcpp::plot(x, y);
	string file = "salesman_" + to_string(file_counter) + ".png";
	matplotlibcpp::save(file);
}

vector<City> Agent::hillClimb(vector<City> cities) {
	City start_city = cities[0];
	vector<City> journeyPath;

	journeyPath.push_back(cities[0]);

	double min;
	int minIndex;
	double totalDistance = 0;

	while (cities.size() != 1) {

		if (shortestDistance != 0 && totalDistance > shortestDistance) {
			return shortestRoute;
		}

		min = calculateDistance(cities[0].get_x(), cities[0].get_y(),
				cities[1].get_x(), cities[1].get_y());
		minIndex = 1;
		for (std::vector<City>::iterator it = cities.begin() + 2;
				it != cities.end(); ++it) {
			double distance = calculateDistance(cities[0].get_x(),
					cities[0].get_y(), (*it).get_x(), (*it).get_y());
			if (distance < min) {
				min = distance;
				minIndex = it - cities.begin();
			}
		}

		totalDistance += min;

		iter_swap(cities.begin(), cities.begin() + minIndex);
		journeyPath.push_back(cities[0]);
		cities.erase(cities.begin() + minIndex);
	}

	double lastDistance = abs(
			(journeyPath.back().get_x() - cities[0].get_x())
					+ (journeyPath.back().get_y() - cities[0].get_y()));
	cities.erase(cities.begin());
	totalDistance += lastDistance;

	double distanceBackToInitial = abs(
			(journeyPath.back().get_x() - journeyPath[0].get_x())
					+ (journeyPath.back().get_y() - journeyPath[0].get_y()));
	journeyPath.push_back(journeyPath[0]);
	totalDistance += distanceBackToInitial;

	if (shortestDistance != 0 && totalDistance > shortestDistance) {
		return shortestRoute;
	} else {
		shortestRoute = journeyPath;
		shortestDistance = totalDistance;
	}

	return shortestRoute;
}

double Agent::calculateDistance(double lon1, double lat1, double lon2,
		double lat2) {
	double r = 6372.8;
	lat1 = (lat1 * M_PI) / 180.;
	lat2 = (lat2 * M_PI) / 180.;
	double dLat = lat2 - lat1;
	double dLon = ((lon2 * M_PI) / 180.) - ((lon1 * M_PI) / 180.);
	double a = pow(sin(dLat / 2), 2)
			+ pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
	double c = 2 * asin(sqrt(a));
	return r * c;
}

void Agent::random_restart_hill_climb() {

	int counter = 0;
	vector<City> journeyPath;
	std::srand(std::time(0));

	while (counter != 200) {

		random_shuffle(cities.begin(), cities.end());

		journeyPath = hillClimb(cities);

		counter += 1;
	}

	for (std::vector<City>::iterator it = journeyPath.begin();
			it != journeyPath.end(); ++it) {
		cout << (*it).get_name() << endl;
	}
	cout << journeyPath.size() << endl;
	cout << "total distance " << shortestDistance << endl;
	cout << "-------------------------------------------" << endl;
	print_cities(journeyPath);
}

vector<City> Agent::simulatedAnnealing(vector<City> cities) {

	// Get the first city in the vector.
	City start_city = cities[0];

	// Create a vector for the cities, which has been visited already.
	vector<City> journeyPath;

	// Push the first city in the new vector.
	journeyPath.push_back(cities[0]);

	// Variable for the closest cities. Min distance, and the index of the city.
	double min;
	int minIndex;

	// Variable fot the total distance of the visited cities.
	double totalDistance = 0;

	// Run until in the city-vector is just one city left.
	while (cities.size() != 1) {

		// Get the distance to the first neighbor.
		min = calculateDistance(cities[0].get_x(), cities[0].get_y(),
				cities[1].get_x(), cities[1].get_y());
		// Get the index of the first neighbor.
		minIndex = 1;

		// Iterate over the rest of the vector. And check the distances.
		for (std::vector<City>::iterator it = cities.begin() + 2;
				it != cities.end(); ++it) {
			double distance = calculateDistance(cities[0].get_x(),
					cities[0].get_y(), (*it).get_x(), (*it).get_y());
			if (distance < min) {
				min = distance;
				minIndex = it - cities.begin();
			}
		}

		// Add the distance of the closest city to the total distance of the route.
		totalDistance += min;

		// Swap the closest city to the first position of the vector and delete the current city out of it.
		iter_swap(cities.begin(), cities.begin() + minIndex);
		// Add the new first position city to the journeypath vector.
		journeyPath.push_back(cities[0]);
		cities.erase(cities.begin() + minIndex);
	}

	// Get the distance to the last remaining city.
	double lastDistance = abs(
			(journeyPath.back().get_x() - cities[0].get_x())
					+ (journeyPath.back().get_y() - cities[0].get_y()));
	cities.erase(cities.begin());
	totalDistance += lastDistance;

	// Add the distance back to the starting city.
	double distanceBackToInitial = abs(
			(journeyPath.back().get_x() - journeyPath[0].get_x())
					+ (journeyPath.back().get_y() - journeyPath[0].get_y()));
	journeyPath.push_back(journeyPath[0]);
	totalDistance += distanceBackToInitial;

	// Use characteristic function of simulated annealing search to decide if the new route should be chosen as shortest route.
	if (shortestDistance != 0 && totalDistance > shortestDistance) {
		// Get the difference of shortestDistance and currentDistance.
		double deltaDistance = totalDistance - shortestDistance;
		// Compute probability of choosing the route as shortest route.
		double probability = exp(-deltaDistance / iteration);
		// Use computed probability.
		std::default_random_engine generator;
		std::bernoulli_distribution distribution(probability);
		if (distribution(generator)) {
			shortestRoute = journeyPath;
			shortestDistance = totalDistance;
		} else {
			return shortestRoute;
		}
	} else {
		shortestRoute = journeyPath;
		shortestDistance = totalDistance;
	}

	// Return the shortest route.
	return shortestRoute;
}

void Agent::start_simulated_annealing(int minuets) {

	// Variable for the duration.
	time_t endwait;

	// Variable for the start and later important for decreasing the timer.
	time_t start = time(NULL);

	// Variable for the duration. Converts from minuets to seconds.
	time_t seconds = minuets * 60;

	// At the beginning is the duration equals 'minuets' (converted in seconds)
	endwait = start + seconds;

	// Vector, which will contain the final route.
	vector<City> journeyPath;

	// The following lines create a first, random route.
	std::srand(std::time(0));
	random_shuffle(cities.begin(), cities.end());
	journeyPath = hillClimb(cities);

	while (start < endwait) {

		// The following two lines, are the ticking timer. Setting start equals time(NULL) decreases the time.
		sleep(1);
		start = time(NULL);

		// For each iteration the algorithm runs with another order of cities.
		random_shuffle(cities.begin(), cities.end());
		journeyPath = simulatedAnnealing(cities);

		/* Increase the variable iteration. It is the denominator of the 'temperature-formula'.
		 * With every iteration the probability decreases, because the denomiator becomes bigger.
		 */
		iteration += 1;
	}

	// Print all the cities, which have been visited.
	for (std::vector<City>::iterator it = journeyPath.begin();
			it != journeyPath.end(); ++it) {
		cout << (*it).get_name() << endl;
	}
	// Print the amount of visited cities.
	cout << journeyPath.size() << endl;
	// Print the total distance of the route.
	cout << "total distance " << shortestDistance << endl;
	cout << "-------------------------------------------" << endl;
	// Print the map.
	print_cities(journeyPath);

}

void Agent::choose_solving_approach() {
	string approach = "-1";

	cout << "assignment_08 " << endl;
	cout << "Select approach: " << endl;
	cout << "(1) Hill Climbing (2) Simulated Annealing" << endl;
	cin >> approach;

	// Choose one of the approaches.
	if (approach == "1") {
		cout << "Hill Climbing" << endl;
		random_restart_hill_climb();
	}

	if (approach == "2") {
		string minuets = "-1";
		cout << "Simulated Annealing" << endl;
		cout << "(1) 1min (2) 3min (3) 5min (4) 10min (5) 15min (6) 30min" << endl;
		cin >> minuets;

		// Choose how long the approach should run.
		if (minuets == "1") {
			start_simulated_annealing(1);
		}
		if (minuets == "2") {
			start_simulated_annealing(3);
		}
		if (minuets == "3") {
			start_simulated_annealing(5);
		}
		if (minuets == "4") {
			start_simulated_annealing(10);
		}
		if (minuets == "5") {
			start_simulated_annealing(15);
		}
		if (minuets == "6") {
			start_simulated_annealing(30);
		}
	}
}
