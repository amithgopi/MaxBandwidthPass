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

int main() {

    sparseGraph();
    denseGraph();

    //  Graph g(5000, 6, 10000);
    // //  g.getAverageEdgeCount();
    // // Graph g("g.txt");

    // // g.printGraph();
    // // g.dumpToFile();


    // int s = g.getRandomNode(), t = g.getRandomNode();
    // // int s = 29, t = 78;
    // cout<<"Shortest path from s - "<<s<<" to t - "<<t<<"\n";
    // int* p;
    // int i;
    // int maxBW[3];

    // auto begin = std::chrono::high_resolution_clock::now();
    // Dijkstra* d = new Dijkstra();
    // p = d->dijkstra(g, s, t);
    // auto end = std::chrono::high_resolution_clock::now();
    // auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // cout<<"Time taken: "<<elapsed.count()<<endl;
    // maxBW[0] = d->getMaxBW();
    // // d->cleanUpPathParams();
    // delete d;

    // d = new Dijkstra();
    // begin = std::chrono::high_resolution_clock::now();
    // p = d->dijkstra(g, s, t, true);
    // end = std::chrono::high_resolution_clock::now();
    // elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // cout<<"Time taken: "<<elapsed.count()<<endl;
    // maxBW[1] = d->getMaxBW();
    // delete d;
    // // cleanUpPathParams();
    // // printPath(p, t, g.num_nodes);

    // cout<<"\n";
    // Kruskal* k = new Kruskal();
    // k->kruskal(g,s, t);
    // maxBW[2] = k->getMaxBW();
    // delete k;

    // if(!(maxBW[0] == maxBW[1] && maxBW[1] == maxBW[2])) {
    //     g.dumpToFile();
    //     cout<<"\nMax BWs does not match.."<<endl;
    // }
    
    
    return 0;

}

void runAlgorithms(Graph &g, ofstream &file, int timesToExecute, bool printPath = false) {
    int maxBW[3];
    for(int i=0; i<timesToExecute; i++) {
        cout<<"\nIteration "<<i<<endl;
        Dijkstra* d;
        int s = g.getRandomNode(), t = g.getRandomNode();
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
    cout<<"----STARTING RUN ON DENSE GRAPH----"<<endl;
    ofstream file;
    file.open("dense.csv");
    Graph g(GRAPH_NUMBER_OF_VERTICES, DENSE_EDGES_PER_NODE, MAX_POSSIBLE_BW_VALUE);
    file<<"run,start,end,time_dijkstra,time_dijkstra_heap,time_kruskal,max_bandwidth\n";
    runAlgorithms(g, file, ITERATIONS_PER_GRAPH);
    file.close();  
}





