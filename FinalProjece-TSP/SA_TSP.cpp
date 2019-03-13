//
// Created by Yingqiao Zheng on 11/17/2018.
//

#include "SA_TSP.h"

void startSA(const vector<vector<int>>& graph, const int num_nodes, const string& city_name, const int max_time, const int seed) {
    // initialize a tour
    Tour test;
    test.path.clear();
    test.tour_length = 0;

    // parameters for SA
    SApara tsp;
    // end temperature
    tsp.endTemp = 2.465e-321;
    // initial temperature
    tsp.iniTemp = DBL_MAX;
    // decay rate
    tsp.deRate = 0.999;
    // max runtime (s)
    tsp.maxTime = max_time;

    string ofp = "OUTPUT/" + city_name + "_LS1_" + to_string(max_time) + "_" + to_string(seed) + ".sol";
    // output file: trace
    string ofp_trace = "OUTPUT/" + city_name + "_LS1_" + to_string(max_time) + "_" + to_string(seed) + ".trace";

    // set random seed
    srand((unsigned int) seed);

    try {
        // initialize first state
        initTour(test, num_nodes, graph);
        firstState(test, graph, 8000);

        // run SA
        SimulatedAnnealing(tsp, test, graph, ofp_trace);

        // write solution file (note that trace file output is inside SA funciont itself)
        outputToFile(ofp, graph, test);
    } catch (const char *msg) {
        cerr << msg << endl;;
        exit(1);
    }
}

// calculate total length of a given tour
DT totalTourLength(const PATH &p, const GRAPH &m) {
    if (m.empty()) throw "Error calculate tour length: Empty adjacency matrix!\n";
    if (p.empty()) return 0;
    DT l = 0;
    for (int i = 1; i < p.size(); ++i) {
        l += m[p[i].ID][p[i-1].ID];
    }
    // add the final length of the cycle
    l += m[p[0].ID][p[p.size()-1].ID];
    return l;
}

void initTour(Tour &t, const int &size, const GRAPH &m) {
    if (m.empty()) throw "Error initialize tour: Empty adjacency matrix!\n";
    if (!t.path.empty()) t.path.clear();
    for (unsigned int i = 0; i < size; ++i) {
        City tmp;
        tmp.ID = i;
        t.path.push_back(tmp);
    }
    t.tour_length = totalTourLength(t.path, m);
}

void firstState(Tour &t, const GRAPH &m, const int &maxTimes) {
    if (t.path.empty()) throw "Cannot create first state: empty path!\n";

    PATH curr = t.path;

    for (int n = 0; n < maxTimes; ++n) {
        for (int i = 0; i < t.path.size(); ++i) {
            int j = rand() % ((int)t.path.size());
            swap(curr[i], curr[j]);
        }
        DT newLength = totalTourLength(curr, m);
        if (newLength < t.tour_length) {
            t.path = curr;
            t.tour_length = newLength;
        }
    }
}

void SimulatedAnnealing(const SApara &p, Tour &t, const GRAPH &m, string ofp_trace) {

    if (t.path.empty()) throw "Cannot do simulated annealing: empty path!\n";

    int better = 0;
    int worse = 0;
    DT best_solution = t.tour_length;
    DT curr_length;

    PATH curr = t.path;
    vector<pair<int, int>> Tabu;
    vector<pair<int, int>>::iterator it;
    pair<int, int> pairToSwap;

    // assert(p.maxIter > 0);
    assert(p.maxTime > 0);
    assert(p.deRate < 1 && p.deRate > 0);
    assert(p.iniTemp > p.endTemp);

    // output file for trace
    ofstream outfile(ofp_trace);

    clock_t start = clock();
    int iter = 0;
    double temp = p.iniTemp;
    do {
        clock_t now = clock();
        float dur = ((float)(now - start)) / CLOCKS_PER_SEC;
        if (dur >= p.maxTime) {
            cout << "Reach maximum runtime. break." << endl;
            break;
        }

        int s1 = rand() % ((int)t.path.size());
        int s2 = rand() % ((int)t.path.size());

        // if randomly pick the same node, continue the loop
        if (s1 == s2)  { continue; }

        pairToSwap.first = s1;
        pairToSwap.second = s2;

        it = find(Tabu.begin(), Tabu.end(), pairToSwap);

        if (it != Tabu.end()) {
            // cout << "Tabu!!" << endl;
            continue;
        }

        if (Tabu.size() >= t.path.size() * 2) {
            Tabu.erase(Tabu.begin());
            Tabu.push_back(pairToSwap);
            // cout << "Full & not Tabu!\n" << endl;
        } else {
            Tabu.push_back(pairToSwap);
            // cout << "Not Full & not Tabu!\n" << endl;
        }

        // cout << Tabu.size() << endl;

//        // swap 1 pair of city and cities inside their range
//        if (s1 > s2) { swap(s1, s2); }
//
//        while (s1 != s2 && s1 < s2) {
//            swap(curr[s1], curr[s2]);
//            s1++;
//            s2--;
//        }

        // swap one pair of city only
        swap(curr[s1], curr[s2]);

        curr_length = totalTourLength(curr, m);
        DT dc = curr_length - t.tour_length;

        if (curr_length < best_solution) {
            best_solution = curr_length;
            outfile << dur << ", " << best_solution << endl;
        }

        double pro = rand() % 1000000;
        pro /= 1000000;

        // if better accept
        if (dc < 0) {
            better++;
            t.path = curr;
            t.tour_length += dc;
            // outfile << dur << ", " << t.tour_length << endl;
        }
        // if worsen, accept with probability
        else if (exp((-dc) / temp) > pro) {
            worse++;
            t.path = curr;
            t.tour_length += dc;
        }

        // print temp result
        if (iter % 10000000 == 0) {
            cout << "Iteration time: " << iter << endl;
            cout << better << " better step, " << worse << " worsening step." << endl;
            cout << "Temperature: " << temp << endl;
            cout << "Current runtime: " << dur << " seconds" << endl;
            printPath(t);
        }

        // update temperature
        temp *= p.deRate;
        if (temp < p.endTemp) {
            cout << "Reach end temperature. break." << endl;
            break;
        }

        // print current solution
//        for (const auto &i : curr)
//            cout << i.ID << ' ';
//        cout << endl;
//        cout << "Delta: " << dc << endl;
//        cout << "Temperature: " << temp << endl;
//        cout << "Probability: " << exp((-dc) / temp) << " & " << pro << endl;

    } while (++iter);
    outfile.close();
}

void printMatrix(const GRAPH &m) {
    for (const auto &row : m) {
        for (const auto &dis : row)
            cout << dis << ' ';
        cout << endl;
    }
}

void printPath(const Tour &t) {
    for (const auto &i : t.path)
        cout << i.ID << ' ';
    cout << endl;
    cout << "length: " << t.tour_length << endl;
}

void outputToFile(string ofp, const GRAPH &m, const Tour &t) {
    ofstream outfile(ofp);
    outfile << t.tour_length << endl;
    for (int i = 0; i < t.path.size(); ++i) {
        outfile << t.path[i].ID + 1 << ",";
    }
    outfile << t.path[0].ID + 1;

    outfile.close();
}


void startState(Tour &t, const GRAPH &m, const int &numNode) {
    if (m.empty()) throw "Error initialize tour: Empty adjacency matrix!\n";
    if (!t.path.empty()) t.path.clear();


    vector<bool> added((unsigned long)numNode, false);

    DT min = INT_MAX;
    // randomly choose and push the first city to path
    unsigned int last = (unsigned int)(rand() % numNode);                   // the last element in path
    unsigned int begin = last;

    City tmp;
    tmp.ID = last;
    t.path.push_back(tmp);
    t.tour_length = 0;
    added[last] = true;

    for (int cnt = 0; cnt < numNode - 1; ++cnt) {
        min = INT_MAX;
        for (unsigned int j = 0; j < numNode; ++j) {
            if (added[j] == true || j == last) { continue; }
            if (m[last][j] < min) {
                min = m[last][j];
                last = j;
            } else { continue; }
        }
        tmp.ID = last;
        t.path.push_back(tmp);
        added[last] = true;
        t.tour_length += min;
    }
    t.tour_length += m[begin][last];
}






