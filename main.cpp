#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/dfs.h>
#include <lemon/adaptors.h>
#include "pathenumeration.h"
#include "findPath.h"

using namespace lemon;
using namespace std;
    
int main() {
    ListDigraph gr;
    ListDigraph::NodeMap<int> dim(gr);
    ListDigraph::NodeMap<int> label(gr);
    Dfs<ListDigraph> dfs(gr);
    ListDigraph::ArcMap<bool> filter(gr);
    ListDigraph::Node src;
    ListDigraph::Node trg;
    ListDigraph::Node curr_node;
    std::vector<int> all_paths; // instantiate a resizable vector to contain all paths

    // Read in Directed Graph from lgf.txt
    // "attributes" source and target are declared to be nodes and named src and trg
    // dim gives which "layer" the given node lies in
    digraphReader(gr, "lgf.txt")
      .nodeMap("label", label)
      .nodeMap("dim", dim)
      .node("source", src)
      .node("target", trg)
      .run();

    ListDigraph::NodeMap<int> layer(gr);
    for(ListDigraph::NodeIt u(gr); u != INVALID; ++u) {
        layer[u] = dim[u];
    }

    int d = dim[trg]-1;
    
    // Enumerate all paths using DFS and backtracking
    int num_paths = 0; // when not 0, enter a different section of "findPath" function
    PathEnumeration enumeration(gr, src, trg);

    // findPath2(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node);
    findPath(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node, layer);

    cout << "finished! these are the paths:" << endl;
    for(int i = 0; i < all_paths.size(); i++){
        cout << all_paths[i] << endl;
    }

	return 0;
}