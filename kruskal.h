#include "min_heap.h"
#include <iostream>
#include <chrono>

using namespace std;


/**
 * @brief Class to hold an instance of kruskal run gor a given graph
 * 
 */
class Kruskal {
    public:

    //Enum to color nodes during DFS
    enum Color {WHITE, GREY, BLACK};

    int* parentOfSet;   //Used for find set
    int* parent;        //Used to trace shortest path on DFS
    int* height;        //Stores rank or heaight for union find
    int vertexCount;
    int edgeCount;

    //Queue for path compression in find
    int* queue;
    int currentQueueSize;

    MinHeap* edgeHeap;

    //New graph to store the maximum spanning tree
    Graph* MST;
    int maxBw;

    /**
     * @brief Initialize members based on the given graph
     * 
     * @param G 
     */
    void init(Graph &G) {
        vertexCount = G.num_nodes;
        edgeCount = G.total_edge_count;
        parentOfSet = new int[vertexCount];
        parent = new int[vertexCount];
        height = new int[vertexCount];
        edgeHeap = new MinHeap(G);

        queue = new int[vertexCount];
        currentQueueSize = 0;

        MST = new Graph(vertexCount);

        //Set initial vales for the arrays - makeset
        for(int i=0; i<vertexCount; i++) {
            parentOfSet[i] = -1;
            height[i] = 0;
            parent[i] = -1;
        }
    }

    ~Kruskal() {
        cleanUp();
    }

    /**
     * @brief Clean up and deallocate momory
     * 
     */
    void cleanUp() {
        delete MST;
        delete edgeHeap;
        delete[] parentOfSet;
        delete[] parent;
        delete[] height;
    }

    /**
     * @brief Implements the union find operation
     * 
     * @param v 
     * @param w 
     */
    void unionSet(int v, int w) {
        if(height[v] > height[w]) parentOfSet[w] = v;
        else if(height[v] < height[w]) parentOfSet[v] = w;
        else {
            parentOfSet[v] = w;
            height[w]++;
        }
    }

    /**
     * @brief Implemetns find operation on the set with path compression
     * 
     * @param v id of node whose root is to be found
     * @return int root of the tree to which the given node belongs to
     */
    int find(int v) {
        int w = v;
        currentQueueSize = 0;
        while(parentOfSet[w] != -1) {
            queue[currentQueueSize] = w;
            currentQueueSize++;
            w = parentOfSet[w];
        }
        while(currentQueueSize > 0) {
            currentQueueSize--;
            parentOfSet[queue[currentQueueSize]] = w;
        
        }
        return w;
    }

    /**
     * @brief Modified DFS to get shortest path from the MST. This stops when vertex t is encounteres
     * This also creates the parent array and computes the BW of the s to t path
     * 
     * @param G Graph input to run DFS on
     * @param r root node
     * @param t end node
     * @param color color array to indicate the status of a vertex
     * @return true if t was found in the current vertex or its decendants
     * @return false if t was not found
     */
    bool DFS(Graph *G, int r, int t, Color* color) {
        color[r] = GREY;
        Node* current = G->list[r]->next;
        while(current != nullptr) {
            if(current->id == t) {
                color[t] = GREY;
                maxBw = min(current->weight, maxBw);
                parent[t] = r;
                return true;
            }
            if(color[current->id] == WHITE) {
                if(DFS(G, current->id, t, color) == true) {
                    maxBw = min(current->weight, maxBw);
                    parent[current->id] = r;
                    return true;
                } 
            }
            
            current = current->next;
        }
        return false;
    }

    /**
     * @brief Print path from s to t recursively by tracing parent array from t
     * 
     * @param x 
     */
    void printPath(int x) {
        if(x == -1) {
            return;
        }
        printPath(parent[x]);
        cout<<x<<" -> ";
    }

    /**
     * @brief Kruskal algorithm implementation
     * 
     * @param G Graph on which to run Kruskal
     * @param s start node id
     * @param t end node id
     * @return int* pointer to parent array
     */
    int* kruskal(Graph &G, int s, int t) {
        init(G);
        int r1, r2;
        Node* edge;
        auto begin = std::chrono::high_resolution_clock::now();
        edgeHeap->heapSort();
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        cout<<"Sort - Time taken: "<<elapsed.count()<<endl;
        for(int i=0; i<edgeCount; i++) {
            edge = edgeHeap->arr[i];
            r1 = find(edge->parentId);
            r2 = find(edge->id);
            if(r1 != r2) {
                //TODO should this resuse exising node object??
                MST->addEdge(edge->parentId, edge->id, edge->weight);
                MST->addEdge(edge->id, edge->parentId, edge->weight);
                unionSet(r1, r2);
            }
        }
        //MST->printGraph();
        maxBw = INT32_MAX;
        Color color[vertexCount];
        fill_n(color, vertexCount, WHITE);
        DFS(MST, s, t, color);

        if(color[t] == WHITE) {
            cout<<"No s to t path found"<<endl;
            return nullptr;
        } else if(s == t) {
            cout<<"Start and end are the same - "<<s<<", BW is INFINITE"<<endl;
            maxBw = INT32_MAX;
            return nullptr;
        } else {
            cout<<"Max BW of pass = "<<maxBw<<endl;
            // printPath(t);
            return parent;
        }

    }

    /**
     * @brief Get the Max B W object
     * 
     * @return int returns max BW of the pass after kruskal
     */
    int getMaxBW() {
        return maxBw;
    }

};



