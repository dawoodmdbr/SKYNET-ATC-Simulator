#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "Flight.h"
#include "Graph.h"
#include "Radar.h"
#include "MinHeap.h"
#include "HashTable.h"
#include "AVL.h"
#include "IO.h"
#include "Utils.h"
#include "Config.h"

using namespace std;

void loadDemoData(Graph &graph, HashTable &hashTable, MinHeap &heap, AVL &avl)
{
    cout << "Loading demo data..." << endl;
    
    int jfk = graph.addNode("JFK", 2, 3, true);
    int lhr = graph.addNode("LHR", 10, 5, true);
    int dxb = graph.addNode("DXB", 15, 12, true);

    int n1 = graph.addNode("N1", 5, 4, false);
    int n2 = graph.addNode("N2", 8, 6, false);
    int n3 = graph.addNode("N3", 12, 9, false);
    int n4 = graph.addNode("N4", 7, 8, false);

    graph.addEdge(jfk, n1, 4);
    graph.addEdge(n1, n2, 3);
    graph.addEdge(n2, lhr, 3);
    graph.addEdge(lhr, n3, 4);
    graph.addEdge(n3, dxb, 4);
    graph.addEdge(jfk, n4, 6);
    graph.addEdge(n4, n3, 5);
    graph.addEdge(n2, n4, 2);
    Flight *f1 = new Flight("PK-101", "Boeing-777", 85, PRIORITY_NORMAL, jfk);
    Flight *f2 = new Flight("UA-909", "Airbus-A380", 70, PRIORITY_HIGH, lhr);
    Flight *f3 = new Flight("EK-202", "Boeing-787", 60, PRIORITY_EMERGENCY, dxb);

    f1->inAir = true;
    f1->crashed = false;
    f2->inAir = true;
    f2->crashed = false;
    f3->inAir = true;
    f3->crashed = false;

    hashTable.insert(f1);
    hashTable.insert(f2);
    hashTable.insert(f3);

    heap.insert(f1);
    heap.insert(f2);
    heap.insert(f3);

    graph.setOccupied(jfk, true);
    graph.setOccupied(lhr, true);
    graph.setOccupied(dxb, true);

    avl.insert(Utils::getCurrentTimestamp(), "PK-101", "DEPARTED", "Flight initialized at JFK");
    avl.insert(Utils::getCurrentTimestamp(), "UA-909", "DEPARTED", "Flight initialized at LHR");
    avl.insert(Utils::getCurrentTimestamp(), "EK-202", "DEPARTED", "Flight initialized at DXB");
    
    cout << "Demo data loaded successfully!" << endl;
}

void showMenu()
{
    cout << endl << "========== SKYNET ATC SIMULATOR ==========" << endl;
    cout << "1) Airspace Visualizer" << endl;
    cout << "2) Airspace Configuration" << endl;
    cout << "   2.1) Add Airport" << endl;
    cout << "   2.2) Add Waypoint/Node" << endl;
    cout << "   2.3) Add Flight Corridor (Edge)" << endl;
    cout << "   2.4) View All Nodes" << endl;
    cout << "3) Flight Control" << endl;
    cout << "   3.1) Add Flight" << endl;
    cout << "   3.2) Move Plane" << endl;
    cout << "   3.3) Declare Emergency" << endl;
    cout << "   3.4) Land Flight" << endl;
    cout << "4) Flight Lookup & Management" << endl;
    cout << "   4.1) Search Flight by ID" << endl;
    cout << "   4.2) Update Flight Info" << endl;
    cout << "5) Route & Path Management" << endl;
    cout << "   5.1) Find Safe Route" << endl;
    cout << "   5.2) Assign Flight Path" << endl;
    cout << "6) Flight History & Logs" << endl;
    cout << "   6.1) Print Full Log" << endl;
    cout << "   6.2) Print Crashed Flights Only" << endl;
    cout << "   6.3) Print Landed Flights Only" << endl;
    cout << "7) System Management" << endl;
    cout << "   7.1) Save State" << endl;
    cout << "   7.2) Load State" << endl;
    cout << "   7.3) Reset System" << endl;
    cout << "   7.4) Load Demo Data" << endl;
    cout << "0) Exit" << endl;
    cout << "=========================================" << endl;
    cout << "Choice: ";
}

void airspaceVisualizer(Radar &radar, Graph &graph, HashTable &hashTable)
{
    radar.clearGrid();
    radar.placeAirports(graph);
    radar.placePlanes(hashTable);
    radar.draw();
}

void addFlight(Graph &graph, HashTable &hashTable, MinHeap &heap, AVL &avl)
{
    string id, model;
    int fuel, priority, nodeIndex;

    cout << "Enter Flight ID: ";
    cin >> id;

    if (hashTable.search(id) != nullptr)
    {
        cout << "Flight ID already exists!" << endl;
        return;
    }

    cout << "Enter Model: ";
    cin >> model;
    cout << "Enter Fuel %: ";
    cin >> fuel;
    cout << "Enter Priority (" << PRIORITY_EMERGENCY << "=Emergency, " << PRIORITY_HIGH << "=High, " << PRIORITY_NORMAL << "=Normal): ";
    cin >> priority;

    cout << "Available nodes:" << endl;
    int nodeCount = graph.getNodeCount();
    for (int i = 0; i < nodeCount; i++)
    {
        Node *node = graph.getNode(i);
        if (node && !graph.isOccupied(i))
        {
            cout << "  Node " << i << ": " << node->id << " (" << node->x << "," << node->y << ")";
            if (node->isAirport)
                cout << " [AIRPORT]";
            cout << endl;
        }
    }

    cout << "Enter Node Index: ";
    cin >> nodeIndex;

    if (nodeIndex < 0 || nodeIndex >= nodeCount || graph.isOccupied(nodeIndex))
    {
        cout << "Invalid or occupied node!" << endl;
        return;
    }

    Flight *flight = new Flight(id, model, fuel, priority, nodeIndex);
    hashTable.insert(flight);
    heap.insert(flight);
    graph.setOccupied(nodeIndex, true);

    avl.insert(Utils::getCurrentTimestamp(), id, "DEPARTED", "Flight added to system");
    cout << "Flight " << id << " added successfully!" << endl;
}

void movePlane(HashTable &hashTable, MinHeap &heap, AVL &avl, Graph &graph)
{
    string id;
    cout << "Enter Flight ID: ";
    cin >> id;

    Flight *flight = hashTable.search(id);
    if (!flight)
    {
        cout << "Flight not found!" << endl;
        return;
    }

    if (!flight->inAir || flight->crashed)
    {
        cout << "Flight is not in air!" << endl;
        return;
    }

    cout << "Available nodes:" << endl;
    int nodeCount = graph.getNodeCount();
    for (int i = 0; i < nodeCount; i++)
    {
        Node *node = graph.getNode(i);
        if (node && !graph.isOccupied(i))
        {
            cout << "  Node " << i << ": " << node->id << " (" << node->x << "," << node->y << ")";
            if (node->isAirport)
                cout << " [AIRPORT]";
            cout << endl;
        }
    }

    int newNodeIndex;
    cout << "Enter Target Node Index: ";
    cin >> newNodeIndex;

    if (newNodeIndex < 0 || newNodeIndex >= nodeCount)
    {
        cout << "Invalid node index!" << endl;
        return;
    }

    if (graph.isOccupied(newNodeIndex) && !graph.getNode(newNodeIndex)->isAirport)
    {
        cout << "Target node is occupied! Collision avoided." << endl;
        return;
    }

    if (flight->fuel_percent < FUEL_COST_PER_MOVE)
    {
        cout << "Insufficient fuel for movement!" << endl;
        return;
    }

    graph.setOccupied(flight->nodeIndex, false);
    flight->nodeIndex = newNodeIndex;
    flight->fuel_percent -= FUEL_COST_PER_MOVE;
    graph.setOccupied(newNodeIndex, true);

    if (flight->fuel_percent < LOW_FUEL_THRESHOLD && flight->priority > AUTO_PRIORITY_THRESHOLD)
    {
        heap.decreaseKey(flight->heapIndex, AUTO_PRIORITY_THRESHOLD);
        avl.insert(Utils::getCurrentTimestamp(), id, "EMERGENCY", "Low fuel - auto priority set to " + to_string(AUTO_PRIORITY_THRESHOLD));
    }

    Node *node = graph.getNode(newNodeIndex);
    avl.insert(Utils::getCurrentTimestamp(), id, "MOVED", "Flight moved to " + node->id);
    cout << "Flight " << id << " moved successfully to " << node->id << "!" << endl;
}

void declareEmergency(HashTable &hashTable, MinHeap &heap, AVL &avl)
{
    string id;
    cout << "Enter Flight ID: ";
    cin >> id;

    Flight *flight = hashTable.search(id);
    if (!flight)
    {
        cout << "Flight not found!" << endl;
        return;
    }

    if (!flight->inAir || flight->crashed)
    {
        cout << "Flight is not in air!" << endl;
        return;
    }

    heap.decreaseKey(flight->heapIndex, PRIORITY_EMERGENCY);
    avl.insert(Utils::getCurrentTimestamp(), id, "EMERGENCY", "Emergency declared by pilot");
    cout << "Emergency declared for " << id << "! Priority set to " << PRIORITY_EMERGENCY << "." << endl;
}

void landFlight(HashTable &hashTable, MinHeap &heap, AVL &avl, Graph &graph)
{
    if (heap.isEmpty())
    {
        cout << "No flights in queue!" << endl;
        return;
    }

    Flight *flight = heap.peek();
    Node *node = graph.getNode(flight->nodeIndex);

    if (!node || !node->isAirport)
    {
        cout << "Flight " << flight->id << " is not at an airport!" << endl;
        return;
    }

    flight = heap.extractMin();
    flight->inAir = false;
    flight->timestamp_landed = Utils::getCurrentTimestamp();
    graph.setOccupied(flight->nodeIndex, false);

    avl.insert(Utils::getCurrentTimestamp(), flight->id, "LANDED",
               "Flight landed at " + node->id);
    cout << "Flight " << flight->id << " landed successfully at " << node->id << "!" << endl;
}

void searchFlight(HashTable &hashTable)
{
    string id;
    cout << "Enter Flight ID: ";
    cin >> id;

    Flight *flight = hashTable.search(id);
    if (!flight)
    {
        cout << "Flight not found!" << endl;
        return;
    }

    cout << endl << "=== Flight Information ===" << endl;
    cout << "ID: " << flight->id << endl;
    cout << "Model: " << flight->model << endl;
    cout << "Fuel: " << flight->fuel_percent << "%" << endl;
    cout << "Priority: " << flight->priority << endl;
    cout << "Node Index: " << flight->nodeIndex << endl;
    cout << "In Air: " << (flight->inAir ? "Yes" : "No") << endl;
    cout << "Crashed: " << (flight->crashed ? "Yes" : "No") << endl;
}

void updateFlightInfo(HashTable &hashTable, MinHeap &heap, AVL &avl)
{
    string id;
    cout << "Enter Flight ID: ";
    cin >> id;

    Flight *flight = hashTable.search(id);
    if (!flight)
    {
        cout << "Flight not found!" << endl;
        return;
    }

    cout << "Current Flight Info:" << endl;
    cout << "  Model: " << flight->model << endl;
    cout << "  Fuel: " << flight->fuel_percent << "%" << endl;
    cout << "  Priority: " << flight->priority << endl;

    int choice;
    cout << "What to update? (1=Fuel, 2=Priority, 3=Both): ";
    cin >> choice;

    if (choice == 1 || choice == 3)
    {
        int newFuel;
        cout << "Enter new Fuel %: ";
        cin >> newFuel;
        if (newFuel >= 0 && newFuel <= 100)
        {
            flight->fuel_percent = newFuel;
            avl.insert(Utils::getCurrentTimestamp(), id, "UPDATED", "Fuel updated to " + to_string(newFuel) + "%");
        }
    }

    if (choice == 2 || choice == 3)
    {
        int newPriority;
        cout << "Enter new Priority (" << PRIORITY_EMERGENCY << "=Emergency, " << PRIORITY_HIGH << "=High, " << PRIORITY_NORMAL << "=Normal): ";
        cin >> newPriority;
        if (newPriority >= PRIORITY_EMERGENCY && newPriority <= PRIORITY_NORMAL)
        {
            int oldPriority = flight->priority;
            flight->priority = newPriority;
            if (newPriority < oldPriority)
            {
                heap.decreaseKey(flight->heapIndex, newPriority);
            }
            avl.insert(Utils::getCurrentTimestamp(), id, "UPDATED", "Priority updated to " + to_string(newPriority));
        }
    }

    cout << "Flight info updated successfully!" << endl;
}

void findSafeRoute(Graph &graph, HashTable &hashTable)
{
    string id;
    cout << "Enter Flight ID: ";
    cin >> id;

    Flight *flight = hashTable.search(id);
    if (!flight)
    {
        cout << "Flight not found!" << endl;
        return;
    }

    PathNode *path = graph.findNearestAirportDijkstra(flight->nodeIndex, true);

    if (!path)
    {
        cout << "No safe route to airport found!" << endl;
        return;
    }

    cout << "Safe route to nearest airport:" << endl;
    PathNode *curr = path;
    while (curr)
    {
        Node *node = graph.getNode(curr->nodeIndex);
        if (node)
        {
            cout << " -> " << node->id << " (" << node->x << "," << node->y << ")";
            if (node->isAirport)
                cout << " [AIRPORT]";
            cout << endl;
        }
        curr = curr->next;
    }

    graph.clearPath(path);
}

void assignFlightPath(Graph &graph, HashTable &hashTable, AVL &avl)
{
    string id;
    cout << "Enter Flight ID: ";
    cin >> id;

    Flight *flight = hashTable.search(id);
    if (!flight)
    {
        cout << "Flight not found!" << endl;
        return;
    }

    if (!flight->inAir || flight->crashed)
    {
        cout << "Flight is not in air!" << endl;
        return;
    }

    PathNode *path = graph.findNearestAirportDijkstra(flight->nodeIndex, true);

    if (!path)
    {
        cout << "No safe route to airport found!" << endl;
        return;
    }

    cout << "Assigned flight path:" << endl;
    PathNode *curr = path;
    int step = 1;
    while (curr)
    {
        Node *node = graph.getNode(curr->nodeIndex);
        if (node)
        {
            cout << "Step " << step << ": " << node->id << " (" << node->x << "," << node->y << ")";
            if (node->isAirport)
                cout << " [AIRPORT]";
            cout << endl;
            step++;
        }
        curr = curr->next;
    }

    avl.insert(Utils::getCurrentTimestamp(), id, "PATH_ASSIGNED", "Flight path assigned to nearest airport");
    graph.clearPath(path);
}

void printLog(AVL &avl)
{
    avl.inOrderPrint();
}

void printCrashedFlights(AVL &avl)
{
    int logArraySize = INITIAL_LOG_ARRAY_SIZE;
    LogEntry* logs = new LogEntry[logArraySize];
    int count = 0;
    avl.collectLogs(logs, count, logArraySize);
    
    if (count >= logArraySize) {
        delete[] logs;
        logArraySize = count * 2;
        logs = new LogEntry[logArraySize];
        avl.collectLogs(logs, count, logArraySize);
    }

    cout << endl << "=== Crashed Flights Log ===" << endl;
    cout << setw(15) << "Timestamp" 
         << setw(15) << "Flight ID"
         << setw(20) << "Status"
         << setw(30) << "Notes" << endl;
    cout << "------------------------------------------------------------" << endl;

    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (logs[i].status == "CRASHED")
        {
            cout << setw(15) << logs[i].timestamp 
                 << setw(15) << logs[i].flightId
                 << setw(20) << logs[i].status
                 << setw(30) << logs[i].notes << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No crashed flights found." << endl;
    }
    cout << endl;
    delete[] logs;
}

void printLandedFlights(AVL &avl)
{
    int logArraySize = INITIAL_LOG_ARRAY_SIZE;
    LogEntry* logs = new LogEntry[logArraySize];
    int count = 0;
    avl.collectLogs(logs, count, logArraySize);
    
    if (count >= logArraySize) {
        delete[] logs;
        logArraySize = count * 2;
        logs = new LogEntry[logArraySize];
        avl.collectLogs(logs, count, logArraySize);
    }

    cout << endl << "=== Landed Flights Log ===" << endl;
    cout << setw(15) << "Timestamp" 
         << setw(15) << "Flight ID"
         << setw(20) << "Status"
         << setw(30) << "Notes" << endl;
    cout << "------------------------------------------------------------" << endl;

    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (logs[i].status == "LANDED")
        {
            cout << setw(15) << logs[i].timestamp 
                 << setw(15) << logs[i].flightId
                 << setw(20) << logs[i].status
                 << setw(30) << logs[i].notes << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No landed flights found." << endl;
    }
    cout << endl;
    delete[] logs;
}

void resetSystem(Graph &graph, HashTable &hashTable, MinHeap &heap, AVL &avl)
{
    int flightArraySize = INITIAL_FLIGHT_ARRAY_SIZE;
    Flight** flights = new Flight*[flightArraySize];
    int count = 0;
    hashTable.getAllFlights(flights, count, flightArraySize);
    
    if (count >= flightArraySize) {
        delete[] flights;
        flights = new Flight*[count + 100]; // Add buffer
        count = 0;
        hashTable.getAllFlights(flights, count);
    }
    
    for (int i = 0; i < count; i++)
    {
        if (flights[i])
        {
            string id = flights[i]->id;
            hashTable.remove(id);
            delete flights[i];
        }
    }
    delete[] flights;

    while (!heap.isEmpty())
    {
        heap.extractMin();
    }

    avl.clear();

    int nodeCount = graph.getNodeCount();
    for (int i = 0; i < nodeCount; i++)
    {
        graph.setOccupied(i, false);
    }

    cout << "System reset successfully! All flights removed." << endl;
    cout << "Use menu option 7.4 to load demo data if needed." << endl;
}

void simulateStep(HashTable &hashTable, MinHeap &heap, AVL &avl, Graph &graph)
{
    int flightArraySize = INITIAL_FLIGHT_ARRAY_SIZE;
    Flight** flights = new Flight*[flightArraySize];
    int count = 0;
    hashTable.getAllFlights(flights, count, flightArraySize);
    
    if (count >= flightArraySize) {
        delete[] flights;
        flights = new Flight*[count + 100]; // Add buffer
        count = 0;
        hashTable.getAllFlights(flights, count);
    }

    for (int i = 0; i < count; i++)
    {
        Flight *flight = flights[i];
        if (flight && flight->inAir && !flight->crashed)
        {
            Utils::updateFuelAndCheckEmergency(flight, heap, avl, graph, hashTable);
            if (flight->inAir && !flight->crashed)
            {
                Utils::moveFlightTowardsAirport(flight, graph, hashTable, avl);
            }
        }
    }
    delete[] flights;
}

void addAirport(Graph &graph, Radar &radar)
{
    string id;
    int x, y;
    
    cout << "Enter Airport ID: ";
    cin >> id;
    
    cout << "Enter X coordinate (0-" << (GRID_COLS - 1) << "): ";
    cin >> x;
    cout << "Enter Y coordinate (0-" << (GRID_ROWS - 1) << "): ";
    cin >> y;
    
    if (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS)
    {
        cout << "Coordinates out of grid bounds!" << endl;
        return;
    }
    
    if (graph.findNodeIndex(id) != -1)
    {
        cout << "Node with ID \"" << id << "\" already exists!" << endl;
        return;
    }
    
    int nodeIndex = graph.addNode(id, x, y, true);
    cout << "Airport \"" << id << "\" added at (" << x << "," << y << ") with node index " << nodeIndex << endl;
}

void addWaypoint(Graph &graph, Radar &radar)
{
    string id;
    int x, y;
    
    cout << "Enter Waypoint/Node ID: ";
    cin >> id;
    
    cout << "Enter X coordinate (0-" << (GRID_COLS - 1) << "): ";
    cin >> x;
    cout << "Enter Y coordinate (0-" << (GRID_ROWS - 1) << "): ";
    cin >> y;
    
    if (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS)
    {
        cout << "Coordinates out of grid bounds!" << endl;
        return;
    }
    
    if (graph.findNodeIndex(id) != -1)
    {
        cout << "Node with ID \"" << id << "\" already exists!" << endl;
        return;
    }
    
    int nodeIndex = graph.addNode(id, x, y, false);
    cout << "Waypoint \"" << id << "\" added at (" << x << "," << y << ") with node index " << nodeIndex << endl;
}

void addFlightCorridor(Graph &graph)
{
    string fromId, toId;
    int weight;
    
    cout << "Enter source node ID: ";
    cin >> fromId;
    cout << "Enter destination node ID: ";
    cin >> toId;
    cout << "Enter edge weight (distance/cost): ";
    cin >> weight;
    
    if (weight < 0)
    {
        cout << "Weight must be non-negative!" << endl;
        return;
    }
    
    int fromIndex = graph.findNodeIndex(fromId);
    int toIndex = graph.findNodeIndex(toId);
    
    if (fromIndex == -1)
    {
        cout << "Source node \"" << fromId << "\" not found!" << endl;
        return;
    }
    
    if (toIndex == -1)
    {
        cout << "Destination node \"" << toId << "\" not found!" << endl;
        return;
    }
    
    if (fromIndex == toIndex)
    {
        cout << "Source and destination cannot be the same!" << endl;
        return;
    }
    
    graph.addEdge(fromIndex, toIndex, weight);
    cout << "Flight corridor added from \"" << fromId << "\" to \"" << toId << "\" with weight " << weight << endl;
}

void viewAllNodes(Graph &graph)
{
    int nodeCount = graph.getNodeCount();
    
    if (nodeCount == 0)
    {
        cout << "No nodes in the system." << endl;
        return;
    }
    
    cout << endl << "=== All Nodes ===" << endl;
    cout << setw(10) << "Index" 
         << setw(15) << "ID"
         << setw(10) << "X"
         << setw(10) << "Y"
         << setw(15) << "Type"
         << setw(10) << "Occupied" << endl;
    cout << "------------------------------------------------------------" << endl;
    
    for (int i = 0; i < nodeCount; i++)
    {
        Node* node = graph.getNode(i);
        if (node)
        {
            cout << setw(10) << i
                 << setw(15) << node->id
                 << setw(10) << node->x
                 << setw(10) << node->y
                 << setw(15) << (node->isAirport ? "Airport" : "Waypoint")
                 << setw(10) << (graph.isOccupied(i) ? "Yes" : "No") << endl;
        }
    }
    cout << endl;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    Graph graph;
    HashTable hashTable;
    MinHeap heap;
    AVL avl;
    Radar radar;

    radar.setGraph(&graph);

    cout << "Initializing SkyNet Air Traffic Control System..." << endl;
    cout << "System initialized. Use menu option 7.4 to load demo data, or 2.x to configure airspace." << endl;

    int choice;
    do
    {
        system("cls");
        showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            airspaceVisualizer(radar, graph, hashTable);
            system("pause");
            break;
        case 2:
        {
            int subChoice;
            cout << "Airspace Configuration:" << endl;
            cout << "  1) Add Airport" << endl;
            cout << "  2) Add Waypoint/Node" << endl;
            cout << "  3) Add Flight Corridor (Edge)" << endl;
            cout << "  4) View All Nodes" << endl;
            cout << "Choice: ";
            cin >> subChoice;
            if (subChoice == 1)
                addAirport(graph, radar);
            else if (subChoice == 2)
                addWaypoint(graph, radar);
            else if (subChoice == 3)
                addFlightCorridor(graph);
            else if (subChoice == 4)
                viewAllNodes(graph);
            else
                cout << "Invalid submenu choice!" << endl;
            system("pause");
            break;
        }
        case 3:
        {
            int subChoice;
            cout << "Flight Control:" << endl;
            cout << "  1) Add Flight" << endl;
            cout << "  2) Move Plane" << endl;
            cout << "  3) Declare Emergency" << endl;
            cout << "  4) Land Flight" << endl;
            cout << "Choice: ";
            cin >> subChoice;
            if (subChoice == 1)
                addFlight(graph, hashTable, heap, avl);
            else if (subChoice == 2)
                movePlane(hashTable, heap, avl, graph);
            else if (subChoice == 3)
                declareEmergency(hashTable, heap, avl);
            else if (subChoice == 4)
                landFlight(hashTable, heap, avl, graph);
            else
                cout << "Invalid submenu choice!" << endl;
            system("pause");
            break;
        }
        case 4:
        {
            int subChoice;
            cout << "Flight Lookup & Management:" << endl;
            cout << "  1) Search Flight by ID" << endl;
            cout << "  2) Update Flight Info" << endl;
            cout << "Choice: ";
            cin >> subChoice;
            if (subChoice == 1)
                searchFlight(hashTable);
            else if (subChoice == 2)
                updateFlightInfo(hashTable, heap, avl);
            else
                cout << "Invalid submenu choice!" << endl;
            system("pause");
            break;
        }
        case 5:
        {
            int subChoice;
            cout << "Route & Path Management:" << endl;
            cout << "  1) Find Safe Route" << endl;
            cout << "  2) Assign Flight Path" << endl;
            cout << "Choice: ";
            cin >> subChoice;
            if (subChoice == 1)
                findSafeRoute(graph, hashTable);
            else if (subChoice == 2)
                assignFlightPath(graph, hashTable, avl);
            else
                cout << "Invalid submenu choice!" << endl;
            system("pause");
            break;
        }
        case 6:
        {
            int subChoice;
            cout << "Flight History & Logs:" << endl;
            cout << "  1) Print Full Log" << endl;
            cout << "  2) Print Crashed Flights Only" << endl;
            cout << "  3) Print Landed Flights Only" << endl;
            cout << "Choice: ";
            cin >> subChoice;
            if (subChoice == 1)
                printLog(avl);
            else if (subChoice == 2)
                printCrashedFlights(avl);
            else if (subChoice == 3)
                printLandedFlights(avl);
            else
                cout << "Invalid submenu choice!" << endl;
            system("pause");
            break;
        }
        case 7:
        {
            int subChoice;
            cout << "System Management:" << endl;
            cout << "  1) Save State" << endl;
            cout << "  2) Load State" << endl;
            cout << "  3) Reset System" << endl;
            cout << "  4) Load Demo Data" << endl;
            cout << "Choice: ";
            cin >> subChoice;
            if (subChoice == 1)
                IO::saveState(hashTable, avl);
            else if (subChoice == 2)
                IO::loadState(hashTable, heap, avl, graph, radar);
            else if (subChoice == 3)
                resetSystem(graph, hashTable, heap, avl);
            else if (subChoice == 4)
                loadDemoData(graph, hashTable, heap, avl);
            else
                cout << "Invalid submenu choice!" << endl;
            system("pause");
            break;
        }
        case 0:
            cout << "Exiting SkyNet..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
            system("pause");
        }

        if (choice != 0)
        {
            simulateStep(hashTable, heap, avl, graph);
        }

    } while (choice != 0);

    int flightArraySize = INITIAL_FLIGHT_ARRAY_SIZE;
    Flight** flights = new Flight*[flightArraySize];
    int count = 0;
    hashTable.getAllFlights(flights, count, flightArraySize);
    
    if (count >= flightArraySize) {
        delete[] flights;
        flights = new Flight*[count + 100]; // Add buffer
        count = 0;
        hashTable.getAllFlights(flights, count);
    }
    
    for (int i = 0; i < count; i++)
    {
        delete flights[i];
    }
    delete[] flights;

    return 0;
}
