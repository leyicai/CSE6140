Author          : Leyi Cai
Affiliation          : Georgia Institute of Technology


Description
-------------
This program is used to computu the Minimal Spanning Tree of input graph file. And than re-compute MST based on the changed edges added from <change_file>. The compute result will be written into the output file.

Installation
------------

To install the first program, simply run

    g++ Run Experiment -std=c11 -o MST


Execution
-----------

Assuming the your executable is called "MST", run it using

    ./MST <graph_file> <change_file> <output_file>
Note: <graph_file> is the original inpur; <change_file> contains the added edges data; <output_file> is the file to write computation results.