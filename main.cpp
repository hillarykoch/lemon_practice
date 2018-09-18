#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/dfs.h>
#include <lemon/adaptors.h>
#include <vector>
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
    digraphReader(gr, "lgf.txt")
      .nodeMap("label", label)
      .node("source", src)
      .node("target", trg)
      .run();
    int d = dim[trg]-1;
    
    std::cout << "A DAG is built with " << countNodes(gr) << " nodes and " << countArcs(gr) << " arcs." << std::endl;
    
    // Enumerate all paths using DFS and backtracking
    int num_paths = 0;
    PathEnumeration enumeration(gr, src, trg);

    //all_paths = findPath(gr, src, trg, enumeration, d, num_paths, temp, filter, curr_node);
    findPath(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node);
    cout << "finished!!! these are the paths:\n" << endl;
    for(int i = 0; i < all_paths.size(); i++){
        cout << all_paths[i] << endl;
    }
    

	return 0;
}