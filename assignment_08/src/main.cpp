/*
 * Daniel Vázquez
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 7
 *
 * main.cpp
 * */
 
#include <iostream>
#include <chrono>
#include <agent.hpp>
#include <environment.hpp>

using ms = chrono::milliseconds;

int main(int argc, char* argv[]) { 
    Environment env;
    env.run();
    return 0;
}
