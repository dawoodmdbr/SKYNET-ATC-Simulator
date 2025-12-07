#include "HashTable.h"
#include "Graph.h"
#include <cstring>

HashTable::HashTable(int size) : tableSize(size) {
    table = new HashNode*[tableSize];
    for (int i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        HashNode* curr = table[i];
        while (curr) {
            HashNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete[] table;
}

int HashTable::hashFunction(const std::string& id) {
    int sum = 0;
    for (size_t i = 0; i < id.length(); i++) {
        sum += (int)id[i];
    }
    return sum % tableSize;
}

void HashTable::insert(Flight* flight) {
    int index = hashFunction(flight->id);
    HashNode* newNode = new HashNode(flight);
    newNode->next = table[index];
    table[index] = newNode;
}

Flight* HashTable::search(const std::string& id) {
    int index = hashFunction(id);
    HashNode* curr = table[index];
    while (curr) {
        if (curr->flight->id == id) {
            return curr->flight;
        }
        curr = curr->next;
    }
    return nullptr;
}

void HashTable::remove(const std::string& id) {
    int index = hashFunction(id);
    HashNode* curr = table[index];
    HashNode* prev = nullptr;
    
    while (curr) {
        if (curr->flight->id == id) {
            if (prev) {
                prev->next = curr->next;
            } else {
                table[index] = curr->next;
            }
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void HashTable::updatePosition(const std::string& id, int newNodeIndex) {
    Flight* flight = search(id);
    if (flight) {
        flight->nodeIndex = newNodeIndex;
    }
}

void HashTable::placeOnRadar(char** grid, int gridRows, int gridCols, Graph& graph) {
    for (int i = 0; i < tableSize; i++) {
        HashNode* curr = table[i];
        while (curr) {
            Flight* flight = curr->flight;
            if (flight && flight->inAir && !flight->crashed) {
                Node* node = graph.getNode(flight->nodeIndex);
                if (node) {
                    int x = node->x;
                    int y = node->y;
                    if (x >= 0 && x < gridCols && y >= 0 && y < gridRows) {
                        if (grid[y][x] == GRID_SYMBOL_EMPTY) {
                            grid[y][x] = GRID_SYMBOL_PLANE;
                        } else if (grid[y][x] == GRID_SYMBOL_AIRPORT) {
                            grid[y][x] = GRID_SYMBOL_AIRPORT;
                        }
                    }
                }
            }
            curr = curr->next;
        }
    }
}

void HashTable::getAllFlights(Flight** flights, int& count, int maxCount) {
    count = 0;
    for (int i = 0; i < tableSize; i++) {
        HashNode* curr = table[i];
        while (curr) {
            if (maxCount > 0 && count >= maxCount) {
                return; // Array full, stop adding
            }
            flights[count++] = curr->flight;
            curr = curr->next;
        }
    }
}

