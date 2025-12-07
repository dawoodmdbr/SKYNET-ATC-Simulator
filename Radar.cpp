#include "Radar.h"
#include <iostream>
#include <iomanip>

Radar::Radar(int rows, int cols) : gridRows(rows), gridCols(cols), graph(nullptr) {
    allocateGrid();
    clearGrid();
}

Radar::~Radar() {
    deallocateGrid();
}

void Radar::allocateGrid() {
    grid = new char*[gridRows];
    for (int i = 0; i < gridRows; i++) {
        grid[i] = new char[gridCols];
    }
}

void Radar::deallocateGrid() {
    if (grid) {
        for (int i = 0; i < gridRows; i++) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
}

void Radar::resizeGrid(int rows, int cols) {
    deallocateGrid();
    gridRows = rows;
    gridCols = cols;
    allocateGrid();
    clearGrid();
}

void Radar::setGraph(Graph* g) {
    graph = g;
}

void Radar::clearGrid() {
    for (int i = 0; i < gridRows; i++) {
        for (int j = 0; j < gridCols; j++) {
            grid[i][j] = GRID_SYMBOL_EMPTY;
        }
    }
}

void Radar::placeAirports(Graph& g) {
    int nodeCount = g.getNodeCount();
    for (int i = 0; i < nodeCount; i++) {
        Node* node = g.getNode(i);
        if (node && node->isAirport) {
            int x = node->x;
            int y = node->y;
            if (x >= 0 && x < gridCols && y >= 0 && y < gridRows) {
                grid[y][x] = GRID_SYMBOL_AIRPORT;
            }
        }
    }
}

void Radar::placePlanes(HashTable& hashTable) {
    if (graph) {
        hashTable.placeOnRadar(grid, gridRows, gridCols, *graph);
    }
}

void Radar::draw() {
    std::cout << std::endl << "   ";
    for (int i = 0; i < gridCols; i++) {
        std::cout << std::setw(2) << i;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < gridRows; i++) {
        std::cout << std::setw(2) << i << " ";
        for (int j = 0; j < gridCols; j++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "Legend: A=Airport, P=Flight, .=Empty" << std::endl;
}

