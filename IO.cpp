#include "IO.h"
#include "AVL.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

void IO::saveState(HashTable& hashTable, AVL& avl) {
    std::ofstream flightFile("flights.txt");
    if (!flightFile.is_open()) {
        std::cout << "Error: Cannot open flights.txt for writing\n";
        return;
    }
    
    int initialSize = INITIAL_FLIGHT_ARRAY_SIZE;
    Flight** flights = new Flight*[initialSize];
    int count = 0;
    hashTable.getAllFlights(flights, count, initialSize);
    
    if (count >= initialSize) {
        delete[] flights;
        int newSize = initialSize * 2;
        flights = new Flight*[newSize];
        count = 0;
        hashTable.getAllFlights(flights, count, newSize);
        if (count >= newSize) {
            delete[] flights;
            flights = new Flight*[count + 100];
            count = 0;
            hashTable.getAllFlights(flights, count);
        }
    }
    
    for (int i = 0; i < count; i++) {
        Flight* f = flights[i];
        flightFile << f->id << "|" << f->model << "|" << f->fuel_percent << "|"
                   << f->priority << "|" << f->nodeIndex << "|" 
                   << (f->inAir ? 1 : 0) << "|" << (f->crashed ? 1 : 0) 
                   << "|" << f->timestamp_landed << "\n";
    }
    
    flightFile.close();
    delete[] flights;
    
    std::ofstream logFile("logs.txt");
    if (!logFile.is_open()) {
        std::cout << "Error: Cannot open logs.txt for writing\n";
        return;
    }
    
    int logArraySize = INITIAL_LOG_ARRAY_SIZE;
    LogEntry* logs = new LogEntry[logArraySize];
    int logCount = 0;
    avl.collectLogs(logs, logCount, logArraySize);
    
    if (logCount >= logArraySize) {
        delete[] logs;
        logArraySize = logCount * 2;
        logs = new LogEntry[logArraySize];
        avl.collectLogs(logs, logCount, logArraySize);
    }
    
    for (int i = 0; i < logCount; i++) {
        logFile << logs[i].timestamp << "|" << logs[i].flightId << "|"
                << logs[i].status << "|" << logs[i].notes << "\n";
    }
    
    logFile.close();
    delete[] logs;
    
    std::cout << "State saved successfully!\n";
}

void IO::loadState(HashTable& hashTable, MinHeap& heap, AVL& avl, Graph& graph, Radar& radar) {
    std::ifstream flightFile("flights.txt");
    if (!flightFile.is_open()) {
        std::cout << "No saved state found. Starting fresh.\n";
        return;
    }
    
    std::string line;
    while (std::getline(flightFile, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string token;
        std::string parts[8];
        int idx = 0;
        
        while (std::getline(iss, token, '|') && idx < 8) {
            parts[idx++] = token;
        }
        
        if (idx >= 8) {
            Flight* flight = new Flight(parts[0], parts[1], std::stoi(parts[2]), 
                                       std::stoi(parts[3]), std::stoi(parts[4]));
            flight->inAir = (parts[5] == "1");
            flight->crashed = (parts[6] == "1");
            flight->timestamp_landed = std::stol(parts[7]);
            
            hashTable.insert(flight);
            if (flight->inAir && !flight->crashed) {
                heap.insert(flight);
            }
            
            graph.setOccupied(flight->nodeIndex, true);
        }
    }
    
    flightFile.close();
    
    std::ifstream logFile("logs.txt");
    if (logFile.is_open()) {
        while (std::getline(logFile, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string token;
            std::string parts[4];
            int idx = 0;
            
            while (std::getline(iss, token, '|') && idx < 4) {
                parts[idx++] = token;
            }
            
            if (idx >= 4) {
                avl.insert(std::stol(parts[0]), parts[1], parts[2], parts[3]);
            }
        }
        logFile.close();
    }
    
    std::cout << "State loaded successfully!\n";
}

