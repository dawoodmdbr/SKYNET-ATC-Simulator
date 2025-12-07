#ifndef RADAR_H
#define RADAR_H

#include "Graph.h"
#include "HashTable.h"
#include "Config.h"

class Radar {
private:
    char** grid;
    int gridRows;
    int gridCols;
    Graph* graph;
    
    void allocateGrid();
    void deallocateGrid();
    
public:
    Radar(int rows = GRID_ROWS, int cols = GRID_COLS);
    ~Radar();
    
    void setGraph(Graph* g);
    void clearGrid();
    void placeAirports(Graph& g);
    void placePlanes(HashTable& hashTable);
    void draw();
    void resizeGrid(int rows, int cols);
    int getGridRows() const { return gridRows; }
    int getGridCols() const { return gridCols; }
};

#endif

