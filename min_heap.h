#ifndef _MIN_HEAP_H
#define _MIN_HEAP_H

#include<iostream>
#include "graph.h"
#include "heap.h"
using namespace std;

#define parent(i) (i-1)>>1
#define left(i) ((i<<1) + 1)
#define right(i) ((i<<1) + 2)

/**
 * @brief Implements min heap as the derived class on Heap of type Node*
 * 
 */
class MinHeap: public Heap<Node*>{
    public:
        /**
         * @brief Construct a new Min Heap object and initialize its paremters.
         * This also fills the heap with edges from the given graph and builds it. 
         * 
         * @param G 
         */
        MinHeap(Graph &G): Heap(G) {
            capacity = graph->total_edge_count + 1;
            arr = new Node*[capacity];

            fillHeap();
            buildHeap();
        }

        ~MinHeap() {
            delete[] arr;
        }

        /**
         * @brief Fill heap with edges from the graph
         * 
         */
        void fillHeap() {
            int numNodes = graph->num_nodes;
            for(int i=0; i<numNodes; i++) {
                Node* currNode = graph->list[i]->next;  
                while(currNode != nullptr) {
                    arr[size] = currNode;
                    size++;
                    currNode = currNode->next;
                } 
            }
        }

        /**
         * @brief Min heapify operation to heapify down the array
         * 
         * @param i position of element from which to start the heapify operation
         */
        void heapify(int i) {
            int l = left(i), r = right(i);
            int smallest = i;

            //Use BW array for comparison
            if(l < size && arr[l]->weight < arr[smallest]->weight) smallest = l;
            if(r < size && arr[r]->weight < arr[smallest]->weight) smallest = r;

            if(smallest != i) {
                swap(&arr[i], &arr[smallest]);
                heapify(smallest);
            }
        }

        inline int getHeapNodeValue(int i) {
            return arr[i]->weight;
        }

};

#endif
