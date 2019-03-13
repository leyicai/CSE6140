#include "fileReader.h"

#define PI 3.141592
#define RRR 6378.388

/*
 * Compute Eucilid Distances
 */
int fileReader::disEuc(int i, int j, vector<double>& x, vector<double>& y){
    double xd = x[i] - x[j], yd = y[i] - y[j];
    return (int)round(sqrt(xd*xd + yd*yd));
}

/*
 * Compute Geo Distances
 */
int fileReader::disGeo(int i, int j, vector<double>& x, vector<double>& y){
    int deg = (x[i] >= 0 ? (int)floor(x[i]) : (int)ceil(x[i]));
    double mini = x[i] - deg, latitudei = PI * (deg + 5.0 * mini / 3.0) / 180.0;

    deg = (y[i] >= 0 ? (int)floor(y[i]) : (int)ceil(y[i]));
    mini = y[i] - deg;
    double longitudei = PI * (deg + 5.0 * mini / 3.0) / 180.0;

    deg = (x[j] >= 0 ? (int)floor(x[j]) : (int)ceil(x[j]));
    mini = x[j] - deg;
    double latitudej = PI * (deg + 5.0 * mini / 3.0) / 180.0;

    deg = (y[j] >= 0 ? (int)floor(y[j]) : (int)ceil(y[j]));
    mini = y[j] - deg;
    double longitudej = PI * (deg + 5.0 * mini / 3.0) / 180.0;

    double q1 = cos(longitudei - longitudej), q2 = cos(latitudei - latitudej), q3 = cos(latitudei + latitudej);
    return (int)(RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
}

/*
 * Read file and construct the graph
 */
vector<vector<int>> fileReader::readGraphIntoMatrix(){
    fstream in(path, ios_base::in);
    string k, v;
    bool euc = true;
    int index;
    double x, y;
    while(in >> k){
        if(k == "NODE_COORD_SECTION"){
            break;
        }
        if(k == "NAME:"){
            in >> v;
            // name = v.substr(0, v.find(".tsp"));
        }
        else if(k == "EDGE_WEIGHT_TYPE:"){
            in >> v;
            euc = (v == "EUC_2D");
        }
        else if(k == "DIMENSION:") {
            in >> v;
            n = stoi(v);
        }
        else{
            continue;
        }
    }

    dmatrix = vector<vector<int>>(n, vector<int>(n, 0));
    vector<double> xset(n, 0.0), yset(n, 0.0);
    for(int i = 0; i < n; i++){
        in >> index >> x >> y;
        // assert(index == i+1);
        xset[i] = x;
        yset[i] = y;
    }

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            int d = euc ? disEuc(i, j, xset, yset) : disGeo(i, j, xset, yset);
            dmatrix[i][j] = dmatrix[j][i] = d;
        }
    }

    // in >> k;
    // assert(k == "EOF");
    read = true;
    return dmatrix;
}

/*
 * Reture a vector of edges with distances based on constructed graph
 */
vector<Distance> fileReader::readGraphIntoEdgeSet(){
    if(!read){
        readGraphIntoMatrix();
    }
    vector<Distance> distances;
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            distances.push_back(Distance(i, j, dmatrix[i][j]));
        }
    }
    return distances;
}