#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

class Flight {
public:
    std::string id;
    std::string model;
    int fuel_percent;
    int priority;
    int nodeIndex;
    int heapIndex;
    bool inAir;
    bool crashed;
    long timestamp_landed;
    Flight* next;

    Flight();
    Flight(const std::string& id, const std::string& model, int fuel, int priority, int nodeIndex);
    ~Flight();
};

#endif

