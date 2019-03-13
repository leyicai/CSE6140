//
// Created by Leonard on 2018/11/18.
//
// This file is the header file of the implementation of branch and bound method.
// Used to solve the TSP problem using branch and bound method while recording data.
// Apart from solving and recording experimental data, also used to flush result to outside files.

#ifndef CSE6140_PROJECT_BRANCH_AND_BOUND_H
#define CSE6140_PROJECT_BRANCH_AND_BOUND_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <limits.h>
#include <time.h>
#include <float.h>

//#define DEBUG
#define MAXTIME

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::string;
using std::fstream;
using std::to_string;

// Branch and bound solver entrance, used to set environment and variables
void TSPBranchAndBound(vector<vector<int>>& graph, vector<int>& final_path, vector<pair<float, int>>& history_res, int& final_res, int num_nodes, time_t max_time);

// Branch and bound main solver function used in recursive, record every solution found better than last one and timing
void BranchAndBoundSolver(vector<vector<int>>& graph, vector<int>& cur_path, vector<int>& final_path, vector<pair<float, int>>& history_res, vector<bool>& visited,
                          int& final_res, int cur_bound, int cur_weight, int level, int num_nodes, clock_t start, bool& time_flg, time_t max_time);


// Flush temporal result to final result
void prepareFinalRes(vector<int>& cur_path, vector<int>& final_path, int num_nodes);

// Find the minimal weighted edge among all edges around one node
int findFirstMin(vector<vector<int>>& graph, int index, int num_nodes);

// Find the second minimal weighted edge among all edges around one node
int findSecondMin(vector<vector<int>>& graph, int index, int num_nodes);

// Flush result to file according to the requirement of assignment description
void flushResToFile(const vector<vector<int>>& graph, const vector<int>& final_path, const vector<pair<float, int>>& history_res, int final_res, int num_nodes,
        string& file_path, const string& city_name, const int max_time);

#endif //CSE6140_PROJECT_BRANCH_AND_BOUND_H
