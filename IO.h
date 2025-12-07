#ifndef IO_H
#define IO_H

#include "Flight.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "AVL.h"
#include "Graph.h"

class Radar;

class IO {
public:
    static void saveState(HashTable& hashTable, AVL& avl);
    static void loadState(HashTable& hashTable, MinHeap& heap, AVL& avl, Graph& graph, Radar& radar);
};

#endif

