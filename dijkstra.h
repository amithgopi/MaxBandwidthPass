#include "graph.h"
#include "max_heap.h"
#include <iostream>

using namespace std;

/**
 * @brief Class to hold the implmentatin fo Dijkstra's algorithm and any variables and members required for its operation
 * 
 */
 class Dijkstra {
    public:

    enum Status {IN_TREE, FRINGE, UNSEEN};

    int* bw;            //Array to hold bandwidth of each vertex
    Status* status;     //Contains status of each node - picked fron the Status enum
    int* parent;        //Parent array to trace the path back to start
    int numNodes;       //No of veritces in the given graph
    MaxHeap* heap;      //Points to an instance of Max heap to store fringes

    int maxBw;

    /**
     * @brief Initializes parameters for the algorithm for the given graph and allocates
     * the required memory for arrays based on these parameters.
     * 
     * @param graph 
     * @param useHeap 
     */
    void initParam(Graph &graph, bool useHeap) {
        numNodes = graph.num_nodes;
        bw = new int[numNodes];
        status = new Status[numNodes];
        parent = new int[numNodes];

        for(int i=0; i<numNodes; i++) {
            status[i] = UNSEEN;
            parent[i] = -1;
            bw[i] = INT32_MIN;  //TODO Verify this??
        }
        //Initialize Heap
        if(useHeap) heap = new MaxHeap(&graph, bw);
    }

    ~Dijkstra() {
        cleanUpPathParams();
    }

    /**
     * @brief Use to clear aloocated array memory after each run
     * 
     */
    void cleanUpPathParams() {
        delete(status);
        delete(parent);
        delete(bw);
    }

    /**
     * @brief Output the shortet path from start to the given node t, by recursively calling the parent array
     * 
     * @param t end node id of the shortest path
     */
    void printPathReverse(int t) {
        int x = t;
        int count=0;
        while(x!=-1 && count < 2*numNodes) {
            cout<<x<<" -> ";
            x = parent[x];
            count++;
        }
        cout<<endl;
    }

    /**
     * @brief Prints the shortest path in reverse by tracing the parent array
     * 
     * @param x end node id of the shortest path
     */
    void printPath(int x) {
        if(x == -1) {
            return;
        }
        printPath(parent[x]);
        cout<<x<<"("<<bw[x]<<") -> ";
    }

    /**
     * @brief Find fringe node with largest BW by looking thourgh the status array and comparing with 
     * the BW array.
     * 
     * @return int fringe with largest BW
     */
    int findLargetFringe() {
        int largestBW = INT32_MIN, largestFringe = -1;
        for(int i=0; i<numNodes; i++) {
            if(status[i] == FRINGE) {
                if(bw[i] > largestBW) {
                    largestBW = bw[i];
                    largestFringe = i;
                }
            }
        }
        return largestFringe;
    }

    /**
     * @brief Find the fringe node with the largest BW from the heap
     * This is done by picking the element at index 0 (root) of the MaxHeap
     * 
     * @return int fringe with largest BW
     */
    int findLargetFringeWithHeap() {
        return heap->get_node(0);
    }

    int* dijkstra(Graph &G, int s, int t, bool useHeap = false) {
        //Initialize arrays for the given graph
        initParam(G, useHeap);
        //Set status of s to in-tree
        status[s] = IN_TREE;

        Node* currNode = G.list[s]->next;

        //Add each [s,v] edge vertex as fringe and update BW
        while(currNode != nullptr) {
            // Node* next = currNode->next;
            status[currNode->id] = FRINGE;
            parent[currNode->id] = s;
            //Update bw only if current BW is lower than new one, helps with cases where there are more than one edge to a node
            if(bw[currNode->id] < currNode->weight) bw[currNode->id] = currNode->weight;
            //Insert to heap if useHeap set
            if(useHeap)  heap->insert(currNode->id);
            //Iterate to next edge/vertex
            currNode = currNode->next;
        }

        //Select function to get largest fringe node
        int fringeVertex = useHeap ? findLargetFringeWithHeap() : findLargetFringe();
        //TODO Stop when t is in tree ??
        while(fringeVertex != -1) {
            status[fringeVertex] = IN_TREE;
            if(useHeap) heap->deleteNode(fringeVertex); //Delete node from heap when its in-tree

            //Look at each edge of the fringe and grow the tree
            currNode = G.list[fringeVertex]->next;

            while(currNode != nullptr) {
                int currentVertexId = currNode->id;
                //Calculate new BW for the current edge if we connect it to the current fringe
                int newBW = min(bw[fringeVertex], currNode->weight);

                //Check if current node is unseen, make it fringe and update BW
                if(status[currentVertexId] == UNSEEN) {
                    status[currentVertexId] = FRINGE;
                    parent[currentVertexId] = fringeVertex;
                    bw[currentVertexId] = newBW;
                    if(useHeap) heap->insert(currentVertexId);
                //If current node is a fringe, update BW if current BW is lower than new one
                } else if(status[currentVertexId] == FRINGE && bw[currentVertexId] <newBW) {
                    if(useHeap) heap->deleteNode(currentVertexId);  //Remove node from heap to update BW
                    parent[currentVertexId] = fringeVertex;
                    bw[currentVertexId] = newBW;
                    if(useHeap) heap->insert(currentVertexId);      //Add node back to heap after the BW update
                }
                //Iterate to next edge of the current fringe node
                currNode = currNode->next;
            }
            //Get ehe next largest fringe
            fringeVertex = useHeap ? findLargetFringeWithHeap() : findLargetFringe();
        }
        
        if(status[t] != IN_TREE) {
            cout<<"No path from "<<s<<" to "<<t<<" in graph";
            return nullptr;
        } else if(s == t) {
            cout<<"Start and end are the same - "<<s<<", BW is INFINITE"<<endl;
            maxBw = INT32_MAX;
            return nullptr;
        } else {
            maxBw = bw[t];
            cout<<"Max BW of pass = "<<bw[t]<<endl;
            return parent;
        }

    }

    int getMaxBW() {
        return maxBw;
    }

};






