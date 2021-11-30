## Finding the Maximum Bandwidth Pass on Graphs

To run the code compile and execute the main method in main.cpp. This would run the program with the defined parameters in the file (main.cpp).

To compile from inside the project directory run:
    g++ ./main.cpp -o main
    
To execute the program, from the same directory, run:
    ./main
Or alternatively on linux (Ubuntu) distributions, run the main.o executable bundled with the code

To change any of these, update the main file, recompile and execute.

The code produces, two sets .csv files - sparse\<iteration-number\>.csv and dense\<iteration-number\>.csv - as the output. These contain the results on a single run split based on the type of input graph used. The csv file has the following columns:
| run | start | end | time_dijkstra | time_dijkstra_heap | time_kruskal | max_bandwidth |
|--|--|--|--|--|--|--|
All the time entries are reported in microseconds.

The Graph::dumpToFile() method can be used to write the current graph in to a file "g.txt".
The rapidcsv.h libray is used to aid in debugging the code when it fails. This helps to parse the csv file dump generated from the program and create an idential graph to test and debug.
