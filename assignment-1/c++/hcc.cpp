/**
 * hcc.cpp
 * Leyi Cai
 * Georgia Institute of Technology, Fall 2018
 *
 * Heist-Closeness Centrality computation implementation
 *
 * NOTE: You should change this file to add in your implementation.
 * Feel free to create as many local functions as you want.
 */

#include "hcc.h"
#include "graph.h"
#include "io.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;


void
HCC::compute(Graph &g, VertexSet &h) {

    // <Implement HCC here>

    vid_t n = g.get_n();
    vid_t v = 0;
    vector <vdata_t> temp_data;
    temp_data.reserve(n);
    temp_data.resize(n,0);




    for ( v = 0; v < n; v ++){
    	if(h.search(v)){		//if v belongs to VetexSet h

    		BFS(g, v, temp_data);
    		//for (int i=0; i < n; i++)
    		  //  cout << temp_data[i] << endl;
   	 	}

   	}


	for ( v = 0; v < n; v ++){
		//cout << 1 / temp_data[v] << endl;
		g.set_vdata(v, 1/temp_data[v]);
	}

}

void
HCC::BFS(Graph &g, vid_t v, vector<vdata_t>& temp){

	queue <vid_t> Q;
	vid_t cur, next;
	vid_t n = g.get_n();

	//To store the visited status of each node. True for visited and False for not visited.
	std::vector<bool> visit;
	visit.reserve(n);
	visit.resize(n, false);

	std::vector <double> distance;
	distance.reserve(n);
	distance.resize(n,0);


	Q.push(v);
	visit[v] = 1;

	while(!Q.empty()){
		cur = Q.front();
		Q.pop();


		for(eid_t* i = g.edge_begin(cur); i < g.edge_end(cur); i++){

			next = *i;

			if(visit[next] == false){
				Q.push(next);
				visit[next] = true;	//set the node as visited.
				distance[next] = distance[cur] + 1;
				temp[next] += distance[next];
			}
		}
	}

}
