#ifndef GRAPH_H
#define GRAPH_H

#include <string>

struct Edge {
    int to;
    int weight;
    Edge* next;
    
    Edge(int t, int w) : to(t), weight(w), next(nullptr) {}
};

struct Node {
    std::string id;
    int x;
    int y;
    bool isAirport;
    Edge* head;
    
    Node() : x(0), y(0), isAirport(false), head(nullptr) {}
};

struct PathNode {
    int nodeIndex;
    PathNode* next;
    
    PathNode(int idx) : nodeIndex(idx), next(nullptr) {}
};

class Graph {
private:
    Node* nodes;
    int nodeCount;
    int capacity;
    bool* occupied;
    
    void expandCapacity();
    int calculateDistance(int from, int to);
    
public:
    Graph();
    ~Graph();
    
    int addNode(const std::string& id, int x, int y, bool isAirport);
    void addEdge(int from, int to, int weight);
    PathNode* findNearestAirportDijkstra(int startNode, bool occupancyAware = false);
    bool isOccupied(int nodeIndex);
    void setOccupied(int nodeIndex, bool status);
    int getNodeCount() const;
    Node* getNode(int index);
    int findNodeIndex(const std::string& id);
    void clearPath(PathNode* path);
};

#endif

