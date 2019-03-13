//
// Created by Leonard on 2018/11/18.
//

#ifndef CSE6140_PROJECT_PARSE_GRAPH_H
#define CSE6140_PROJECT_PARSE_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <cmath>

#define RRR 6378.388
//#define DEBUG

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::string;
using std::fstream;
using std::istringstream;

void parseToAdjMatrix(vector<vector<int>>& graph, string& name, int& num_nodes, string& file_path);

void printInfo(vector<vector<int>>& graph, string& name, int& num_nodes);


#endif //CSE6140_PROJECT_PARSE_GRAPH_H
