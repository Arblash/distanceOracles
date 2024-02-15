The programs used in the thesis.

The `main.cpp` is a (2,1)-approximate distance oracle program that accepts two arguments:
1. Algorithm Type - affects the sampling method, possible values:
    `1000`: sample is the whole graph, results in program equivalent to BFS. Denoted as `BFS` in the thesis.
    `2005`: basic random sample, used in the (3,0)-approximate distance oracle by Thorup and Zwick. Denoted as `Basic` in the thesis.
    `2008`: basic random sample expanded with Baswana, Goyal and Sen method, that was used in their (2,1)-approximate distance oracle. Denoted as `BaGoSe` in the thesis.
    `2010`: basic random sample expanded with Patrascu and Roditty method, that was used in their (2,1)-approximate distance oracle. Denoted as `PatRod` in the thesis.
2. Integer Seed for the pseudorandom number generator - allows for the same basic sample for all algorithm types, reducing the noise in the comparison. It will not affect PTHash, as we made no attempt to modify this library.

The program will read the graph from the standard input and will output approximate distances between all pairs of vertices and some statistics to the standard output. It can be easily modified to answer only some queries from standard input.

The `runTests.cpp` program is a program that tests the `main.cpp` on graphs generated by `generateGraph.cpp`, it outputs the results to the standard output.
You can modify the constants here for different test sets.

The `pthash` folder is a folder with a submodule `PTHash`.

The programs can be compiled using cmake on the Linux system (and possibly Mac OS), as required for pthash.
For cmake installation please refer to the pthash's `README.md`.

To compile the programs run the following in terminal:
    mkdir build
    cd build
    cmake ..
    make -j
