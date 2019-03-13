/*
 * Implementation of MST-Approx Algorithm
 */

#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <ctime>
#include <string>

#include "fileReader.h"

using namespace std;

/*
 * Implementation of Union-find Data Structure
 */
class UnionFind{
private:
    int rootCnt;
    vector<int> root;
    vector<int> rank;
public:
    UnionFind(int n){
        rootCnt = n;
        root = vector<int>(n, 0);
        for(int i = 1; i < n; i++){root[i] = i;}
        rank = vector<int>(n, 0);
    }
    int find(int i);
    bool inSameSet(int i, int j);
    void u(int i, int j);
    bool finish();
};

/*
 * MST-Approx Algorithm
 */
class mstApprox {
private:
    int n;
    int bestSol;
    int cutoff;
    bool stop = false;
    clock_t start;
    string name;
    string directory;
    vector<Distance> distances;
    vector<vector<int>> dmatrix;
    vector<vector<pair<int, int>>> MST;
    vector<int> allSol;
    vector<clock_t> timeStamps;
    vector<vector<int>> tour;

    // Compute MST
    void computMST();
    // DFS in MST
    void dfs(vector<bool>& visited, vector<int>& steps, int node, int& disSum);
    // Compute an approx solution given root (start node) of the MST
    void approxTSP(int start);
    // Write results into files
    void writeResult();

public:
    // Initilize an instance of the problem
    mstApprox(int n_, vector<Distance> d, vector<vector<int>> m, int c, string city, string dir){
        n = n_;
        distances = d;
        dmatrix = m;
        bestSol = INT_MAX;
        cutoff = c;
        name = city;
        directory = dir;
        stop = false;
    }

    static bool disCmp(const Distance& a, const Distance& b){
        return a.value < b.value;
    }

    // Try different roots and compute best solution of MST-Approx
    void approxTSP();
};
