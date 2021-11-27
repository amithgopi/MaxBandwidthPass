#include <iostream>
#include <chrono>
#include "graph.h"
#include "dijkstra.h"
#include "kruskal.h"
// #include "graph.h"

using namespace std;


int main() {

     Graph g(5000, 6, 10000);
    //  g.getAverageEdgeCount();
    // Graph g("g.txt");

    // g.printGraph();
    // g.dumpToFile();


    int s = g.getRandomNode(), t = g.getRandomNode();
    // int s = 29, t = 78;
    cout<<"Shortest path from s - "<<s<<" to t - "<<t<<"\n";
    int* p;
    int i;
    int maxBW[3];

    auto begin = std::chrono::high_resolution_clock::now();
    Dijkstra* d = new Dijkstra();
    p = d->dijkstra(g, s, t);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<"Time taken: "<<elapsed.count()<<endl;
    maxBW[0] = d->getMaxBW();
    // d->cleanUpPathParams();
    delete d;

    d = new Dijkstra();
    begin = std::chrono::high_resolution_clock::now();
    p = d->dijkstra(g, s, t, true);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<"Time taken: "<<elapsed.count()<<endl;
    maxBW[1] = d->getMaxBW();
    delete d;
    // cleanUpPathParams();
    // printPath(p, t, g.num_nodes);

    cout<<"\n";
    Kruskal* k = new Kruskal();
    k->kruskal(g,s, t);
    maxBW[2] = k->getMaxBW();
    delete k;

    if(!(maxBW[0] == maxBW[1] && maxBW[1] == maxBW[2])) {
        g.dumpToFile();
        cout<<"\nMax BWs does not match.."<<endl;
    }
    
    
    return 0;



}

