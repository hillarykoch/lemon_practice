#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>
#include <lemon/lgf_reader.h>
#include <lemon/dfs.h>
#include <lemon/adaptors.h>
#include "pathenumeration.h"

using namespace lemon;
using namespace std;
    
int main() {
    ListDigraph gr;
    ListDigraph::NodeMap<int> dim(gr);
    ListDigraph::NodeMap<int> assoc(gr);
    ListDigraph::NodeMap<int> label(gr);
    Dfs<ListDigraph> dfs(gr);
    ListDigraph::ArcMap<bool> filter(gr);
    FilterArcs<ListDigraph> subgraph(gr, filter);
    Dfs<FilterArcs<ListDigraph> > sub_dfs(subgraph);
    ListDigraph::Node src;
    ListDigraph::Node trg;
    ListDigraph::Node old_node;
    ListDigraph::Node curr_node;
    std::vector<int> all_paths; // instantiate a resizable vector to contain all paths
    std::vector<int> temp; 
    int sz;
    // Read in Directed Graph from lgf.txt
    // "attributes" source and target are declared to be nodes and named src and trg
    digraphReader(gr, "lgf.txt")
      .nodeMap("dim", dim)
      .nodeMap("assoc", assoc)
      .nodeMap("label", label)
      .node("source", src)
      .node("target", trg)
      .run();

    int d = dim[trg]-1;
    
    std::cout << "A DAG is built with " << countNodes(gr) << " nodes and " << countArcs(gr) << " arcs." << std::endl;
    // Enumerate all paths using DFS and backtracking
    std::cout << "Paths:" << std::endl;
    int num_paths = 0; //, k = 0, n = countNodes(gr);
    int count;
    PathEnumeration enumeration(gr, src, trg);
    //while(somethingorother != NULL) { // NEED TO DETERMINE THE CORRECT STOPPING RULE
        // find initial path
        if(num_paths == 0) {
            // add the universal source and target nodes
            enumeration.push_first(src);
            enumeration.push_last(trg);
        	// Run DFS on full graph
    		dfs.run(src, trg);
    		for(ListDigraph::NodeIt n(gr); n != INVALID; ++n) {
    			if(dfs.reached(n) && gr.id(n) != gr.id(src) && gr.id(n) != gr.id(trg)) {
    				enumeration.push_at(1, n);
    			}
    		}
            // Add path to all_paths
            // d+2 = enumeration.len() when the enumeration is full
            for(int i=0; i < enumeration.len(); i++) {
                all_paths.push_back(gr.id(enumeration[i]));
            }
            curr_node = enumeration[d];
            enumeration.pop_last();
    		num_paths++;
            std::cout << "the current node is " << gr.id(curr_node) << std::endl;
        }
          else {
              
          }
        if(num_paths > 0) {
            if(enumeration.outArcs(curr_node) == 0) {
                old_node = curr_node;
                // turn all outArcs back to true and return the degree to true # out arcs
                enumeration.reset_outArcs(curr_node);
                // unfilter all nodes
                enumeration.reset_filter();
                // move curr_node one back in the path
                curr_node = enumeration[enumeration.len()-2];
                std::cout << "the current node is " << gr.id(curr_node) << std::endl;
                std::cout << "the current node has " << enumeration.outArcs(curr_node) << " outArcs" << std::endl;
                
                if(enumeration.outArcs(curr_node) > 1) {
                    // pop the last node (this automatically hides the arc decrements outArcs)
                    enumeration.pop_last();
                    std::cout << "the current node is " << gr.id(curr_node) << std::endl;
                    std::cout << "the current node has " << enumeration.outArcs(curr_node) << " outArcs" << std::endl;

                    // update the subgraph for dfs based on filter
                    for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
                        filter[a] = enumeration.filter(a);
                    }

                    // find a new path to the target node
                    std::cout << "running sub_dfs..." << std::endl;
                    sub_dfs.run(curr_node,trg);
                    // Add path to all_paths
                    sz = enumeration.len();
                    for(int i = 0; i < sz; i++) {
                        std::cout << "adding enumeration to all_paths" << std::endl;
                        all_paths.push_back(gr.id(enumeration[i]));
                    }
                    for(ListDigraph::NodeIt n(gr); n != INVALID; ++n) {
                        if(sub_dfs.reached(n) && gr.id(n) != gr.id(curr_node)) {
                            std::cout << "adding sub_dfs to all_paths" << std::endl;
                            temp.push_back(gr.id(n));
                        }
                    }
                    sort(temp.begin(), temp.end());
                    for(int i = 0; i < temp.size(); i++){
                        all_paths.push_back(temp[i]);
                    }
                    // Hide the most recent arc in the subgraph
                    for(ListDigraph::OutArcIt a(gr, curr_node); a != INVALID; ++a) {
                        if(gr.source(a) == curr_node && gr.id(gr.target(a)) == temp[0]) {
                            cout << "the source of arc is " << gr.id(gr.source(a));
                            cout << " and the target of arc is " << gr.id(gr.target(a)) << std::endl;
                            enumeration.negate_filter(a);
                            enumeration.decrement_arcs(curr_node);
                            cout << "the filter on that arc is " << enumeration.filter(a);
                        }
                    }
                    
                    // empty out temp vector
                    temp.clear();
                    // increment total paths found
                    num_paths++;
                }
                std::cout << "Printing all_paths...\n";
                sz = all_paths.size();
                for(int i=0; i < sz; i++) {
                    std::cout << all_paths[i];
                }
                std::cout << std::endl;
            } //else { // if (enumeration.outArcs(curr_node) != 0)
            if(enumeration.outArcs(curr_node) != 0) {
                // update the subgraph for dfs based on filter
                for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
                    filter[a] = enumeration.filter(a);
                }
                // find a new path to the target node
                std::cout << "running sub_dfs..." << std::endl;
                sub_dfs.run(curr_node,trg);
                // Add path to all_paths
                sz = enumeration.len();
                for(int i = 0; i < sz; i++) {
                    std::cout << "adding enumeration to all_paths" << std::endl;
                    all_paths.push_back(gr.id(enumeration[i]));
                }
                for(ListDigraph::NodeIt n(gr); n != INVALID; ++n) {
                    if(sub_dfs.reached(n) && gr.id(n) != gr.id(curr_node)) {
                        std::cout << "adding sub_dfs to all_paths" << std::endl;
                        temp.push_back(gr.id(n));
                    }
                }
                sort(temp.begin(), temp.end());
                for(int i = 0; i < temp.size(); i++){
                    all_paths.push_back(temp[i]);
                }
                // Hide the most recent arc in the subgraph
                for(ListDigraph::OutArcIt a(gr, curr_node); a != INVALID; ++a) {
                    if(gr.source(a) == curr_node && gr.id(gr.target(a)) == temp[0]) {
                        cout << "the source of arc is " << gr.id(gr.source(a));
                        cout << " and the target of arc is " << gr.id(gr.target(a));
                        //enumeration.negate_filter(a);
                        enumeration.decrement_arcs(curr_node);
                    }
                }
            
                // empty out temp vector
                temp.clear();
                // increment total paths found
                num_paths++;
            }
        }
        std::cout << "the current node is " << gr.id(curr_node) << std::endl;
        std::cout << "the current node has " << enumeration.outArcs(curr_node) << " outArcs" << std::endl;
        std::cout << "Printing all_paths...\n";
                sz = all_paths.size();
                for(int i=0; i < sz; i++) {
                    std::cout << all_paths[i];
                }
                std::cout << std::endl;
        // Making sure that the number of arcs in the instantiated graph is correct
        count = 0;
        for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
        	if(enumeration.filter(a) == true) {
        		count++;
        	}	
        }
        std::cout << "There are " << count << " arcs in the instantiation of enumeration." << std::endl;



// BELOW HERE IS SUPPOSED TO BE COMMENTED OK



    //}    

//int main() {
//	ListDigraph gr;
//    ListDigraph::NodeMap<int> dim(gr);
//    ListDigraph::NodeMap<int> assoc(gr);
//    ListDigraph::NodeMap<int> label(gr);
//    ListDigraph::Node src;
//    ListDigraph::Node trg;
//    ListDigraph::ArcMap<bool> filter(gr, false); // filter is instantiated to "false" on all arcs
//    Dfs<ListDigraph> dfs(gr);
//    FilterArcs<ListDigraph> subgraph(gr, filter);
//    Dfs<FilterArcs<ListDigraph> > sub_dfs(subgraph);  //(filterArcs(gr, filter));
//    
//    //int size = static_cast<int>(testpath.size());
//    //testpath[0] = 1;
//    //std::cout << testpath[0] << size << std::endl;
//
//    
//    // Read in Directed Graph from lgf.txt
//    // "attributes" source and target are declared to be nodes and named src and trg
//    digraphReader(gr, "lgf.txt")
//      .nodeMap("dim", dim)
//      .nodeMap("assoc", assoc)
//      .nodeMap("label", label)
//      .node("source", src)
//      .node("target", trg)
//      .run();
//
//    std::cout << "A DAG is built with " << countNodes(gr) << " nodes and " << countArcs(gr) << " arcs." << std::endl;
//    std::vector<int> testpath(dim[trg]-1);
//    int size = testpath.size();
//    std::cout << size;



//    int count = 0;
//  	for (ListDigraph::OutArcIt a(gr, src); a != INVALID; ++a) {
//		count++;
//		//_deg[_gr.target(a)]--;
//	}
//	std::cout << "the src node has " << count << " outgoing arcs." << endl;
//
//	int count2 = 0;
//	for (ListDigraph::InArcIt b(gr, trg); b != INVALID; ++b) {
//		count2++;
//	}
//	std::cout << "the trg node has " << count2 << " incoming arcs." << endl;
//
//	int count4 = 0;
//	for (ListDigraph::ArcIt d(gr); d != INVALID; ++d) {
//		count4++;
//	}
//	std::cout << "the full graph has " << count4 << " total arcs." << endl;
//
//	// Run DFS on full graph
//	dfs.run(src,trg);
//
//	for(ListDigraph::NodeIt n(gr); n != INVALID; ++n) {
//		if(dfs.reached(n)) {
//			std::cout << gr.id(n);
//			ListDigraph::Node pred = dfs.predNode(n);
//			if (pred != INVALID) {
//				std::cout << "<-" << gr.id(pred) << std::endl;
//			}
//		}
//	}
//	std::cout << std::endl;
//
//	// For some reason, filter won't instantiate as full graph, so do that here
//	for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
//		filter[a] = true;
//	}
//	std::cout << "The subgraph initially has " << countArcs(subgraph) << " arcs" << std::endl;
//
//
//	std::cout << "Filtering arcs..." << endl;
//	int count3 = 0;
//	for (ListDigraph::ArcIt c(gr); c != INVALID; ++c) {
//		if((count3 % 4) == 0) { // if even count
//			filter[c] = false;
//		}
//		count3++;
//	}
//
//	std::cout << "The subgraph has " << countArcs(subgraph) << " arcs" << std::endl;
//
//	// Run DFS on subgraph
//	sub_dfs.run(src,trg);
//	for(ListDigraph::NodeIt a(gr); a != INVALID; ++a) {
//		if(sub_dfs.reached(a)) {
//			std::cout << gr.id(a);
//			ListDigraph::Node pred = sub_dfs.predNode(a);
//			if (pred != INVALID) {
//				std::cout << "<-" << gr.id(pred) << std::endl;
//			}
//		}
//	}
//	std::cout << std::endl;
//
//	int i = 0;
//	for(ListDigraph::NodeIt a(gr); a != INVALID; ++a) {
//		if(sub_dfs.reached(a) && gr.id(a) != gr.id(src) && gr.id(a) != gr.id(trg)) {
//			testpath[i] = gr.id(a);
//			i++;
//		}
//	}
//
//	for(i = size-1; i >= 0; i--){
//		if(i == 0) {
//			std::cout << testpath[i];	
//		} else {
//			std::cout << testpath[i] << ", ";		
//		}
//	}
	

	return 0;
}