#ifndef _EDGE_HEAP_H
#define _EDGE_HEAP_H

#include<iostream>
#include "graph.h"
using namespace std;

#define parent(i) (i-1)>>1
#define left(i) ((i<<1) + 1)
#define right(i) ((i<<1) + 2)


class EdgeHeap{
    public:

        Node** arr;
        int capacity;
        int size;
        Graph* graph;

        EdgeHeap(Graph &G) {
            graph = &G;
            int numNodes = graph->num_nodes;
            capacity = graph->total_edge_count + 1;
            size = 0;
            arr = new Node*[capacity];

            fillHeap();
            buildHeap();
        }

        void swap(Node* *a, Node* *b) {
            Node* temp = *a;
            *a = *b;
            *b = temp;
        }

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

        void heapify(int i) {
            int l = left(i), r = right(i);
            int largest = i;

            //Use BW array for comparison
            if(l < size && arr[l]->weight > arr[largest]->weight) largest = l;
            if(r < size && arr[r]->weight > arr[largest]->weight) largest = r;

            if(largest != i) {
                swap(&arr[i], &arr[largest]);
                heapify(largest);
            }
        }

        void buildHeap() {
            int start = (size/2) - 1;
            for(int i =  start; i >= 0; i--) {
                heapify(i);
            }
        }

        void heapSort() {
            int actualSize = size;
            for(int i=0; i<actualSize; i++){
                size--;
                swap(&arr[0], &arr[size]);
                heapify(0);
            }
            size = actualSize;
        }

        void printHeap() {
            for(int i=0; i<size; i++) {
                cout<<arr[i]->weight<<", ";
            }
            cout<<endl;
        }


};

#endif
