#include <iostream>
#include <chrono>
#include "graph.h"
#include "dijkstra.h"
#include "heap.h"
// #include "graph.h"

using namespace std;


int main() {
    cout<<"Hello world\n\n";

     Graph g(5000, 6, 5000);
     g.getAverageEdgeCount();
    // Graph g("g.txt");

    // g.printGraph();
    // g.dumpToFile();
    // g.parseFile();

    int s = g.getRandomNode(), t = g.getRandomNode();
    // int s = 17, t = 2;
    cout<<"Shortest path from s - "<<s<<" to t - "<<t<<"\n";
    int* p;
    int i;

    auto begin = std::chrono::high_resolution_clock::now();
    p = dijkstra(g, s, t);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<"Time taken: "<<elapsed.count()<<endl;
    // printPath(p, t, g.num_nodes);
    // cout<<"\n\n";
    // cleanUpPathParams();
    // p = dijkstraWithHeap(g, s, t);
   
    cleanUpPathParams();
    begin = std::chrono::high_resolution_clock::now();
    p = dijkstra(g, s, t, true);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<"Time taken: "<<elapsed.count()<<endl;
    // printPath(p, t, g.num_nodes);
    
    return 0;



}

