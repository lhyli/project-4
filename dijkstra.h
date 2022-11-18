

#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<vector<size_t>> dijkstra(const vector<vector<double>>& map, //return the vector which store the paths
    const vector<string>& names,
    string origin, string target,
    double& cost);

void print_paths(const vector<vector<size_t>>& paths,  //print paths 
    const vector<string>& names);

#endif