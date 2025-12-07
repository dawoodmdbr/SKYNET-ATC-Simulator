#ifndef UTILS_H
#define UTILS_H

#include "Flight.h"
#include "MinHeap.h"
#include "AVL.h"
#include "Graph.h"
#include "HashTable.h"
#include <string>

class Utils {
public:
    static long getCurrentTimestamp();
    static void updateFuelAndCheckEmergency(Flight* flight, MinHeap& heap, AVL& avl, Graph& graph, HashTable& hashTable);
    static bool moveFlightTowardsAirport(Flight* flight, Graph& graph, HashTable& hashTable, AVL& avl);
};

#endif

