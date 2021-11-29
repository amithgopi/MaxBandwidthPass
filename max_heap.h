#ifndef _MAX_HEAP_H
#define _MAX_HEAP_H

#include<iostream>
#include "graph.h"
#include "heap.h"
using namespace std;


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
            //For existing nodes, add it to same position and fix heap at that posistion
            if(position[node] != -1){
                fixHeap(position[node]);
                return;
            }

            arr[size] = node;
            position[node] = size;
            size++;

            heapifyUp(size - 1);

        }

        /**
         * @brief Heapify an element up the heap
         * 
         * @param pos 
         */
        void heapifyUp(int pos) {
            while (pos != 0 && bw[arr[parent(pos)]] < bw[arr[pos]]) //Use BW array for comparison
            {
                swap(&position[arr[pos]], &position[arr[parent(pos)]]);
                swap(&arr[pos], &arr[parent(pos)]);
                
                pos = parent(pos);
            }
        }

        /**
         * @brief fixes the heap at the specified position
         * 
         * @param pos 
         */
        void fixHeap(int pos) {
            if(pos == 0 || bw[arr[parent(pos)]] > bw[arr[pos]]) {
                heapify(pos);
            } else {
                heapifyUp(pos);
            }
        }

        /**
         * @brief Removes an element from the heap at the specified posistion/index
         * 
         * @param index 
         */
        void remove(int index) {
            if(index>size-1 || size == 0) {
                cout<<"Cannot delete. i = "<<index<<" node = "<<position[arr[index]]<<", size = "<<size<<endl;
                return;
            }
            size--; //Reduce size of heap
            
            position[arr[index]] = -1;  //Set posistion of element to be removed as -1
            if(index != size) position[arr[size]] = index;  //handle edge case when node to delete is the last node

            arr[index] = arr[size]; //Replace element with right most leaf (last in heap array) 
            arr[size] = -1;     //Change last replaecd elemetn (not at the end of the array) to -1

            fixHeap(index);         //Fix the heap at the deleted location
            
        }

        /**
         * @brief Delete a given node/vertex id from the heap
         * 
         * @param node 
         */
        void deleteNode(int node) {
            remove(position[node]);
        }

        /**
         * @brief Get the BW of the node with the given id
         * 
         * @param i 
         * @return int BW of ith node
         */
        inline int getHeapNodeValue(int i) {
            return bw[arr[i]];
        }
};


#endif
