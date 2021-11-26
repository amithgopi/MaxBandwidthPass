#ifndef _HEAP_H
#define _HEAP_H
#include<iostream>
#include "graph.h"

using namespace std;

template <class T>
class Heap {
    public:
        T* arr;
        int size;
        int capacity;

        Graph* graph;

        virtual void heapify(int i) = 0;
        virtual int getHeapNodeValue(int i) = 0;

        void swap(T *a, T *b) {
            T temp = *a;
            *a = *b;
            *b = temp;
        }

        Heap(Graph &G) {
            graph = &G;
            size = 0;
        }

        void buildHeap() {
            int start = (size/2) - 1;
            for(int i =  start; i >= 0; i--) {
                heapify(i);
            }
        }

        void printHeap() {
            for(int i=0; i<size; i++) {
                cout<<getHeapNodeValue(i)<<", ";
            }
            cout<<endl;
        }

        void heapSort() {
            int actualSize = size;
            for(int i=0; i<actualSize; i++){
                size--;
                swap(&arr[0], &arr[size]);
                heapify(0);
            }
            size = actualSize;
            // printHeap();
        }
};

#endif