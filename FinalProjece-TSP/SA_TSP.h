//
// Created by Yingqiao Zheng on 11/17/2018.
//

#ifndef CSE_6140_TSP_SA_TSP_H
#define CSE_6140_TSP_SA_TSP_H

#define MIN(a,b) a < b ? a : b

#include <iostream>
#include <vector>
#include <float.h>          // DBL_MAX
#include <cstdlib>          // srand(), rand()
#include <time.h>           // time()
#include <cmath>            // sqrt(), round()
#include <string>
#include <algorithm>        // std::swap
#include <assert.h>
#include <limits.h>
#include <fstream>
#include <utility>
#include <float.h>

using namespace std;

typedef int DT;
typedef vector<vector<DT>> GRAPH;

// data structure for cities
typedef struct {
    // DT x, y;    // x and y coodinate of city
    unsigned int ID;     // unique city ID
}City;

typedef vector<City> PATH;

// data structure for final TSP tour
typedef struct {
    DT tour_length;         // total length of path
    PATH path;              // vector to store final path
}Tour;

// data structure for TSP parameter
typedef struct {
    // unsigned int maxIter;
    double iniTemp;
    double deRate;
    double endTemp;
    double maxTime;   // should be in unit of minute
}SApara;

/***************** FUNCTION DECLARATION *******************/
void startSA(const vector<vector<int>>& graph, const int num_nodes, const string& city_name, const int max_time, const int seed);
DT totalTourLength(const PATH &p, const GRAPH &m);
void initTour(Tour &t, const int &size, const GRAPH &m);
void firstState(Tour &t, const GRAPH &m, const int &maxTimes);
void printMatrix(const GRAPH &m);
void printPath(const Tour &t);
void SimulatedAnnealing(const SApara &p, Tour &t, const GRAPH &m, string ofp_trace);
void outputToFile(string ofp, const GRAPH &m, const Tour &t);

void startState(Tour &t, const GRAPH &m, const int &numNode);



#endif //CSE_6140_TSP_SA_TSP_H
