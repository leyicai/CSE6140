/**
 * hcc.h
 * Kasimir Gabert
 * Georgia Institute of Technology, Fall 2018
 *
 * Heist-Closeness Centrality computation
 */

#ifndef _H_HCC_H
#define _H_HCC_H

#include "graph.h"
#include "io.h"
#include <iostream>
#include <queue>
#include <vector>



namespace HCC {

/** Compute HCC for each vertex, saving into vdata */
void compute(Graph& g, VertexSet &h);
void BFS(Graph &g, vid_t v, std::vector<vdata_t>& temp);

};


#endif /* _H_HCC_H */

