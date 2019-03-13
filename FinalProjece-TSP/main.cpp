//
// Created by Leonard on 2018/11/18.
//

#include <unistd.h>
#include "parse_graph.h"
#include "fileReader.h"
#include "branch_and_bound.h"
#include "mstApprox.h"
#include "SA_TSP.h"
#include "ILS.h"

#define PATH_MAX 1024

//#define MULTIPLEFILE

int main(int argc, char** argv){
    if(argc < 4 || argc > 5) {
        cout << "ERROR! Invalid input parameter! Legal input format: " << endl;
        cout << "./EXEC <filename> <alg [BnB | Approx | LS1 | LS2] > <time> <seed [only need for local search method]>" << endl;
        exit(0);
    }

    set<string> cities({"Atlanta.tsp", "Berlin.tsp", "Boston.tsp", "Champaign.tsp", "Cincinnati.tsp",
                        "Denver.tsp", "NYC.tsp", "Philadelphia.tsp", "Roanoke.tsp", "SanFrancisco.tsp",
                        "Toronto.tsp", "UKansasState.tsp", "ulysses16.tsp", "UMissouri.tsp"});

    string file_name = argv[1];
    string method = argv[2];
    int max_time = atoi(argv[3]);
    int seed;

    if(cities.find(file_name) == cities.end()) {
        cout << "ERROR! Invalid input file name! Please enter correct file name!" << endl;
        exit(0);
    } else if(method != "BnB" && method != "Approx" && method != "LS1" && method != "LS2") {
        cout << "ERROR! Invalid approach of algorithm! Please use either [BnB, Approx, LS1, LS2] to determine which algorithm to execute!" << endl;
        exit(0);
    } else if((method == "BnB" || method == "Approx") && argc == 5) {
        cout << "ERROR! BnB and Approx approach do not need random seed! Correct input format is shown below: " << endl;
        cout << "./EXEC <filename> <alg [BnB | Approx] > <time> " << endl;
        exit(0);
    } else if((method == "LS1" || method == "LS2") && argc == 4) {
        cout << "ERROR! Local search method need to specify a random seed! Correct input format is shown below: " << endl;
        cout << "./EXEC <filename> <alg [LS1 | LS2] > <time> <seed>" << endl;
        exit(0);
    } else if(method == "LS1" || method == "LS2") {
        seed = atoi(argv[4]);
    }

    char buf[PATH_MAX];
    if(getcwd(buf, PATH_MAX) == 0) {
        cout << "ERROR! Unable to get current working directory!" << endl;
        exit(0);
    }
    string cur_work_path = buf;
    string file_path = cur_work_path + "/DATA/" + file_name;
    cout << "file path: " << file_path << endl;

    int index = file_name.find('.');
    string city_name = file_name.substr(0, index);
    string out_file_path = cur_work_path + "/OUTPUT/";

    cout << "Start reading dataset and generating graph..." << endl;
    fileReader reader(file_path);
    vector<vector<int>> graph = reader.readGraphIntoMatrix();
    vector<Distance> dis = reader.readGraphIntoEdgeSet();
    cout << "Graph generated!" << endl;

    int num_nodes = graph.size();

    if(method == "BnB") {
        cout << "Start Branch and Bound!" << endl;
        vector<pair<float, int>> history_res;
        vector<int> final_path(num_nodes + 1, -1);
        int final_res = INT_MAX;
        TSPBranchAndBound(graph, final_path, history_res, final_res, num_nodes, max_time);
        flushResToFile(graph, final_path, history_res, final_res, num_nodes, out_file_path, city_name, max_time);
        cout << "Branch and Bound finished or exceeded running time! Work done!" << endl;
    } else if(method == "Approx") {
        cout << "Start Approximation approach!" << endl;
        mstApprox alg(num_nodes, dis, graph, max_time, city_name, "OUTPUT/"); //Notice: directory ending with '/' (or '\' for Windows?)
        alg.approxTSP();
        cout << "Approximation approach finished or exceeded running time! Work done!" << endl;
    } else if(method == "LS1") {
        cout << "Start Simulated Annealing approach (local search)!" << endl;
        startSA(graph, num_nodes, city_name, max_time, seed);
        cout << "Simulated Annealing finished or exceeded running time! Work done!" << endl;
    } else if(method == "LS2") {
        cout << "Start Iterated Local search!" << endl;
        startILS(graph, num_nodes, city_name, max_time, seed);
        cout << "Iterated Local search finished or exceeded running time! Work done!" << endl;
    } else {
        cout << "This is impossible! LOL" << endl;
        cout << "Kill the programmer!" << endl;
        exit(0);
    }

    /*
#ifdef MULTIPLEFILE
    vector<string> file_paths({"Atlanta", "Berlin", "Boston", "Champaign", "Cincinnati",
                               "Denver", "NYC", "Philadelphia", "Roanoke", "SanFrancisco",
                               "Toronto", "UKansasState", "ulysses16", "UMissouri"});
    const string data_file_prefix = "/Users/apple/CLionProjects/CSE6140_Project/DATA/";
    const string output_file_prefix = "/Users/apple/CLionProjects/CSE6140_Project/OUTPUT/";
    const string data_file_suffix = ".tsp";
    const string output_file_suffix = ".tour";

    for(int count = 0; count < 14; count++){
        vector<vector<int>> graph;
        string TSP_name;
        int num_nodes = 0;
        file_path = data_file_prefix + file_paths[count] + data_file_suffix;
        out_file_path = output_file_prefix + file_paths[count] + output_file_suffix;
        parseToAdjMatrix(graph, TSP_name, num_nodes, file_path);

        // printInfo(graph, TSP_name, num_nodes);

        vector<pair<float, int>> history_res;
        vector<int> final_path(num_nodes + 1, -1);
        int final_res = INT_MAX;

        TSPBranchAndBound(graph, final_path, history_res, final_res, num_nodes, max_time);

        flushResToFile(graph, final_path, history_res, final_res, num_nodes, out_file_path);
    }
#endif

#ifndef MULTIPLEFILE
        vector<vector<int>> graph;
        string TSP_name = "ulysses16";
        int num_nodes = 0;

        //Fang's file reader
        fileReader reader(file_path);
        graph = reader.readGraphIntoMatrix();
        num_nodes = graph.size();

        //my file reader
        //parseToAdjMatrix(graph, TSP_name, num_nodes, file_path);

        //int temp_res = graph[0][7]+graph[7][3]+graph[3][1]+graph[1][2]+graph[2][15]+graph[15][9]+graph[9][8]+graph[8][10]+graph[10][4]+graph[4][14]+graph[14][5]+graph[5][6]+graph[6][11]+graph[11][12]+graph[12][13]+graph[13][0];
        //cout << "temp_res: " << temp_res << endl;

        printInfo(graph, TSP_name, num_nodes);

        vector<pair<float, int>> history_res;
        vector<int> final_path(num_nodes + 1, -1);
        int final_res = INT_MAX;

        TSPBranchAndBound(graph, final_path, history_res, final_res, num_nodes, max_time);

        flushResToFile(graph, final_path, history_res, final_res, num_nodes, out_file_path);
#endif
*/
    return 0;
}
