#ifndef _MAX_HEAP_H
#define _MAX_HEAP_H

#include<iostream>
#include "graph.h"
#include "heap.h"
using namespace std;

#define parent(i) (i-1)>>1
#define left(i) ((i<<1) + 1)
#define right(i) ((i<<1) + 2)

class MaxHeap: public Heap<int> {

    int* position;
    int* bw;

    public:
        /**
         * @brief Construct a new Max Heap object
         * 
         * @param c Max capacity of the heap
         */
        MaxHeap(Graph* _graph, int* _bw): Heap(*_graph) {
            bw = _bw;
            capacity = graph->num_nodes;
            
            arr = new int[capacity];
            fill_n(arr, capacity, -1);
            position = new int[capacity];
            fill_n(position, capacity, -1);
        }

        ~MaxHeap() {
            delete[] arr;
            delete[] position;
        }

        int get_node(int i){
            return arr[i];
        }

        /**
         * @brief Calls max hepify to fix heap at the given position - moves the element at intex i
         * to its correct position in the heap array
         * 
         * @param i index of item to be moved
         */
        void heapify(int i) {
            int l = left(i), r = right(i);
            int largest = i;

            //Use BW array for comparison
            if(l < size && bw[arr[l]] > bw[arr[largest]]) largest = l;
            if(r < size && bw[arr[r]] > bw[arr[largest]]) largest = r;

            if(largest != i) {
                swap(&position[arr[i]], &position[arr[largest]]);
                swap(&arr[i], &arr[largest]);
                
                heapify(largest);
            }
        }

        /**
         * @brief Insert the given elemnent at the correct position in the heap
         * 
         * @param node Value of element to be inserted
         */
        void insert(int node) {
            if (size >= capacity) {
                cout<<"Cannot insert new element. Capacity full.\n";
                return;
            }
            //For existing nodes, add it to same position and heapify
            if(position[node] != -1){
                heapify(position[node]);
                return;
            }

            arr[size] = node;
            position[node] = size;
            size++;

            int pos = size - 1;
            while (pos != 0 && bw[arr[parent(pos)]] < bw[arr[pos]]) //Use BW array for comparison
            {
                swap(&position[arr[pos]], &position[arr[parent(pos)]]);
                swap(&arr[pos], &arr[parent(pos)]);
                
                pos = parent(pos);
            }
        }

        void remove(int i) {
            // printHeap(0);
            if(i>size-1 || size == 0) {
                cout<<"Cannot delete. i = "<<i<<" node = "<<position[arr[i]]<<", size = "<<size<<endl;
                return;
            }

            size--;
            
            position[arr[i]] = -1;  
            if(i != size) position[arr[size]] = i;  //handle edge case when node to delete is the last node

            arr[i] = arr[size]; 
            arr[size] = -1;
            
            for (int i=(size/2)-1; i>=0; i--) {
                heapify(i);
            }
        }

        void deleteNode(int node) {
            remove(position[node]);
        }

        inline int getHeapNodeValue(int i) {
            return bw[arr[i]];
        }
};


#endif
