#include "Utils.h"
#include "Config.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

long Utils::getCurrentTimestamp() {
    return static_cast<long>(time(nullptr));
}

void Utils::updateFuelAndCheckEmergency(Flight* flight, MinHeap& heap, AVL& avl, Graph& graph, HashTable& hashTable) {
    if (!flight || !flight->inAir || flight->crashed) return;
    
    int fuelDecrease = rand() % (FUEL_DECREASE_MAX - FUEL_DECREASE_MIN + 1) + FUEL_DECREASE_MIN;
    flight->fuel_percent -= fuelDecrease;
    
    if (flight->fuel_percent < 0) {
        flight->fuel_percent = 0;
    }
    
    if (flight->fuel_percent < LOW_FUEL_THRESHOLD && flight->priority > AUTO_PRIORITY_THRESHOLD) {
        heap.decreaseKey(flight->heapIndex, AUTO_PRIORITY_THRESHOLD);
        avl.insert(Utils::getCurrentTimestamp(), flight->id, "EMERGENCY", 
                   "Low fuel - auto priority set to " + std::to_string(AUTO_PRIORITY_THRESHOLD));
    }
    
    if (flight->fuel_percent <= 0) {
        flight->crashed = true;
        flight->inAir = false;
        
        if (flight->heapIndex >= 0) {
            heap.removeAt(flight->heapIndex);
        }
        
        graph.setOccupied(flight->nodeIndex, false);
        
        avl.insert(Utils::getCurrentTimestamp(), flight->id, "CRASHED", 
                   "Fuel exhausted - flight crashed");
        
        std::cout << "ALERT: Flight " << flight->id << " has crashed due to fuel exhaustion!" << std::endl;
    }
}

bool Utils::moveFlightTowardsAirport(Flight* flight, Graph& graph, HashTable& hashTable, AVL& avl) {
    if (!flight || !flight->inAir || flight->crashed) return false;
    
    Node* currentNode = graph.getNode(flight->nodeIndex);
    if (!currentNode) return false;
    
    if (currentNode->isAirport) {
        return false;
    }
    
    PathNode* path = graph.findNearestAirportDijkstra(flight->nodeIndex, true);
    
    if (!path) {
        return false;
    }
    
    PathNode* nextNode = path->next;
    if (nextNode) {
        int targetNode = nextNode->nodeIndex;
        
        if (graph.isOccupied(targetNode) && !graph.getNode(targetNode)->isAirport) {
            graph.clearPath(path);
            path = graph.findNearestAirportDijkstra(flight->nodeIndex, true);
            
            if (!path || !path->next) {
                if (path) graph.clearPath(path);
                return false;
            }
            
            nextNode = path->next;
            targetNode = nextNode->nodeIndex;
        }
        
        graph.setOccupied(flight->nodeIndex, false);
        flight->nodeIndex = targetNode;
        graph.setOccupied(targetNode, true);
        
        graph.clearPath(path);
        return true;
    }
    
    graph.clearPath(path);
    return false;
}

