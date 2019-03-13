/*
CSE6140 HW2
This is an example of how your experiments should look like.
Feel free to use and modify the code below, or write your own experimental code, as long as it produces the desired output.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <string>
#include <tuple>
#include <unordered_set>
using namespace std;

class Graph {
public:
       int ver_num, edge_num;             //numbre of vertices and edges
       vector<tuple<int, int, int>> graph;          //to store graph nodes:(weight,u,v)
       vector<vector<pair<int, int>>> tree;        //adjacent lists to store MST nodes:u ->(v, weight)
       int MSTweight;
       void printGraph() {
              cout << ver_num << " " << edge_num << endl;
              for (int i = 0; i < graph.size(); ++i)
              {
                     cout << get<0>(graph[i]) << ": " << get<1>(graph[i]) << " -> " << get<2>(graph[i]) << endl;
              }
              cout << endl;
              /*
              sort(graph.begin(), graph.end());
              for (int i = 0; i < graph.size(); ++i)
              {
                     cout << get<0>(graph[i]) << ": " << get<1>(graph[i]) << " -> " << get<2>(graph[i]) << endl;
              }
              cout << endl;
              */
       }
       void printMST() {
              int i;
              cout << MSTweight << endl;
              for (i = 0; i < tree.size(); ++i) {
                     //cout <<"nodesize "<<tree[i].size();
                     cout << "  node " << i << ": ";
                     for (auto edge : tree[i])
                            cout  << edge.first << ": " << edge.second << ",  ";
                     cout << endl;
              }
       }



};
Graph parseEdges(string graph_file) {
       ifstream input(graph_file);
       Graph g;
       if (input.is_open()) {
              int n, m;     //n: vertex number. m: edge number
              int u, v, weight;    //vertices of every edge
              int cur_u = -1, cur_v = -1, cur_weight = -1;
              input >> n >> m;     //read vertex number and edge number
              g.ver_num = n;
              g.edge_num = m;
              g.tree = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>());   //initialize tree

              for (int i = 0; i < m; ++i) {
                     input >> u >> v >> weight;

                     if (u == cur_u && v == cur_v) {
                            //there are multiple edges between u and v
                            //select the least-weighted edge to store
                            if (weight < cur_weight) {
                                   //clear the last data of adjacency lists of u and v
                                   g.graph.pop_back();
                                   cur_weight = weight;
                            }
                            else continue;
                     }

                     else if (u != cur_u || v != cur_v)        //a new pair of vertices
                            cur_weight = weight;

                     //put data into adjacency lists of u and v
                     g.graph.push_back(make_tuple(weight, u, v));
                     cur_u = u;
                     cur_v = v;
              }

       }

       return g;
}

/*Find Function of Disjoint-set, return the root of which subset that x belongs*/
int Find(vector<int> &parent, int x)
{
       int a = x;
       while (x != parent[x])
              x = parent[x];
       while (a != parent[a]) {
              int z = a;
              a = parent[a];
              parent[z] = x;
       }
       return x;
}


int computeMST(Graph& g)
{
       int n = g.ver_num;          //number of vertices
       int m = g.edge_num;         //m: number of edges
       vector<int> parent(n);                                 //并查集数组
       int MSTweight = 0;                                           //所求边权之和
       int MSTedge = 0;                                       //记录最小生成树边数
       for (int i = 0; i < n; i++)                            //初始化并查集
              parent[i] = i;

       sort(g.graph.begin(), g.graph.end());                         //所有边按边权从小到大排序

       for (int i = 0; i < m; ++i)                            //枚举所有边
       {
              int u = get<1>(g.graph[i]);
              int v = get<2>(g.graph[i]);
              int weight = get<0>(g.graph[i]);
              int parent_u = Find(parent, u);           //查询端点u所在集合的根结点
              int parent_v = Find(parent, v);           //查询端点v所在集合的根结点
              if (parent_u != parent_v) {                               //如果不在一个集合中
                     parent[parent_u] = parent_v;                       //合并集合（相当于把测试边加入到最小生成树）
                     MSTweight += weight;
                     MSTedge++;                               //当前生成树边数加1

                     g.tree[u].push_back(make_pair(v, weight));
                     g.tree[v].push_back(make_pair(u, weight));
                     //g.tree.push_back(g.graph[i]);             //store the node into MST
                     if (MSTedge == n - 1)                    //边数等于顶点数减1，算法结束
                            break;
              }
       }
       if (MSTedge != n - 1) {
              cout << "MSTedge Error! MSTedge = " << MSTedge << endl;
              return -1;
       }                                  //无法连通时返回-1

       else {
              g.MSTweight = MSTweight;
              return MSTweight;           //返回最小生成树边权之和
       }
}

bool DFS(Graph& g, int current, int v, vector<bool>& visit, vector<int>& path) {
       visit[current] = true;
       if (current == v) {
              //reach the destination
              return true;
       }
       for (auto edge_curr : g.tree[current]) {
              //If edge of u is not visited, DFS the other node of this edge
              if (visit[edge_curr.first] == false) {
                     path.push_back(edge_curr.first);
                     if (!visit[edge_curr.first] && DFS(g, edge_curr.first, v, visit, path))
                            return true;
                     path.pop_back();
              }
       }
       return false;
}

int recomputeMST(int u, int v, int weight, Graph& g) {
       int n = g.ver_num;
       int m = g.edge_num;
       int MSTweight = g.MSTweight;

       for (auto& edge : g.tree[u]) {
              //Search adjacent list of node u to see if (u,v) is already in
              if (v == edge.first) {
                     if (weight < edge.second) {
                            //the new weight is smaller, change the MSTweight
                            MSTweight = MSTweight - edge.second + weight;
                            g.MSTweight = MSTweight;
                            edge.second = weight;

                            //change data in adjacency list of node v
                            for (auto& edge : g.tree[v]) {
                                   if (u == edge.first) {
                                          edge.second = weight;
                                          break;
                                   }
                            }
                     }
                     //Else, the new weight is bigger, MST will not be changed
                     //cout << "no dfs" << endl;
                     return MSTweight;
              }
       }
       //If edge (u,v) is not in the current MST,
       //add (u,v) will create a cycle.
       //Delete the max element in the cycle.
       vector<bool>visit(g.tree.size(), false);
       vector<int>path;
       path.push_back(u);

       if (DFS(g, u, v, visit, path)) {
              //a cycle was found
              int biggest = 0, index = -1;
              for (int i = 0; i < path.size() - 1; i++) {
                     for (auto edge : g.tree[path[i]]) {
                            if (edge.first == path[i + 1]) {
                                   if (edge.second > biggest) {
                                          biggest = edge.second;
                                          index = i;
                                          //cout << index << endl;
                                          //use index to store the position of the biggest edge in path[]
                                          //which is path[i] -> path[i + 1]
                                   }
                                   break;
                            }
                     }
              }
              if (weight < biggest) {

                     //g.printMST();
                     //cout << "edge is changed" << endl;
                     //substitute the biggest-weighted edge

                     int i = 0;
                     for (auto& edge : g.tree[path[index]]) {
                            //find the edge to erase
                            if (edge.first == path[index + 1]) {
                                   g.tree[path[index]].erase(g.tree[path[index]].begin() + i, g.tree[path[index]].begin() + i + 1);
                                   //cout << "erase" << endl;
                                   //g.printMST();
                                   break;
                            }
                            i++;   //the position in adjacency list
                     }
                     i = 0;
                     for (auto& edge : g.tree[path[index + 1]]) {
                            if (edge.first == path[index]) {
                                   g.tree[path[index + 1]].erase(g.tree[path[index + 1]].begin() + i, g.tree[path[index + 1]].begin() + i + 1);
                                   //cout << "erase" << endl;
                                   //g.printMST();
                                   break;
                            }
                            i++;
                     }
                     g.tree[u].push_back(make_pair(v, weight));
                     //.printMST();
                     g.tree[v].push_back(make_pair(u, weight));
                     //g.printMST();
                     MSTweight = MSTweight - biggest + weight;
              }
              //else cout << "the biggest is the new edge" << endl;
       }

       g.MSTweight = MSTweight;
       //path.clear();
       return MSTweight;
}



int main(int argc, char *argv[]) {

       /*
       1. inputs: graph file, change file, name of output file
       2. parseEdges to parse graph file
       3. calculate MST (returns integer, weight of MST); we print this integer to the output file
       4. loop through change file, call function pass in new edge and MST

       */

       if (argc < 4) {
              cout << "Usage: " << argv[0] << " <graph_file> <change_file> <output_file>" << endl;
              return 1;
       }

       string graph_file = argv[1];
       string change_file = argv[2];
       string output_file = argv[3];

       ofstream output;
       output.open(output_file);

       //Write this function to parse edges from graph file to create your graph object
       Graph G = parseEdges(graph_file);
       //cout << "Graph: " << endl;
       //G.printGraph();

       //Run your MST function on graph G and collect as output the total weight of the MST
       clock_t startMST = clock();
       int MSTweight = computeMST(G);
       clock_t endMST = clock();
       //cout << "\nMST: \n" << MSTweight << endl;
       //G.printMST();
       //int newMSTWeight1 = recomputeMST(4, 9, 1, G);
       //cout << newMSTWeight1 << endl;
       //G.printMST();

       //cout << startMST << endl;
       //cout << endMST << endl;
       //Subtract the start time from the finish time to get the actual algorithm running time
       float totalTime = float(endMST - startMST) / (float) CLOCKS_PER_SEC;
       //printf("%f\n", totalTime);
       //Write initial MST weight and time to output file
       output << MSTweight << " " << totalTime << endl;

       //Iterate through changes file
       ifstream changes(change_file);

       int newMSTWeight = -1;

       if (changes.is_open()) {
              int numChanges;
              changes >> numChanges; //read number of changes

              int counter = 0;
              clock_t startRecompute = clock();
              while (counter < numChanges) {
                     int u, v, weight;
                     changes >> u; //read u
                     changes >> v; //read v
                     changes >> weight; //read w(u,v)
                     //cout << "counter = " << counter << endl;
                     //Run your recomputeMST function to recalculate the new weight of the MST given the addition of this new edge
                     //Note: you are responsible for maintaining the MST in order to update the cost without recalculating the entire MST
                     clock_t startNewMST = clock();
                     newMSTWeight = recomputeMST(u, v, weight, G);
                     clock_t endNewMST = clock();

                     //cout << newMSTWeight << endl;
                     //G.printMST();
                     float totalNewMST = float(endNewMST - startNewMST) / (float) CLOCKS_PER_SEC;
                     //printf("%f\n", totalNewMST);
                     //Write new weight and time to output file
                     output << newMSTWeight << " " << totalNewMST << endl;

                     counter++;
              }
              clock_t endRecompute = clock();
              cout << "Total time for recompute: " << 1000 * float(endRecompute - startRecompute) / (float) CLOCKS_PER_SEC<<endl;
              changes.close();
       }

       output.close();


       return 0;

}
