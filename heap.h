#ifndef _HEAP_H
#define _HEAP_H
#include<iostream>
#include "graph.h"

using namespace std;

/**
 * @brief Parent class for heaps used
 * 
 * @tparam T type of data to be sored in the heap
 */
template <class T>
class Heap {
    public:
        T* arr;
        int size;
        int capacity;

        Graph* graph;

        virtual void heapify(int i) = 0;
        virtual int getHeapNodeValue(int i) = 0;

        /**
         * @brief Swap two entries in the heap
         * 
         * @param a pointer to first element
         * @param b pointer to second element
         */
        void swap(T *a, T *b) {
            T temp = *a;
            *a = *b;
            *b = temp;
        }

        Heap(Graph &G) {
            graph = &G;
            size = 0;
        }

        /**
         * @brief Build a heap from the current elements in the array arr
         * 
         */
        void buildHeap() {
            int start = (size/2) - 1;
            for(int i =  start; i >= 0; i--) {
                heapify(i);
            }
        }

        /**
         * @brief Print the values of each element in the heap array. This prints out the vales of an element which gets
         * compared on heapify operations.
         * 
         */
        void printHeap() {
            for(int i=0; i<size; i++) {
                cout<<getHeapNodeValue(i)<<", ";
            }
            cout<<endl;
        }

        /**
         * @brief Sorts the elements in the heap array based on the implmentation of heapify
         * 
         */
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