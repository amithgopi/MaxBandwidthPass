#include <iostream>
#include <chrono>
#include <fstream>
#include "graph.h"
#include "dijkstra.h"
#include "kruskal.h"
#include <sys/resource.h>

#define CSV_FILE_SEPERATOR ","

//These can be modified to change the type of graph
#define GRAPH_NUMBER_OF_VERTICES 5000
#define SPARSE_EDGES_PER_NODE 6
#define DENSE_EDGES_PER_NODE 1000
#define MAX_POSSIBLE_BW_VALUE 10000
#define NUMBER_OF_ITERATIONS 5

using namespace std;

void sparseGraph(int i);
void denseGraph(int i);
void testFromFile();

/**
 * @brief Main function
 * 
 * @return int 
 */
int main() {
    
    for(int i=0; i<NUMBER_OF_ITERATIONS; i++) {
        sparseGraph(i);
        denseGraph(i);
    }

    // testFromFile();
    
    return 0;

}

/**
 * @brief Runs each algorithm for the specifeid number of iterations and records execution time and BW in the specified file
 * 
 * @param g Graph on which to run each algorithm
 * @param file open file stream refrence to write the run details to
 * @param timesToExecute Nunmber of times to run
 * @param printPath Flag to enable printing the shortest path
 */
void runAlgorithms(Graph &g, ofstream &file, int timesToExecute, bool printPath = false) {
    int maxBW[3];
    struct rusage r_usage;
    for(int i=0; i<timesToExecute; i++) {
        int ret = getrusage(RUSAGE_SELF,&r_usage);
        cout<<r_usage.ru_maxrss;

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
        maxBW[2] = k->getMaxBW();
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

/**
 * @brief Generate and run program for sparse graph based on defined parameters
 * 
 */
void sparseGraph(int i = 1) {
    cout<<"\n----STARTING RUN ON SPARSE GRAPH----"<<endl;
    ofstream file;
    file.open("sparse" + to_string(i) + ".csv");
    Graph g(GRAPH_NUMBER_OF_VERTICES, SPARSE_EDGES_PER_NODE, MAX_POSSIBLE_BW_VALUE);
    file<<"run,start,end,time_dijkstra,time_dijkstra_heap,time_kruskal,max_bandwidth\n";
    runAlgorithms(g, file, NUMBER_OF_ITERATIONS);  
    file.close();
}

/**
 * @brief  Generate and run program for dense graph based on defined parameters
 * 
 */
void denseGraph(int i = 1) {
    cout<<"\n----STARTING RUN ON DENSE GRAPH----"<<endl;
    ofstream file;
    file.open("dense" + to_string(i) + ".csv");
    Graph g(GRAPH_NUMBER_OF_VERTICES, DENSE_EDGES_PER_NODE, MAX_POSSIBLE_BW_VALUE);
    file<<"run,start,end,time_dijkstra,time_dijkstra_heap,time_kruskal,max_bandwidth\n";
    runAlgorithms(g, file, NUMBER_OF_ITERATIONS);
    file.close();  
}

/**
 * @brief Used for testing and debugging - loads grapgh from the file dump
 * 
 */
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





