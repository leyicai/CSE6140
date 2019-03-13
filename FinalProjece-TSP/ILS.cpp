//
// Created by Leyi Cai on 11/20/2018.
//


#include "ILS.h"

void startILS(const vector<vector<int>>& graph, const int num_nodes, const string& city_name, const int max_time, const int seed) {

    //maximum runtime (second)
    //replace with random parameter
    double max_time_sec = max_time;
    //maximum runtime (ms)
    double max_time_ms = max_time_sec * 1000;

    SOL* best_solution = new SOL;
    ILS(best_solution, graph, num_nodes, max_time_ms, city_name, seed);
    print_permutation(best_solution->permutation);
}

void outputToFile(string ofp, SOL *s, const int &CITY_SIZE, vector<vector<int>> adj_mat) {
    ofstream outfile(ofp);
    outfile.setf(ios::fixed);
    outfile << s->cost << endl;
//    for (int i = 0; i < CITY_SIZE - 1; ++i) {
//        outfile << s->permutation[i] << " " << s->permutation[i+1] << ", " << adj_mat[s->permutation[i]][s->permutation[i+1]] << endl;
//    }
//    outfile << s->permutation[CITY_SIZE-1] << " " << s->permutation[0] << ", " << adj_mat[s->permutation[CITY_SIZE-1]][s->permutation[0]] << endl;

    for(int i = 0; i < CITY_SIZE - 1; i++) {
        outfile << s->permutation[i] + 1 << ",";
    }
    outfile << s->permutation[CITY_SIZE-1] + 1 << endl;

    outfile.close();
}

// Compute distance between two cities
double cal_distance(CITIES c1, CITIES c2) {
    double distance = sqrt(pow((floor(c1.x) - floor(c2.x)), 2) + pow((floor(c1.y) - floor(c2.y)), 2));
    return distance;
}

void get_adj_mat(vector<CITIES> &cities, vector<vector<int>> &adj_mat, int CITY_SIZE) {

    for (int i = 0; i < CITY_SIZE; i++) {
        for (int j = 0; j < CITY_SIZE; j++) {
            double distance = cal_distance(cities[i], cities[j]);
            adj_mat[i][j] = distance;
            adj_mat[j][i] = distance;
        }
    }
}

//Initialize cities' permutation by nearest neighbor heuristic
void initial_sol(vector<int> &permutation, vector<vector<int>> adj_mat, int CITY_SIZE) {
    vector<bool> visit (CITY_SIZE, false);
    int i, j;

    //starts from node 0
    visit[0] = true;
    int current = 0;
    permutation.push_back(0);

    for (i = 1; i < CITY_SIZE; i++) {
        // find the nearest and unvisited adjacent vertex of current vertex
        double min = DBL_MAX;
        int min_ind;
        for (j = 0; j < CITY_SIZE; j++) {
            if (adj_mat[current][j] < min && !visit[j]) {
                min = adj_mat[current][j];
                min_ind = j;
            }
        }
        // set the found node as visited, store it into permutation array
        permutation.push_back(min_ind);
        visit[min_ind] = true;

        // next, explore the found node
        current = min_ind;
    }
}

int get_cost(vector<int>permutation, vector<vector<int>>adj_mat) {
    double cost = 0;
    for (int i = 0; i < permutation.size() - 1; i++) {
        cost += adj_mat[permutation[i]][permutation[i + 1]];
    }
    cost += adj_mat[permutation[0]][permutation[permutation.size() - 1]];
    return cost;
}

// Compute the variance of cost after 2-opt
// Permutation is changed from
// i-1 - i - abc - j - j+1
// to
// i-1 - k - cba - i - j+1
int get_delta(int i, int j, vector<int>permutation, vector<vector<int>>adj_mat, int CITY_SIZE) {
    int delta = 0;
    if (i == 0) {
        if (j == CITY_SIZE - 1) {
            delta  = 0;
        }
        else {
            delta = adj_mat[permutation[i]][permutation[j + 1]] + adj_mat[permutation[CITY_SIZE - 1]][permutation[j]] - adj_mat[permutation[CITY_SIZE - 1]][permutation[i]] - adj_mat[permutation[j]][permutation[j + 1]];
        }
    }
    else {
        if ( j == CITY_SIZE - 1) {
            delta = adj_mat[permutation[i]][permutation[0]] + adj_mat[permutation[i - 1]][permutation[j]] - adj_mat[permutation[i - 1]][permutation[i]] - adj_mat[permutation[j]][permutation[0]];
        }
        else {
            delta = adj_mat[permutation[i]][permutation[j + 1]] + adj_mat[permutation[i - 1]][permutation[j]] - adj_mat[permutation[i - 1]][permutation[i]] - adj_mat[permutation[j]][permutation[j + 1]];
        }
    }
    return delta;
}

// Use 2-opt method to perform local search
SOL* local_search(SOL* s, vector<vector<int>> adj_mat, int CITY_SIZE) {

    SOL* best_solution = new SOL;
    best_solution -> cost = s -> cost;
    best_solution -> permutation = s -> permutation;

    int cost;

    int i_best = -1, j_best = -1;

    //int max_iterations = 50;

    //for (int k = 0; k < max_iterations; k++) {
    for ( int i = 0; i < CITY_SIZE - 1; i++) {
        for (int j = i + 1; j < CITY_SIZE; j++) {
            int delta = get_delta(i, j, s->permutation, adj_mat, CITY_SIZE);
            // cout << "i: " << s->permutation[i] << " j:" << s->permutation[j] << " delta = " << delta << endl;
            cost = s->cost + delta;

            if (cost < best_solution->cost) {
                i_best = i;
                j_best = j;
                best_solution->cost = cost;
                //cout << "update: i = " << i_best << ", j = " << j_best << " new cost: " << best_solution->cost << endl;
            }
        }
    }
    //}

    //change permutation between i_best and j_best
     if (i_best != -1) {
        // cout << "before reverse: " << endl;
        // print_permutation(best_solution->permutation);
        reverse(best_solution->permutation.begin() + i_best, best_solution->permutation.begin() + j_best + 1);
        // cout << "**update: i = " << i_best << ", j = " << j_best << " new cost: " << best_solution->cost << endl;
        // print_permutation(best_solution->permutation);
      }
    return best_solution;

}

SOL* random_swap(SOL*s, vector<vector<int>> adj_mat, int CITY_SIZE, int max_iterations) {
    while (max_iterations--)
    {
        SOL* swap_solution = new SOL;
        swap_solution -> permutation = s -> permutation;
        int i = rand() % CITY_SIZE;
        int j = i;
        while (j == i) {
            j = rand() % CITY_SIZE;
        }
        int temp = swap_solution->permutation[i];
        swap_solution->permutation[i] = swap_solution->permutation[j];
        swap_solution->permutation[j] = temp;
        swap_solution->cost = get_cost(swap_solution->permutation, adj_mat);
        if (swap_solution->cost < s->cost) {
            //cout << "swap! Cost: " << swap_solution->cost << endl;
            return swap_solution;
        }
    }
    return s;
}

// Use double-bridge strategy to modify current solution
SOL* double_bridge(SOL* s, vector<vector<int>> adj_mat, int CITY_SIZE) {

    SOL* db_solution = new SOL;
    vector<int> new_permutation;

    int ind1, ind2, ind3;
    ind1 = 1 + rand() % (CITY_SIZE / 4);
    ind2 = ind1 + 1 + rand() % (CITY_SIZE / 4);
    ind3 = ind2 + 1 + rand() % (CITY_SIZE / 4);

    new_permutation.reserve(CITY_SIZE);
    for (int i = 0; i < ind1; i++) {
        new_permutation.push_back(s->permutation[i]);
    }
    for (int i = ind3; i < CITY_SIZE; i++) {
        new_permutation.push_back(s->permutation[i]);
    }
    for (int i = ind2; i < ind3; i++) {
        new_permutation.push_back(s->permutation[i]);
    }
    for (int i = ind1; i < ind2; i++) {
        new_permutation.push_back(s->permutation[i]);
    }

    db_solution -> permutation = new_permutation;
    db_solution -> cost = get_cost(new_permutation, adj_mat);

    if (db_solution->cost < s->cost) {
        cout << "***after db, cost is reduce to: " << db_solution->cost << endl;
    }
    return db_solution;

}

// The acceptance criteria: return the better one or the more recent one
// return (s2<s)?s2:s
SOL* better_equal(SOL*s, SOL*s2) {
    if (s2->cost < s->cost) {
        return s2;
    }
    else return s;
}

void ILS(SOL *&best_solution, vector<vector<int>> adj_mat, int CITY_SIZE, double max_time, string name, unsigned int seed) {

    clock_t start = clock();
    clock_t end;
    double duration = 0;

    //output file name
    string ofp = "OUTPUT/" + name;
    ofp.append("_ILS.tour");

    //solution file name
    string sfp = "OUTPUT/" + name;
    sfp.append("_LS2_");
    sfp.append(to_string((int)max_time/1000));
    sfp.append("_");
    sfp.append(to_string(seed));
    sfp.append(".sol");

    // solution trace file name
    string tfp = "OUTPUT/" + name;
    tfp.append("_LS2_");
    tfp.append(to_string((int)max_time/1000));
    tfp.append("_");
    tfp.append(to_string(seed));
    tfp.append(".trace");
    // open trace file
    ofstream tracefile(tfp);
    tracefile.setf(ios::fixed);

    cout.setf(ios::fixed);

    // get initial solution
    initial_sol(best_solution->permutation, adj_mat, CITY_SIZE);
    best_solution->cost = get_cost(best_solution->permutation, adj_mat);
    cout << "Initial Solution: Cost = " << best_solution->cost << endl;
    //print_permutation(best_solution->permutation);

    // local search on initial solution
    best_solution = local_search(best_solution, adj_mat, CITY_SIZE);
    //cout << "LocalSearch on initial solution: Cost = " << best_solution->cost <<" getcost = "<<get_cost(best_solution->permutation, adj_mat)<< endl;
    // print_permutation(best_solution->permutation);

    SOL* s1;
    SOL* s2;

    srand(seed);
    while (duration < max_time) {
        s1 = random_swap(best_solution, adj_mat, CITY_SIZE, 50);
        //s1 = double_bridge(best_solution, adj_mat, CITY_SIZE);
        //cout << "s1: Cost = " << s1->cost << endl;
        //print_permutation(s1->permutation);

        s2 = local_search(s1, adj_mat, CITY_SIZE);
        //cout << "s2: Cost = " << s2->cost << endl;
        // print_permutation(s2->permutation);

        //cout << "before compare: best cost = " << best_solution->cost << " s2 cost = " << s2->cost << endl;
        SOL* result = better_equal(best_solution, s2);
        //cout << "compare result: Cost = " << best_solution->cost << endl << endl;
        //print_permutation(best_solution->permutation);
        if (best_solution->cost > result -> cost) {
            //cout << "double_bridge->s1: Cost = " << s1->cost << endl;
            //print_permutation(s1->permutation);
            best_solution = result;
            clock_t current = clock() - start;
            cout << "Best solution updated at time: " << current << "ms\nCost = " << best_solution->cost << endl;
            tracefile << (double)current/1000 << ", " << best_solution->cost << endl;
            //print_permutation(best_solution->permutation);
            cout << endl;
        }
        end = clock();
        duration = 1000 * double(end - start) / (CLOCKS_PER_SEC);
        //cout << "duration: " << duration << "ms" <<endl;
    }
    tracefile.close();
    outputToFile(std::move(sfp), best_solution, CITY_SIZE, adj_mat);
}

void print_permutation(vector<int>permutation) {
    cout << "Permutation:" << endl;
    for (int i = 0; i < permutation.size(); i++) {
        cout << permutation[i] + 1 << "  ";
    }
    cout << endl << endl;
}
