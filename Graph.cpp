#include "Graph.h"
#include "MinHeap.h"
#include "Config.h"
#include <climits>
#include <cstdlib>

Graph::Graph() : nodeCount(0), capacity(GRAPH_INITIAL_CAPACITY) {
    nodes = new Node[capacity];
    occupied = new bool[capacity];
    for (int i = 0; i < capacity; i++) {
        occupied[i] = false;
    }
}

Graph::~Graph() {
    for (int i = 0; i < nodeCount; i++) {
        Edge* curr = nodes[i].head;
        while (curr) {
            Edge* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete[] nodes;
    delete[] occupied;
}

void Graph::expandCapacity() {
    int newCapacity = capacity * 2;
    Node* newNodes = new Node[newCapacity];
    bool* newOccupied = new bool[newCapacity];
    
    for (int i = 0; i < nodeCount; i++) {
        newNodes[i] = nodes[i];
        newOccupied[i] = occupied[i];
    }
    for (int i = nodeCount; i < newCapacity; i++) {
        newOccupied[i] = false;
    }
    
    delete[] nodes;
    delete[] occupied;
    nodes = newNodes;
    occupied = newOccupied;
    capacity = newCapacity;
}

int Graph::addNode(const std::string& id, int x, int y, bool isAirport) {
    if (nodeCount >= capacity) {
        expandCapacity();
    }
    
    nodes[nodeCount].id = id;
    nodes[nodeCount].x = x;
    nodes[nodeCount].y = y;
    nodes[nodeCount].isAirport = isAirport;
    nodes[nodeCount].head = nullptr;
    occupied[nodeCount] = false;
    
    return nodeCount++;
}

void Graph::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= nodeCount || to < 0 || to >= nodeCount) return;
    
    Edge* newEdge = new Edge(to, weight);
    newEdge->next = nodes[from].head;
    nodes[from].head = newEdge;
    
    Edge* reverseEdge = new Edge(from, weight);
    reverseEdge->next = nodes[to].head;
    nodes[to].head = reverseEdge;
}

int Graph::calculateDistance(int from, int to) {
    if (from < 0 || from >= nodeCount || to < 0 || to >= nodeCount) return INT_MAX;
    int dx = nodes[from].x - nodes[to].x;
    int dy = nodes[from].y - nodes[to].y;
    return (dx * dx + dy * dy);
}

bool Graph::isOccupied(int nodeIndex) {
    if (nodeIndex < 0 || nodeIndex >= nodeCount) return false;
    return occupied[nodeIndex];
}

void Graph::setOccupied(int nodeIndex, bool status) {
    if (nodeIndex >= 0 && nodeIndex < nodeCount) {
        occupied[nodeIndex] = status;
    }
}

int Graph::getNodeCount() const {
    return nodeCount;
}

Node* Graph::getNode(int index) {
    if (index < 0 || index >= nodeCount) return nullptr;
    return &nodes[index];
}

int Graph::findNodeIndex(const std::string& id) {
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i].id == id) {
            return i;
        }
    }
    return -1;
}

PathNode* Graph::findNearestAirportDijkstra(int startNode, bool occupancyAware) {
    if (startNode < 0 || startNode >= nodeCount) return nullptr;
    
    int* dist = new int[nodeCount];
    int* parent = new int[nodeCount];
    bool* visited = new bool[nodeCount];
    
    for (int i = 0; i < nodeCount; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
        visited[i] = false;
    }
    
    dist[startNode] = 0;
    
    for (int count = 0; count < nodeCount; count++) {
        int u = -1;
        int minDist = INT_MAX;
        
        for (int i = 0; i < nodeCount; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || minDist == INT_MAX) break;
        visited[u] = true;
        
        Edge* curr = nodes[u].head;
        while (curr) {
            int v = curr->to;
            if (!visited[v]) {
                if (occupancyAware && occupied[v] && !nodes[v].isAirport) {
                    curr = curr->next;
                    continue;
                }
                
                int alt = dist[u] + curr->weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    parent[v] = u;
                }
            }
            curr = curr->next;
        }
    }
    
    int nearestAirport = -1;
    int minDist = INT_MAX;
    
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i].isAirport && dist[i] < minDist && dist[i] != INT_MAX) {
            if (occupancyAware && occupied[i]) continue;
            minDist = dist[i];
            nearestAirport = i;
        }
    }
    
    PathNode* path = nullptr;
    if (nearestAirport != -1) {
        int current = nearestAirport;
        PathNode* tail = nullptr;
        
        while (current != -1) {
            PathNode* newNode = new PathNode(current);
            if (path == nullptr) {
                path = newNode;
                tail = newNode;
            } else {
                newNode->next = path;
                path = newNode;
            }
            current = parent[current];
        }
    }
    
    delete[] dist;
    delete[] parent;
    delete[] visited;
    
    return path;
}

void Graph::clearPath(PathNode* path) {
    while (path) {
        PathNode* temp = path;
        path = path->next;
        delete temp;
    }
}

