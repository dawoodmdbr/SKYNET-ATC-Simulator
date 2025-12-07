#ifndef CONFIG_H
#define CONFIG_H

const int GRID_ROWS = 20;
const int GRID_COLS = 20;

const int GRAPH_INITIAL_CAPACITY = 10;
const int HASH_TABLE_DEFAULT_SIZE = 101;
const int MIN_HEAP_INITIAL_CAPACITY = 100;

const int FUEL_COST_PER_MOVE = 5;
const int LOW_FUEL_THRESHOLD = 10;
const int AUTO_PRIORITY_THRESHOLD = 2;
const int FUEL_DECREASE_MIN = 2;
const int FUEL_DECREASE_MAX = 4;
const int DEFAULT_FUEL_PERCENT = 100;
const int DEFAULT_PRIORITY = 3;

const int PRIORITY_EMERGENCY = 1;
const int PRIORITY_HIGH = 2;
const int PRIORITY_NORMAL = 3;

const int INITIAL_FLIGHT_ARRAY_SIZE = 100;
const int INITIAL_LOG_ARRAY_SIZE = 1000;

const char GRID_SYMBOL_EMPTY = '.';
const char GRID_SYMBOL_AIRPORT = 'A';
const char GRID_SYMBOL_PLANE = 'P';

#endif

