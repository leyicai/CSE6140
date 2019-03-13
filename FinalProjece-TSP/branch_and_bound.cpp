//
// Created by Leonard on 2018/11/18.
//
// This is the detailed implementation of branch and bound method.
// To have a more clear idea about what role does each function play in the branch and bound method, you can reference to the header file.

#include "branch_and_bound.h"

void TSPBranchAndBound(vector<vector<int>>& graph, vector<int>& final_path, vector<pair<float, int>>& history_res,
                       int& final_res, int num_nodes, time_t max_time){
    int cur_bound = 0;
    vector<bool> visited(num_nodes, false);
    vector<int> cur_path(num_nodes, -1);
    for(int i = 0; i < num_nodes; i++){
        cur_bound += (findFirstMin(graph, i, num_nodes) + findSecondMin(graph, i, num_nodes));
    }

    cur_bound = (cur_bound & 1) ? cur_bound / 2 + 1 : cur_bound / 2;

    visited[0] = true;
    cur_path[0] = 0;

    clock_t start = clock();
    bool time_flg = false;
    BranchAndBoundSolver(graph, cur_path, final_path, history_res, visited, final_res, cur_bound, 0, 1, num_nodes, start, time_flg, max_time);
}

void BranchAndBoundSolver(vector<vector<int>>& graph, vector<int>& cur_path, vector<int>& final_path, vector<pair<float, int>>& history_res, vector<bool>& visited,
                          int& final_res, int cur_bound, int cur_weight, int level, int num_nodes, clock_t start, bool& time_flg, time_t max_time){
    if(level == num_nodes){
        if(graph[cur_path[level - 1]][cur_path[0]] != 0){
            int cur_res = cur_weight + graph[cur_path[level - 1]][cur_path[0]];
            if(cur_res < final_res){
                clock_t now = clock();
                history_res.push_back(std::make_pair(((float)(now - start)) / CLOCKS_PER_SEC, cur_res));
                prepareFinalRes(cur_path, final_path, num_nodes);
                final_res = cur_res;
            }
        }
        return;
    }

    for(int i = 0; i < num_nodes; i++){
        if(graph[cur_path[level - 1]][i] != 0 && visited[i] == false){
#ifdef MAXTIME
            if(time_flg){
                return;
            }
            clock_t now = clock();
            if(((float)(now - start)) / CLOCKS_PER_SEC >= max_time){
                time_flg = true;
            }
#endif
#ifdef DEBUG
            cout << "Execute Step " << i + 1 << " in level " << level << endl;
#endif
            int temp = cur_bound;
            cur_weight += graph[cur_path[level - 1]][i];

            if(level == 1){
                cur_bound -= ((findFirstMin(graph, cur_path[level - 1], num_nodes) + findFirstMin(graph, i, num_nodes)) / 2);
            } else{
                cur_bound -= ((findSecondMin(graph, cur_path[level - 1], num_nodes) + findFirstMin(graph, i, num_nodes)) / 2);
            }

            if(cur_bound + cur_weight < final_res){
                cur_path[level] = i;
                visited[i] = true;
                BranchAndBoundSolver(graph, cur_path, final_path, history_res, visited, final_res,
                        cur_bound, cur_weight, level + 1, num_nodes, start, time_flg, max_time);
            }

            cur_weight -= graph[cur_path[level - 1]][i];
            cur_bound = temp;

            fill(visited.begin(), visited.end(), false);
            for(int j = 0; j < level; j++){
                visited[cur_path[j]] = true;
            }
        }
    }
}

void prepareFinalRes(vector<int>& cur_path, vector<int>& final_path, int num_nodes){
    for(int i = 0; i < num_nodes; i++){
        final_path[i] = cur_path[i];
    }
    final_path[num_nodes] = cur_path[0];
}

int findFirstMin(vector<vector<int>>& graph, int index, int num_nodes){
    int min = INT_MAX;
    for(int i = 0; i < num_nodes; i++){
        if(graph[index][i] < min && i != index){
            min = graph[index][i];
        }
    }
    return min;
}

int findSecondMin(vector<vector<int>>& graph, int index, int num_nodes){
    int fi_min = INT_MAX, se_min = INT_MAX;
    for(int i = 0; i< num_nodes; i++){
        if(i == index) {
            continue;
        }
        if(graph[index][i] < fi_min) {
            se_min = fi_min;
            fi_min = graph[index][i];
        } else if(graph[index][i] <se_min){
            se_min = graph[index][i];
        }
    }
    return se_min;
}

void flushResToFile(const vector<vector<int>>& graph, const vector<int>& final_path, const vector<pair<float, int>>& history_res, int final_res, int num_nodes, string& file_path, const string& city_name, const int max_time) {
    cout << "START OUTPUT!" << endl;
    cout << "FILE PATH: " << file_path << endl;
    file_path += city_name + "_BnB_" + to_string(max_time) + ".sol";
    fstream fout(file_path, std::ios_base::out);
    fout << final_res << endl;
    /*
    for(int iter = 0; iter < num_nodes; iter++){
        fout << final_path[iter] << " " << final_path[iter + 1] << " " << graph[final_path[iter]][final_path[iter + 1]] << endl;
    }
     */
    for(int iter = 0; iter < num_nodes; iter++){
        fout << final_path[iter] + 1 << ",";
    }
    fout << final_path[0] + 1 << endl;
    fout.close();

    int index = file_path.find('.');
    string his_res_file_path = file_path.substr(0, index) + ".trace";
    fout.open(his_res_file_path, std::ios_base::out);
    fout.precision(2);
    for(auto res : history_res){
        fout << res.first << ", " << res.second << endl;
    }
    fout.close();
}