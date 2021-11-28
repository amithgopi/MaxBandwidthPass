## Finding the Maximum Bandwidth Pass on Graphs

To run the code compile and execute the main method in main.cpp. This would run the program with the defined parameters in the file (main.cpp).

To compile from inside the project directory run:
    g++ ./main.cpp -o main
    
To execute the program, from the same directory, run:
    ./main

To change any of these, update the main file, recompile and execute.

The code produces, two .csv files - sparse.csv and dense.csv - as the output. These contain the results on a single run split based on the type of input graph used. The csv file has the following columns:
| run | start | end | time_dijkstra | time_dijkstra_heap | time_kruskal | max_bandwidth |
|--|--|--|--|--|--|--|
All the time entries are reported in microseconds.