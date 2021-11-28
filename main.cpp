#include <iostream>
#include <chrono>
#include <fstream>
#include "graph.h"
#include "dijkstra.h"
#include "kruskal.h"

#define CSV_FILE_SEPERATOR ","
#define GRAPH_NUMBER_OF_VERTICES 5000
#define SPARSE_EDGES_PER_NODE 6
#define DENSE_EDGES_PER_NODE 1000
#define MAX_POSSIBLE_BW_VALUE 10000
#define ITERATIONS_PER_GRAPH 5

using namespace std;

void sparseGraph();
void denseGraph();
void testFromFile();

int main() {
    
    sparseGraph();
    denseGraph();

    // testFromFile();
    
    return 0;

}

void runAlgorithms(Graph &g, ofstream &file, int timesToExecute, bool printPath = false) {
    int maxBW[3];
    for(int i=0; i<timesToExecute; i++) {
        
        Dijkstra* d;
        int s = g.getRandomNode(), t = g.getRandomNode();
        cout<<"\nIteration "<<i+1<<" from "<<s<<" to "<<t<<endl;
        file<<i+1<<CSV_FILE_SEPERATOR<<s<<CSV_FILE_SEPERATOR<<t<<CSV_FILE_SEPERATOR;

        //Normal Dijkstra's algorithm
        d = new Dijkstra();
        auto begin = std::chrono::high_resolution_clock::now();
        d->dijkstra(g, s, t);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        if(printPath) { d->printPath(t); cout<<endl;}
        cout<<"Dijkstra - Time taken: "<<elapsed.count()<<endl;
        file<<elapsed.count()<<CSV_FILE_SEPERATOR;
        maxBW[0] = d->getMaxBW();
        delete d;

        //Dijkstra's algorithm using heap to find max fringe
        d = new Dijkstra();
        begin = std::chrono::high_resolution_clock::now();
        d->dijkstra(g, s, t, true);
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        if(printPath) { d->printPath(t); cout<<endl;}
        cout<<"Dijkstra Heap - Time taken: "<<elapsed.count()<<endl;
        file<<elapsed.count()<<CSV_FILE_SEPERATOR;
        maxBW[1] = d->getMaxBW();
        delete d;

        //Kruskal MST
        Kruskal* k = new Kruskal();
        begin = std::chrono::high_resolution_clock::now();
        k->kruskal(g,s, t);
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        if(printPath) { k->printPath(t); cout<<endl;}
        cout<<"Kruskal - Time taken: "<<elapsed.count()<<endl;
        file<<elapsed.count()<<CSV_FILE_SEPERATOR;
        maxBW[2] = d->getMaxBW();
        delete k;

        if(maxBW[0] == maxBW[1] && maxBW[1] == maxBW[2]) {
            file<<maxBW[0]<<CSV_FILE_SEPERATOR<<"\n";
        } else {
            g.dumpToFile();
            cout<<"\nMax BWs are not the same.\n";
            exit(1);
        }
        
    }
}

void sparseGraph() {
    cout<<"\n----STARTING RUN ON SPARSE GRAPH----"<<endl;
    ofstream file;
    file.open("sparse.csv");
    Graph g(GRAPH_NUMBER_OF_VERTICES, SPARSE_EDGES_PER_NODE, MAX_POSSIBLE_BW_VALUE);
    file<<"run,start,end,time_dijkstra,time_dijkstra_heap,time_kruskal,max_bandwidth\n";
    runAlgorithms(g, file, ITERATIONS_PER_GRAPH);  
    file.close();
}

void denseGraph() {
    cout<<"\n----STARTING RUN ON DENSE GRAPH----"<<endl;
    ofstream file;
    file.open("dense.csv");
    Graph g(GRAPH_NUMBER_OF_VERTICES, DENSE_EDGES_PER_NODE, MAX_POSSIBLE_BW_VALUE);
    cout<<"Avg edge count = "<<g.getAverageEdgeCount()<<endl;
    file<<"run,start,end,time_dijkstra,time_dijkstra_heap,time_kruskal,max_bandwidth\n";
    runAlgorithms(g, file, ITERATIONS_PER_GRAPH);
    file.close();  
}

void testFromFile() {
    Graph g("g.txt");
    Dijkstra* d = new Dijkstra();
    int s = 4, t = 96;
    d->dijkstra(g, s, t);
    d->printPath(t); cout<<endl;
    delete d;

    d = new Dijkstra();
    d->dijkstra(g, s, t, true);
    d->printPath(t); cout<<endl;
    delete d;
}





