

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "dijkstra.h"

const string path_separator = " -- ";

using namespace std;
// A Depth First Search based method to retrieve all the shortest paths from the source to the destination.
// We start from the destination and use the parent node information stored in the prev vector. It is possible 
// to have multiple parent nodes and thus we need to add all of them to the path list respectively. The search
// ends when the destination node meet the source node. 
void format_path_helper(const vector<vector<size_t>>& prev, size_t source, size_t target, vector<size_t> path, vector<vector<size_t>>& result)
{
    // recursion base case: add the source node to the path list and the path is generated.
    if (source == target)
    {
        path.push_back(source);
        result.push_back(path);
        return;
    }
    for (const auto parent : prev[target]) 
    {
        path.push_back(target);
        format_path_helper(prev, source, parent, path, result);
        path.pop_back();
    }
}
// This function is a wrapper of the format_path_helper and returns the path lists.  
vector<vector<size_t>> format_path(const vector<vector<size_t>>& prev, size_t source, size_t target)
{
    vector<vector<size_t>> result = {};
    vector<size_t> path = {};
    format_path_helper(prev, source, target, path, result);
    return result;
}

// This function prints all the paths using the input names mapped from the node indices.
// The delimiter " -- " is used to join the path.
void print_paths(
    const vector<vector<size_t>>& paths,
    const vector<string>& names)
{
    if (paths.empty())  //when there is no path
    {
        cout << "No path found!" << endl;
        return;
    }
    for (const auto& path : paths)
    {
        string s = "";
        auto rit = path.rbegin();
        for (; rit != path.rend() - 1; rit++)
        {
            s += names[*rit];
            s += path_separator;
        }
        s += names[*rit];
        cout << s << endl;
    }
}

// Inline function to find the index of the element in a vector.
size_t inline get_index_from_vector(const vector<string>& v, const string& key)
{
    auto it = find(v.begin(), v.end(), key);
    return distance(v.begin(), it);
}

// This is the core part of the dijkstra algorithm
// Give a source and a destination, find ALL of the shortest path with a in-place 
// modification towards the cost which stores the shortest path distance. 
// If no shortest path is found, an empty vector is returned and the cost is set to -1.
vector<vector<size_t>> dijkstra(const vector<vector<double>>& map,
    const vector<string>& names, string origin, string target, double& cost)
{
    auto size = names.size();
    vector<bool> visited(size, 0);
    vector<double> dist(size, INFINITY);
    vector<vector<size_t>> prev(size, vector<size_t>());

    size_t origin_index = get_index_from_vector(names, origin);
    size_t target_index = get_index_from_vector(names, target);

    // We use C++ (tree) set to utilize the priority queue (min-heap) property.
    // std::pair is used with the first element to be the distance weight and the 
    // second element to be the index of the node.
    set<pair<double, size_t>> pq;
    pq.insert({ 0, origin_index });
    dist[origin_index] = 0;

    // the process continues until the priority queue is empty.
    while (!pq.empty())
    {
        //auto [v_dist, v] = *pq.begin();
        //pq.erase(pq.begin());
        auto ele = *pq.begin();
        auto v_dist = ele.first;
        auto v = ele.second;
        pq.erase(ele);

        if (!visited[v])
        {
            visited[v] = 1;
            for (auto nb = 0; nb < size; nb++)
            {
                // only visit the neighbor nodes that have not been visited
                if ((map[v][nb] > 0) && (!visited[nb]))
                {
                    auto alt = dist[v] + map[v][nb];
                    if (alt <= dist[nb])
                    {
                        if (alt < dist[nb])
                        {
                            dist[nb] = alt;
                            prev[nb].clear();
                        }
                        prev[nb].push_back(v);
                        pq.insert({ dist[nb], nb });
                    }
                }
            }
        }
    }

    // modify cost variable in-place
    cost = dist[target_index] == INFINITY ? -1 : dist[target_index];

    // return ALL shortest paths
    return format_path(prev, origin_index, target_index);
}