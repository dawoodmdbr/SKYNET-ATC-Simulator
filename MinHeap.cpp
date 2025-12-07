#include "MinHeap.h"
#include "Config.h"
#include <climits>
#include <ctime>

MinHeap::MinHeap() : size(0), capacity(MIN_HEAP_INITIAL_CAPACITY) {
    heap = new Flight*[capacity];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

bool MinHeap::isHigherPriority(Flight* a, Flight* b) {
    if (a->priority < b->priority) return true;
    if (a->priority > b->priority) return false;
    
    if (a->fuel_percent < b->fuel_percent) return true;
    if (a->fuel_percent > b->fuel_percent) return false;
    
    if (a->timestamp_landed < b->timestamp_landed) return true;
    return false;
}

void MinHeap::swap(int i, int j) {
    Flight* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
    
    heap[i]->heapIndex = i;
    heap[j]->heapIndex = j;
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (isHigherPriority(heap[index], heap[parent])) {
            swap(index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        
        if (left < size && isHigherPriority(heap[left], heap[smallest])) {
            smallest = left;
        }
        if (right < size && isHigherPriority(heap[right], heap[smallest])) {
            smallest = right;
        }
        
        if (smallest != index) {
            swap(index, smallest);
            index = smallest;
        } else {
            break;
        }
    }
}

void MinHeap::insert(Flight* flight) {
    if (size >= capacity) {
        int newCapacity = capacity * 2;
        Flight** newHeap = new Flight*[newCapacity];
        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }
    
    heap[size] = flight;
    flight->heapIndex = size;
    heapifyUp(size);
    size++;
}

Flight* MinHeap::extractMin() {
    if (size == 0) return nullptr;
    
    Flight* min = heap[0];
    swap(0, size - 1);
    size--;
    if (size > 0) {
        heapifyDown(0);
    }
    min->heapIndex = -1;
    return min;
}

Flight* MinHeap::peek() {
    if (size == 0) return nullptr;
    return heap[0];
}

void MinHeap::decreaseKey(int heapIndex, int newPriority) {
    if (heapIndex < 0 || heapIndex >= size) return;
    
    Flight* flight = heap[heapIndex];
    int oldPriority = flight->priority;
    flight->priority = newPriority;
    
    if (newPriority < oldPriority) {
        heapifyUp(heapIndex);
    } else if (newPriority > oldPriority) {
        heapifyDown(heapIndex);
    }
}

void MinHeap::removeAt(int heapIndex) {
    if (heapIndex < 0 || heapIndex >= size) return;
    
    swap(heapIndex, size - 1);
    size--;
    if (heapIndex < size) {
        heapifyDown(heapIndex);
    }
    heap[size]->heapIndex = -1;
}

bool MinHeap::isEmpty() const {
    return size == 0;
}

int MinHeap::getSize() const {
    return size;
}

Flight* MinHeap::getFlight(int index) {
    if (index < 0 || index >= size) return nullptr;
    return heap[index];
}

