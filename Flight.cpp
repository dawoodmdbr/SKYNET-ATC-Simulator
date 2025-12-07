#include "Flight.h"
#include "Config.h"

Flight::Flight() : fuel_percent(DEFAULT_FUEL_PERCENT), priority(DEFAULT_PRIORITY), nodeIndex(0), heapIndex(-1), 
                   inAir(true), crashed(false), timestamp_landed(0), next(nullptr) {
}

Flight::Flight(const std::string& id, const std::string& model, int fuel, int priority, int nodeIndex)
    : id(id), model(model), fuel_percent(fuel), priority(priority), nodeIndex(nodeIndex),
      heapIndex(-1), inAir(true), crashed(false), timestamp_landed(0), next(nullptr) {
}

Flight::~Flight() {
}

