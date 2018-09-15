#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>
#include <lemon/lgf_reader.h>
#include <lemon/dfs.h>
#include <lemon/adaptors.h>
#include <stdexcept>

using namespace lemon;
using namespace std;

struct node {
    //int data;
    ListDigraph::Node data;
    node *next; // pointer called "next" points to node
};

class llist {
    private:
        node *head;
        node *tail;

    public:
        llist() {
            head = NULL;
            tail = NULL;
        }

    void insert_first(ListDigraph::Node value) {//int value) {
        node *temp = new node;
        temp->data = value;

        if(head == NULL) {
            head = temp;
            tail = temp;
            head->next = NULL;
            tail->next = NULL;
            temp = NULL;
        } else {
            temp->next = head;  // point the new node to head
            head = temp; // move head back one, to the new node
        }
    }

    void insert_last(ListDigraph::Node value) {//int value) {
        node *temp = new node;
        temp->data = value;
        temp->next = NULL; // temp doesnt point to anything
        if(head == NULL) {
            head = temp;
            tail = temp;
            temp = NULL;
        } else {
            tail->next = temp; // point current tail to temp
            tail = temp; // move tail to the new node
        }        
    }

    void insert_at(int index, ListDigraph::Node value) {
        // currently no exception handling for trying to access an invalid index
        node *temp = new node;
        node *curr = new node;
        node *prev = new node;
        temp->data = value;

        curr = head->next;
        prev = head;

        int count = 0;
        while(count < index - 1) {
            prev = curr;
            curr = curr->next;
            count++;   
        }
        prev->next = temp;
        temp->next = curr;
    }

    void remove_first() {
        if(head == tail) {
            // If there is only one node, point head and tail to null
            head = NULL;
            tail = NULL;
        }
        
        // currently not handling exception when list is empty

        node *temp = new node;
        temp = head;
        head = head->next;
        delete temp;
    }

    void remove_last() {
        if (head == tail) {
            // If there is only one node, point head and tail to null
            head = NULL;
            tail = NULL;
        }

        // currently not handling exception when list is empty

        node *curr = new node;
        node *prev = new node;
        curr = head;
        while(curr->next != NULL) { // start at head and iterate over llist
          prev = curr;
          curr = curr->next;    
        }
        tail=prev;
        prev->next=NULL;
        delete curr;
    }

    void remove_at(int index) {
        // currently no exception handling for trying to access an invalid index
        node *curr = new node;
        node *foll = new node;

        curr = head;
        int count = 0;
        while(count < index - 1) {
            foll = foll->next;
            curr = curr->next;
            count++;   
        }
        curr->next = foll->next;
        delete foll;
    }

    //int get_data(int index) {
    const ListDigraph::Node& operator[](int index) const {
        // currently not handling exception when index is too big
        node *curr = new node;
        curr = head;
        int count = 0;
        while(count < index) {
            // start at head and iterate over llist
            // this indexing starts at 0!!!
            count++;
            curr = curr->next;    
        }
        return curr->data;
    }

    const int len() const {
        if(head == NULL) { // if there is nothing
            return 0;
        } else {
            node *curr = new node;
            curr = head;
            int count = 1;
            while(curr->next != NULL) {
                count++;
                curr = curr->next;
            }
            return count;
        }
    }
};

class PathEnumeration {
    private:
        const ListDigraph& _gr;
        ListDigraph::Node _src;
        ListDigraph::Node _trg;
        ListDigraph::NodeMap<int> _deg;
        ListDigraph::NodeMap<bool> _visited;
    	ListDigraph::NodeMap<bool> _contains;
    	//std::vector<ListDigraph::Node> _pathlist;
        llist _pathlist;
    	ListDigraph::ArcMap<bool> _filter;
        int _len;
    	//Dfs<ListDigraph> _dfs(const ListDigraph& _gr);
        //FilterArcs<ListDigraph> _subgraph;
        //Dfs<FilterArcs<ListDigraph> > _sub_dfs;  //(filterArcs(gr, filter));
    
    public:
        PathEnumeration(const ListDigraph& gr, ListDigraph::Node& src, ListDigraph::Node& trg) : _gr(gr), _deg(gr), _visited(gr), _filter(gr), _contains(gr) {
            for (ListDigraph::NodeIt u(gr); u != INVALID; ++u) {
                _deg[u] = countOutArcs(gr, u);
            }
            for(ListDigraph::NodeIt u(gr); u != INVALID; ++u) {
                _visited[u] = false;
            }
            for (ListDigraph::ArcIt a(gr); a != INVALID; ++a){
            	_filter[a] = true; //initialize every path to be valid at start	
            }
        }
        
    int outArcs(const ListDigraph::Node& u) const {
        return _deg[u];
    }

	bool filter(const ListDigraph::Arc& u) const {
    	return _filter[u];
    }

    bool contains(const ListDigraph::Node& u) const {
        return _contains[u];
    }

    bool visited(const ListDigraph::Node& u) const {
        return _visited[u];
    }

    const ListDigraph::Node& operator[](int index) const {
        //return _pathlist[index];
        return _pathlist[index];
    } 
  
    void push_first(const ListDigraph::Node& u) {
        //_contains[u] = true; // the node is in the path
        _visited[u] = true; // the node has been visited
        _pathlist.insert_first(u);
    }

    void push_last(const ListDigraph::Node& u) {
        //_contains[u] = true; // the node is in the path
        _visited[u] = true; // the node has been visited
        _pathlist.insert_last(u);
    }

    void push_at(const int index, const ListDigraph::Node& u) {
        //_contains[u] = true; // the node is in the path
        _visited[u] = true; // the node has been visited
        _pathlist.insert_at(index, u);
    }

    void pop_last() {
        // have not added something to handle when sz < 2
        // though this should never happen here
        int sz = _pathlist.len();
        ListDigraph::Node u = _pathlist[sz-1]; // The tail
        ListDigraph::Node v = _pathlist[sz-2]; // The one before the tail
        _pathlist.remove_last();
        _contains[u] = false;

        for (ListDigraph::OutArcIt a(_gr, u); a != INVALID; ++a) {
            std::cout << "entered the outarcit" << std::endl;
            std::cout << "u is " << _gr.id(u) << std::endl;
            std::cout << "and v is " << _gr.id(v) << std::endl;
            if(_gr.source(a) == u && _gr.target(a) == v) {
                std::cout << "entered the double if" << std::endl;
                _filter[a] = false; // the edge is hidden in the subgraph
                _deg[_gr.source(a)]--;
            }
        }
    }

    void pop_first() {
        // have not added something to handle when sz < 2
        // though this should never happen here
        int sz = _pathlist.len();
        ListDigraph::Node u = _pathlist[0]; // The tail
        _pathlist.remove_first();
        _contains[u] = false;
    }

    void pop_at(const int index) {
        // have not added something to handle when sz < 2
        // though this should never happen here
        ListDigraph::Node u = _pathlist[index]; // The tail
        ListDigraph::Node v = _pathlist[index-1]; // The one before the tail
        _pathlist.remove_at(index);
        _contains[u] = false;

        for (ListDigraph::OutArcIt a(_gr, u); a != INVALID; ++a) {
            std::cout << "entered the outarcit" << std::endl;
            std::cout << "u is " << _gr.id(u) << std::endl;
            std::cout << "and v is " << _gr.id(v) << std::endl;
            if(_gr.source(a) == u && _gr.target(a) == v) {
                std::cout << "entered the double if" << std::endl;
                _filter[a] = false; // the edge is hidden in the subgraph
                _deg[_gr.source(a)]--;
            }
        }
    }

    const int len() {
        return _pathlist.len();
    }
};
    
    int main() {
        ListDigraph gr;
        ListDigraph::NodeMap<int> dim(gr);
        ListDigraph::NodeMap<int> assoc(gr);
        ListDigraph::NodeMap<int> label(gr);
        Dfs<ListDigraph> dfs(gr);
        ListDigraph::Node src;
        ListDigraph::Node trg;
        
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
        std::vector<int> all_paths(d); // instantiate a resizable vector to contain all paths 
        std::cout << "A DAG is built with " << countNodes(gr) << " nodes and " << countArcs(gr) << " arcs." << std::endl;
    
        // Enumerate all paths using DFS and backtracking
        std::cout << "Paths:" << std::endl;
        int num_paths = 0; //, k = 0, n = countNodes(gr);
        int count;
        PathEnumeration enumeration(gr, src, trg);
    
        if(num_paths == 0) {
            // add the universal source and target nodes
            enumeration.push_first(src);
            enumeration.push_last(trg);
            std::cout << "the length of the pathlist is " << enumeration.len() << std::endl;

        	// Run DFS on full graph
    		dfs.run(src, trg);
    		count = 0;
    		for(ListDigraph::NodeIt n(gr); n != INVALID; ++n) {
    			if(dfs.reached(n) && gr.id(n) != gr.id(src) && gr.id(n) != gr.id(trg)) {
    				all_paths[count] = gr.id(n);
    				enumeration.push_at(1, n);
                    //enumeration.push_last(n);
                    std::cout << "pushed" << std::endl;
                    std::cout << "the length of the pathlist is " << enumeration.len() << std::endl;
    				count++;
    			}
    		}
    		std::sort(all_paths.begin(), all_paths.end()); // Put it in order of how we progress through the graph
    		num_paths++;
        }
    
        // Making sure that the number of arcs in the instantiated graph is correct
        count = 0;
        for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
        	if(enumeration.filter(a) == true) {
        		count++;
        	}	
        }
        std::cout << "There are " << count << " arcs in the instantiation of enumeration." << std::endl;
    
        // Taking a look at which nodes are in the current enumeration
        std::cout << "the length of the pathlist is " << enumeration.len() << std::endl;
        for(int i=0; i < enumeration.len(); i++) {
        	std::cout << gr.id(enumeration[i]) << std::endl;
        }
    
    //
    //    count = 0;
    //    for(ListDigraph::ArcIt a(gr); a != INVALID; ++a) {
    //    	if(enumeration.filter(a) == true) {
    //    		count++;
    //    	}	
    //    }
    //    std::cout << "There are " << count << " arcs in the instantiation of enumeration.";
    //    
    
    
    //    while(deg(src) != 0) { // While we haven't filtered all outgoing arcs from the source node
    //
    //    }
    
    
    //    while (k >= 0) {
    //        x[k]++;
    //        if (x[k] == n) {
    //            x[k] = -1;
    //            k--;
    //            ordering.pop();
    //        } else {
    //            ListDigraph::Node u = nodes[x[k]];
    //            bool accept = !ordering.contains(u) && ordering.inArcs(u) == 0;
    //            if (accept) {
    //                ordering.push(u);
    //                if (k == n - 1) {
    //                    c++;
    //                    for (int i = 0; i < n; i++) {
    //                        cout << label[ordering[i]] << ' ';
    //                    }
    //                    cout << endl;
    //                    ordering.pop();
    //                } else {
    //                    k++;
    //                }
    //            }
    //        }
    //    }
    //    cout << c << " distinct orderings found." << endl;
    
    
    
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