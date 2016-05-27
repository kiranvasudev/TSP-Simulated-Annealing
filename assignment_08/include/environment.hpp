/*
 * Daniel Vázquez
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 7
 *
 * environment.cpp
 * */

#ifndef ENV_H_
#define ENV_H_

#include <string>
#include <vector>


using namespace std;

class City {
    private:
        string name;
        float x;
        float y;

    public:
        City(string name, float x, float y);
        ~City();
        string get_name();
        void set_name(string name);
        double get_x();
        void set_x(float coord);
        double get_y();
        void set_y(float coord);
};

class Environment {
public:
	Environment()
	{
		
	}
	~Environment() {
	}
	void run();
	vector<City> read_file();

private:
	vector<City> cities;

};
#endif /* ENV_H_ */
