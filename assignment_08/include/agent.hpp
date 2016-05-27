/*
 * Daniel Vázquez, Kiran Vasudev, Patrick Nagel
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 7
 *
 * agent.hpp
 * */
#ifndef Agent_H_
#define Agent_H_

#include <vector>
#include <fstream>
#include <environment.hpp>

using namespace std;

class Agent {
    private:
        vector<City> cities;
        double shortestDistance;
        vector<City> shortestRoute;
        
        vector<City> hillClimb(vector<City> cities);
        void random_restart_hill_climb();
        
        

    public:
        Agent(vector<City> Cities);
        ~Agent();
        void run();
        void print_cities(vector<City>&);
        void save_cities_image(vector<City>& cities, int file_counter);
        double calculateDistance(double lat1, double lon1, double lat2, double lon2);
};

#endif // Agent_H_
