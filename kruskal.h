#include "min_heap.h"
#include <iostream>


using namespace std;

enum Color {WHITE, GREY, BLACK};

int* parentOfSet;
int* parentInPass;
int* height;
int vertexCount;
int edgeCount;

int* queue;
int currentQueueSize;

MinHeap* edgeHeap;

Graph* MST;
int maxBw;

void init(Graph &G) {
    vertexCount = G.num_nodes;
    edgeCount = G.total_edge_count;
    parentOfSet = new int[vertexCount];
    parentInPass = new int[vertexCount];
    height = new int[vertexCount];
    edgeHeap = new MinHeap(G);

    queue = new int[vertexCount];
    currentQueueSize = 0;

    MST = new Graph(vertexCount);

    for(int i=0; i<vertexCount; i++) {
        parentOfSet[i] = -1;
        height[i] = 0;
        parentInPass[i] = -1;
    }
}

void cleanUp() {
    MST->~Graph();
    delete MST;
    delete edgeHeap;
    delete[] parentOfSet;
    delete[] parentInPass;
    delete[] height;
}

void unionSet(int v, int w) {
    if(height[v] > height[w]) parentOfSet[w] = v;
    else if(height[v] < height[w]) parentOfSet[v] = w;
    else {
        parentOfSet[v] = w;
        height[w]++;
    }
}

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

bool DFS(Graph *G, int r, int t, Color* color) {
    color[r] = GREY;
    Node* current = G->list[r]->next;
    while(current != nullptr) {
        if(current->id == t) {
            color[t] = GREY;
            maxBw = min(current->weight, maxBw);
            parentInPass[t] = r;
            return true;
        }
        if(color[current->id] == WHITE) {
            if(DFS(G, current->id, t, color) == true) {
                maxBw = min(current->weight, maxBw);
                parentInPass[current->id] = r;
                return true;
            } 
        }
           
        current = current->next;
    }
    return false;
}

void printPathK(int x) {
    if(x == -1) {
        return;
    }
    printPath(parentInPass[x]);
    cout<<x<<" -> ";
}

int* kruskal(Graph &G, int s, int t) {
    init(G);
    int r1, r2;
    Node* edge;
    edgeHeap->heapSort();
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
        return nullptr;
    } else {
        cout<<"Max BW of pass = "<<maxBw<<endl;
        printPathK(t);
        return parentInPass;
    }

}

