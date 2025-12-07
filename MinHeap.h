#ifndef MINHEAP_H
#define MINHEAP_H

#include "Flight.h"

class MinHeap {
private:
    Flight** heap;
    int size;
    int capacity;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    void swap(int i, int j);
    bool isHigherPriority(Flight* a, Flight* b);
    
public:
    MinHeap();
    ~MinHeap();
    
    void insert(Flight* flight);
    Flight* extractMin();
    Flight* peek();
    void decreaseKey(int heapIndex, int newPriority);
    void removeAt(int heapIndex);
    bool isEmpty() const;
    int getSize() const;
    Flight* getFlight(int index);
};

#endif

