#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Flight.h"
#include "Config.h"

class Graph;

struct HashNode {
    Flight* flight;
    HashNode* next;
    
    HashNode(Flight* f) : flight(f), next(nullptr) {}
};

class HashTable {
private:
    HashNode** table;
    int tableSize;
    
    int hashFunction(const std::string& id);
    
public:
    HashTable(int size = HASH_TABLE_DEFAULT_SIZE);
    ~HashTable();
    
    void insert(Flight* flight);
    Flight* search(const std::string& id);
    void remove(const std::string& id);
    void updatePosition(const std::string& id, int newNodeIndex);
    void placeOnRadar(char** grid, int gridRows, int gridCols, Graph& graph);
    void getAllFlights(Flight** flights, int& count, int maxCount = -1);
};

#endif

