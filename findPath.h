#ifndef _FINDPATH_H
#define _FINDPATH_H

#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/dfs.h>
#include <lemon/adaptors.h>
#include <vector>
#include "pathenumeration.h"

using namespace lemon;
using namespace std;

// if we get all the way back to zero and then find a whole new path,
// then we need to pop that whole path onto enumeration again

void findPath(ListDigraph& gr,
              ListDigraph::Node src,
              ListDigraph::Node trg,
              PathEnumeration& enumeration,
              int d,
              int num_paths,
              vector<int>& all_paths,
              ListDigraph::ArcMap<bool>& filter,
              ListDigraph::Node& curr_node ) {

    cout << "\n\naaaaand we're in!" << endl;

    // STOPPING RULE
    while(!(enumeration.len() > 0 &&
          enumeration[0] == src &&
          enumeration.outArcs(enumeration[0]) == 0)) {

    	if(num_paths == 0) {
            cout << "inside the \"num_paths == 0\"" << endl;
    		Dfs<ListDigraph> dfs(gr);

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

            cout << "the pathlists are: ";
            for(int i = 0; i < all_paths.size(); i++) {
                cout << all_paths[i];
            }
            cout << endl;

            findPath(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node);

    	} else {
            cout << "inside the \"num_paths != 0\" aka an else statement" << endl;
    		ListDigraph::ArcMap<bool> filter(gr);
    		FilterArcs<ListDigraph> subgraph(gr, filter);
    		Dfs<FilterArcs<ListDigraph> > sub_dfs(subgraph);
    		ListDigraph::Node old_node;
    		std::vector<int> temp;
            int sz;

            cout << "the current node has " << enumeration.outArcs(curr_node) << " outArcs" << endl;

    //        if(curr_node == src && enumeration.outArcs(curr_node) > 0) {
    //            
    //        }

    		if(enumeration.outArcs(curr_node) == 0) {
                cout << "inside the \"num_paths != 0\" but the current node has 0 outArcs" << endl;
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
                    cout << "inside the current node had 0 outArcs but its predecessor has many" << endl;
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
                    cout << "managed the sub_dfs" << endl;

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

                    findPath(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node);

                } else if(enumeration.outArcs(curr_node) == 1) {
                    cout << "inside the current node had 0 outArcs and its predecessor has only 1" << endl;
                    // move curr_node one back in the path
                    old_node = curr_node;
                    enumeration.pop_last();
                    curr_node = enumeration[enumeration.len()-2];

                    // turn all outArcs back to true and return the degree to true # out arcs
                    enumeration.reset_outArcs(curr_node);

                    // unfilter all nodes
                    enumeration.reset_filter();

                    // Pop last element in pathlist
                    // (automatically decrements arcs and filters the edge)
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

                    findPath(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node);
                }
            } else { // if (enumeration.outArcs(curr_node) != 0)
            cout << "inside the current node had many outArcs" << endl;
                // update the subgraph for dfs based on filter
                for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
                    filter[a] = enumeration.filter(a);
                }

                // find a new path to the target node
                std::cout << "running sub_dfs..." << std::endl;
                sub_dfs.run(curr_node, trg);
                cout << "ran the sub_dfs" << endl;

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
                        enumeration.negate_filter(a); // this used to be commented out but that must be wrong i think
                        enumeration.decrement_arcs(curr_node);
                    }
                }
                // empty out temp vector
                temp.clear();

                // increment total paths found
                num_paths++;

                findPath(gr, src, trg, enumeration, d, num_paths, all_paths, filter, curr_node);
            }
        }
    }
}
   
#endif