//
// Created by Leonard on 2018/11/18.
//
#include "parse_graph.h"


void parseToAdjMatrix(vector<vector<int>>& graph, string& name, int& num_nodes, string& file_path){
    fstream fin(file_path, std::ios_base::in);
    string content, temp, weight_type;
    int index;
    while(content != "NODE_COORD_SECTION" && !fin.eof()) {
        temp = "";
        getline(fin, temp);
        istringstream sin(temp);
        sin >> content;
        if (content == "NAME:") {
            sin >> name;
        } else if (content == "DIMENSION:") {
            sin >> temp;
            num_nodes = stoi(temp);
        } else if (content == "EDGE_WEIGHT_TYPE:") {
            sin >> weight_type;
        }
        sin.str("");
    }

#ifdef DEBUG
    cout << "Enter NODE_COORD_SECTION!" << endl;
#endif

    vector<pair<double, double>> coords(num_nodes, pair<double, double>());
    for(int line = 0; line < num_nodes; line++){
        fin >> index >> coords[index - 1].first >> coords[index - 1].second;
#ifdef DEBUG
        cout << index << " " << coords[index].first << " " << coords[index].second << endl;
#endif
    }
    fin.close();

#ifdef DEBUG
    cout << "name: " << name << endl;
    cout << "num_nodes: " << num_nodes << endl;
    cout << "Enter GRAPH_PARSE_SECTION!" << endl;
#endif

    for(int i = 0; i < num_nodes; i++){
        graph.push_back(vector<int>());
#ifdef DEBUG
        cout << "Computing node " << i << endl;
#endif
        for(int j = 0; j < num_nodes; j++){
            if(j == i){
                graph[i].push_back(0);
            } else{
                int res;
                if(weight_type == "EUC_2D"){
                    res = (int)round(sqrt(pow(coords[i].first - coords[j].first, 2) + pow(coords[i].second - coords[j].second, 2)));
                } else{
                    double q1, q2, q3;
                    q1 = cos(coords[i].second - coords[j].second);
                    q2 = cos(coords[i].first - coords[j].first);
                    q3 = cos(coords[i].first + coords[j].first);
                    res = (int)(RRR * acos(0.5 * ((1 + q1) * q2 - (1 - q1) * q3)) + 1);
                }
                graph[i].push_back(res);
            }
        }
    }
#ifdef DEBUG
    cout << "GRAPH PARSE DONE!" << endl;
#endif
}


void printInfo(vector<vector<int>>& graph, string& name, int& num_nodes){
    cout << "****** GRAPH'S INFORMATION ******" << endl;
    cout << "NAME: " << name << endl;
    for(int i = 0; i < num_nodes; i++){
        for(int j = 0; j < num_nodes; j++){
            cout << i << " -> " << j << ": " << graph[i][j] << endl;
        }
    }
}
