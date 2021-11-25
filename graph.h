#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <ctime>
#include <random>
#include <fstream>
#include <vector>
//Used to read CSV files, to generate graphs for debugging
#include "rapidcsv.h"

using namespace std;

class Node {
    public:
        int id;
        int weight;
        Node* next = nullptr;
        int parentId;

        Node() {}

        Node(int _id, int _weight, Node* _next, int _parentId): id(_id), weight(_weight), next(_next), parentId(_parentId) {}

        Node(int _id, int _weight, int _parentId): id(_id), weight(_weight), parentId(_parentId) {}
};

class Graph {
    private:
        mt19937 generator;
        uniform_int_distribution<int> distributeNodes;
        int* edgeCount;
    public:
        int num_nodes;
        int egdes_per_node;
        int max_BW_value;
        int total_edge_count;
        Node** list;

    public:

        Graph(int _num_nodes) {
            initEmptyGraph(_num_nodes);
        }

        void initEmptyGraph(int numNodes) {
            num_nodes = numNodes;
            total_edge_count = 0;


            list = new Node*[num_nodes];

            //Set head of each list to a node of id equal to index and 0 weight
            for(int i=0; i<num_nodes; i++) {
                list[i] = new Node(i, 0, nullptr, -1);
            }

            edgeCount = new int[num_nodes];
            fill_n(edgeCount, num_nodes, 0);
        }
        /**
         * @brief Construct a new Graph object from the given paramters
         * 
         * @param _num_nodes Number of vertices in the graph
         * @param _edges_per_node Average number of edges for each node
         * @param maxBWValue Maximum value of capacity/weight for each edge. Edge weight is in range [1, maxBWValue]
         */
        Graph(int _num_nodes, int _edges_per_node, int maxBWValue) {
            initEmptyGraph(_num_nodes);
            egdes_per_node = _edges_per_node;
            max_BW_value = maxBWValue;
            //Init random distribution values
            random_device rd;
            mt19937 gen(rd());
            generator = gen;
            uniform_int_distribution<int> distributeEdgeNumbers(1, egdes_per_node - 1);
            uniform_int_distribution<int> distributeEdgeWeight(1, maxBWValue);
            uniform_int_distribution<int> distributeEdgeNode(0, num_nodes-1);

            distributeNodes = distributeEdgeNode;
            //Allocate adj list space as array of pointers to node

            for(int i=0; i<num_nodes; i++) {
               
                int numEdges = distributeEdgeNumbers(gen);
                
                for(int j=0; j<(numEdges); j++) {
                    int weight = distributeEdgeWeight(gen), edgeNode = distributeEdgeNode(gen);
                    addEdge(i, edgeNode, weight);
                    addEdge(edgeNode, i, weight);
                }
            }

        }

        /**
         * @brief Destroy the Graph object and clear nay allocated memory
         * 
         */
        ~Graph() {
            delete[] edgeCount;
            //Clear each instantiated node object
            for(int i=0; i<num_nodes; i++) {
                Node* t = list[i];
                Node* temp = t;
                while(temp != nullptr) {
                    temp = t->next;
                    delete t;
                    t = temp;
                }
            }
            //Clear adjacency list
            delete[] list;
        }

        /**
         * @brief Adds a new edge node object to the adjacency list at the given fromNode.
         * 
         * @param fromNode Node from which the edge is to be fromed
         * @param toNode Destination node of the edge
         * @param weight The weight or capacity of the edge
         */
        void addEdge(int fromNode, int toNode, int weight) {
            if(fromNode == toNode) return;
            edgeCount[fromNode]++;
            total_edge_count++;
            Node* temp = list[fromNode]->next;
            //Create new node for edge
            list[fromNode]->next = new Node(toNode, weight, fromNode);
            //Reassign next and prev pointers
            list[fromNode]->next->next = temp;
            // list[fromNode]->next->prev = list[fromNode];
            // if(temp != nullptr) temp->prev = list[fromNode]->next;
        }

        /**
         * @brief Get a random node id from the set [0, num_nodes-1]
         * 
         * @return int a random node id
         */
        int getRandomNode() {
            return distributeNodes(generator);
        }

        /**
         * @brief compute and return the average no of edges per node in the generated graph
         * 
         * @return float number of edges per node
         */
        float getAverageEdgeCount() {
            // cout<<"Avg edges = "<<(float)totalEdges/num_nodes<<endl;
            return (float)total_edge_count/num_nodes;
        }


        /**
         * @brief Construct a new Graph object from a given CSV file generated with dumpToFile()
         * 
         * @param fileName 
         */
        Graph(string fileName){
            parseFile(fileName);
            //Init random distribution values
            random_device rd;
            mt19937 gen(rd());
            generator = gen;

            uniform_int_distribution<int> distributeEdgeNode(0, num_nodes-1);
            distributeNodes = distributeEdgeNode;
            
        }

        /**
         * @brief Parse a csv file with the graph information and add edges to it based on this information
         * 
         * @param fileName Name of the CSV file to be parsed
         */
        void parseFile(string fileName) {
            rapidcsv::Document doc(fileName, rapidcsv::LabelParams(-1, -1));
            num_nodes = doc.GetRowCount();
            initEmptyGraph(num_nodes);
   
            for(int i=0; i<num_nodes; i++) {
                std::vector<int> r = doc.GetRow<int>(i);
                for(int j=r.size()-1; j>1; j-=2) {
                    addEdge(i, r[j-1], r[j]);
                }
            }
            

        }

        /**
         * @brief Write the grapgh contents to a file in csv format.
         * The file is formatted with one row of the adjacency list in each line
         * Each line as entries as node_id, weight, next_node_id, next_node_weight ....
         * 
         */
        void dumpToFile() {
            ofstream file;
            file.open("g.txt");

            for(int i=0; i<num_nodes; i++){
                    Node* currNode = list[i];
                    while(currNode != nullptr) {
                        file<<currNode->id<<","<<currNode->weight;
                        currNode = currNode->next;
                        if(currNode != nullptr) file<<",";
                    } 
                    file<<'\n';
                }
                file.close();
        }
        
        /**
         * @brief Print the adjacency list to the termial
         * 
         */
        void printGraph() {
            for(int i=0; i<num_nodes; i++){
                Node* currNode = list[i];
                while(currNode != nullptr) {
                    cout<<currNode->id<<"("<<currNode->weight<<") -> ";
                    currNode = currNode->next;
                } 
                cout<<endl;
            }
        }
    
};

#endif