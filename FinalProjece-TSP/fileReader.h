/*
 * Read TSP files
 * Compute distances and construct graphs and edges array
 * Use adjacency matrix to represent graphs
 */
#ifndef CSE6140_PROJECT_FILE_READER_H
#define CSE6140_PROJECT_FILE_READER_H

#include <string>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <fstream>
using namespace std;

struct Distance{
    int n1, n2, value;
    Distance(int _n1, int _n2, int _v){n1 = _n1; n2 = _n2; value = _v;}
};

class fileReader {
private:
    string path;
    int n;
    vector<vector<int>> dmatrix;
    bool read = false;
public:
    fileReader(string p){path = p; n = 0; read = false;}
    vector<vector<int>> readGraphIntoMatrix();
    vector<Distance> readGraphIntoEdgeSet();
    int disEuc(int i, int j, vector<double>& x, vector<double>& y);
    int disGeo(int i, int j, vector<double>& x, vector<double>& y);
};

#endif //CSE6140_PROJECT_FILE_READER_H
/*
 * Usage:
 * 1. Declair an instance of fileReader with path to TSP file
 * fileReader r("/path/to/TSP/file/");
 * 2. To get adj matrix
 * vector<vector<int>> M = r.readGraphIntoMatrix();
 * 3. To a vector of edges
 * vector<Distance> D = r.readGraphIntoEdgeSet();
 */
