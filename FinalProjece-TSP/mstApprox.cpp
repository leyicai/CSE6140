#include "mstApprox.h"

/*
 * Implementation of union-find data structure
 */
int UnionFind::find(int i){
    while(i != root[i]){
        // Path compression
        root[i] = root[root[i]];
        i = root[i];
    }
    return i;
}
bool UnionFind::inSameSet(int i, int j){
    return find(i) == find(j);
}
void UnionFind::u(int i, int j){
    if(inSameSet(i, j)){
        return;
    }
    // Merge by rank
    int iroot = root[i], jroot = root[j];
    rootCnt--;
    if(rank[iroot] < rank[jroot]){
        swap(iroot, jroot);
    }

    root[jroot] = iroot;
    if(rank[iroot] == rank[jroot]){
        rank[iroot]++;
    }
}
bool UnionFind::finish(){
    return rootCnt == 1;
}

/*
 * Implementation of MST-Approx Algorithm
 */
void mstApprox::computMST(){
    UnionFind uf(n);
    MST = vector<vector<pair<int, int>>>(n);
    sort(distances.begin(), distances.end(), disCmp);
    for(Distance e : distances){
        int n1 = e.n1, n2 = e.n2, d = e.value;
        if(!uf.inSameSet(n1, n2)){
            uf.u(n1, n2);
            MST[n1].push_back(make_pair(n2, d));
            MST[n2].push_back(make_pair(n1, d));
        }
        if(uf.finish()){
            break;
        }
    }
}

void mstApprox::dfs(vector<bool>& visited, vector<int>& steps, int node, int& disSum){
    // assert(!visited[node]);
    visited[node] = true;
    if(steps.size() == 0){
        steps.push_back(node);
    }
    else{
        disSum += dmatrix[node][steps.back()];
        steps.push_back(node);
    }
    for(auto i : MST[node]){
        if(!visited[i.first]){
            dfs(visited, steps, i.first, disSum);
        }
    }
}

void mstApprox::approxTSP(int start){
    int disSum = 0;
    vector<int> steps;
    // Let Node 1 be the root
    vector<bool> visited(n, false);
    dfs(visited, steps, start, disSum);
    // assert(steps.size() == n);
    disSum += dmatrix[steps[0]][steps.back()];
    clock_t now = clock();
    if(disSum < bestSol){
        bestSol = disSum;
        allSol.push_back(bestSol);
        timeStamps.push_back(now);
        tour.push_back(steps);
    }
    if((now-start) / (float) CLOCKS_PER_SEC > cutoff){
        stop = true;
    }
}

void mstApprox::approxTSP() {
    start = clock();
    computMST();
    for (int i = 0; i < n && !stop; i++) {
        approxTSP(i);
    }
    writeResult();
}

void mstApprox::writeResult(){
    // write solution file
    string path = directory + name + "_Approx_" + to_string(cutoff) + ".sol";
    fstream out(path, ios_base::out);
    out << bestSol << endl;
    vector<int> bestTour = tour.back();
    for(int i = 0; i < n-1; i++){
        out << bestTour[i]+1 << ",";
    }
    out << bestTour[n-1]+1 << endl;

    // write trace fale
    path =  directory + name + "_Approx_" + to_string(cutoff) + ".trace";
    fstream out2(path, ios_base::out);
    for(int i = 0; i < allSol.size(); i++){
        out2 << (timeStamps[i]-start)/(float)CLOCKS_PER_SEC << "," << allSol[i] << endl;
    }
}