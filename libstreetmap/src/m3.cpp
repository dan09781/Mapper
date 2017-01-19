#include "m1.h"
#include "m2.h"
#include "m3.h"
#include <vector>
#include "graphics.h"
#include <cfloat>
#include "StreetsDatabaseAPI.h"
#include "OSMEntity.hpp"
#include <iomanip>
#include <algorithm> 
#include "OSMDatabaseAPI.h"
#include <sstream>
#include "data_structure.h"
#include "milestone_3_graph.h"
#include <list>
#include <limits>
#include <queue>
#include <functional>
#include <time.h>
#include <unordered_map>

using namespace std;

/************ DIJKSTRA INITIALIZER ***********/
unsigned NO_PREV = 69696969;
unsigned NO_EDGE = 99999999;
double bestPathLen = 999999;
/*********************************************/


// Returns a path (route) between the start intersection and the end intersection, if one exists.

vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end) {
    vector<unsigned> path;
    bool path_found = false;

    set_all_node_visited_false(intersect_id_start); // resetting all the nodes and setting the starting node's pathLen to 0

    path_found = shortest_path(intersect_id_start, intersect_id_end); // if a path is found, returns true

    if (path_found) {
        path = bfsTraceBack(intersect_id_end); // if path found, traces back on the routes to return the path
    }
    
    return path;
}

// Returns the time required to travel along the path specified. 

double compute_path_travel_time(const vector<unsigned>& path) {

    double travel_time = 0;
    unsigned prev_street_id;
    unsigned street_id;

    for (auto i = path.begin(); i != path.end(); i++) {
        street_id = DataStructure::street_id_of_all_street_seg_id[(*i)]; // getting the street segment info 
        if (i == path.begin()) { // first time 
            prev_street_id = street_id;
            travel_time += find_street_segment_travel_time(*i); // calling m1 function to get travel time 
        } else {
            if (prev_street_id == street_id) {
                travel_time += find_street_segment_travel_time(*i); // if same street 
            } else {
                travel_time += find_street_segment_travel_time(*i); // if taking a turn
                travel_time += 0.25;
                prev_street_id = street_id;
            }
        }
    } 
    return travel_time;
}

// Returns a path to the closest POI from the starting intersection id 

vector<unsigned> find_path_to_point_of_interest(unsigned intersect_id_start, string point_of_interest_name) {
    vector<unsigned> path;
    unsigned closest_intersection;
    unsigned path_found = 0;

    set_all_node_visited_false(intersect_id_start);
    DataStructure::POI_look_up_table.clear(); // clear

    auto range = DataStructure::POI_names_to_ids.equal_range(point_of_interest_name); // putting all the id's of the POI into a map
    for (auto i = range.first; i != range.second; i++) {

        LatLon temp = DataStructure::lat_lon_of_all_poi_ids[i->second];
        closest_intersection = find_closest_intersection(temp); // find the closest intersections to all the poi id's
        DataStructure::POI_look_up_table.insert({(closest_intersection), (i->second)}); // put in a binary tree with intersection id as key and the poi id as value
    }

    path_found = shortest_path_POI(intersect_id_start); // finds the path and if found, returns the closest poi id 

    if (path_found != 0) { // if path_found isn't 0, a path is found 
        path = bfsTraceBack(path_found); // track back on the routes and return a path
    }

    return path;
}


// Finding if a path exists from an intersection to another intersection 

bool shortest_path(unsigned intersect_id_start, unsigned end_id) {
    priority_queue <waveElem, vector<waveElem>, compare > wavefront;
    wavefront.push(waveElem(Graph::hash_of_vertices[intersect_id_start], NO_EDGE, 0, NO_PREV));
    unsigned prev_street;

    while (!wavefront.empty() && wavefront.top().pathLen < bestPathLen) {
        waveElem curr = wavefront.top(); // get the minimum in the priority queue
        wavefront.pop(); // erase the element 

        unsigned curr_id = curr.vertex.intersection_id;

        if (curr.pathLen < Graph::hash_of_vertices[curr_id].pathLen) { // if the pathLen of the min of the priority queue is smaller than the node itself

            Graph::hash_of_vertices[curr_id].pathLen = curr.pathLen; // put in the values 
            Graph::hash_of_vertices[curr_id].reaching_edge_id = curr.edge_id;

            if (curr_id == end_id) { // if reached the destination intersection id 
                bestPathLen = Graph::hash_of_vertices[curr_id].pathLen;

                return true; // best path is found
            }

            for (auto i = Graph::hash_of_vertices[curr_id].adjacent_edges.begin(); i != Graph::hash_of_vertices[curr_id].adjacent_edges.end(); i++) {
                prev_street = DataStructure::street_id_of_all_street_seg_id[(*i).street_seg_id];
                if (Graph::hash_of_edges[(*i).street_seg_id].oneway == false) { // if not one way
                    if ((*i).from == curr_id) { // if the current node is the adjacent edge's from 
                        if ((Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time) < Graph::hash_of_vertices[(*i).to].pathLen) {
                            unsigned to = (*i).to;
                            Vertex to_vertex = Graph::hash_of_vertices[to];
                            if (prev_street == curr.previous_street || curr.previous_street == NO_PREV) { // if same street, don't add 0.25 (15seconds)
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time, prev_street));
                            } else {// if different street, add 0.25 (15seconds)
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + 0.25 + (*i).street_seg_travel_time, prev_street));
                            }
                        }
                    } else { // if the current node is the adjacent edge's to 
                        if ((Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time) < Graph::hash_of_vertices[(*i).from].pathLen) {
                            Vertex from_vertex = Graph::hash_of_vertices[(*i).from];
                            if (prev_street == curr.previous_street || curr.previous_street == NO_PREV) { // if same street, don't add 0.25 (15seconds)
                                wavefront.push(waveElem(from_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time, prev_street));
                            } else { // if different street, add 0.25 (15seconds)
                                wavefront.push(waveElem(from_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + 0.25 + (*i).street_seg_travel_time, prev_street));
                            }
                        }
                    }
                } else {

                    if (Graph::hash_of_edges[(*i).street_seg_id].oneway == true && Graph::hash_of_edges[(*i).street_seg_id].from == curr_id) {
                        if ((Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time) < Graph::hash_of_vertices[(*i).to].pathLen) {
                            unsigned to = (*i).to;
                            Vertex to_vertex = Graph::hash_of_vertices[to];
                            if (prev_street == curr.previous_street || curr.previous_street == NO_PREV) { // if same street, don't add 0.25 (15seconds)
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time, prev_street));
                            } else { // if different street, add 0.25 (15seconds)
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + 0.25 + (*i).street_seg_travel_time, prev_street));
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}


// Tracks back on the shortest route found and returns a vector of actual path 
vector<unsigned> bfsTraceBack(unsigned end_id) {

    vector <unsigned> path;

    unsigned prev_edge = Graph::hash_of_vertices[end_id].reaching_edge_id;

    unsigned set_node = end_id;
    while (prev_edge != NO_EDGE) {
        path.push_back(prev_edge); // put in vector 
        DataStructure::shortest_path.push_back(prev_edge);
        Edge previous = Graph::hash_of_edges[prev_edge];
        unsigned prev_from;
        if (previous.oneway == true) { // if one-way, from is from 
            prev_from = previous.from;
        } else {
            if (set_node == previous.from) { // if not one-way, and the last node visited is the from, previous from is the to
                prev_from = previous.to;
            } else
                prev_from = previous.from; // if not one-way, and the last node visited is the to, previous from is the from
        }
        prev_edge = Graph::hash_of_vertices[prev_from].reaching_edge_id; // getting the reaching edge id of the previous from 
        set_node = prev_from; // current node == set_node
    }

    reverse(path.begin(), path.end()); // since started from the end, the vector is backwards, needs to reverse

    return path;
}

void set_all_node_visited_false(unsigned intersect_id_start) {

    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        
        if (i == intersect_id_start)
            Graph::hash_of_vertices[i].pathLen = 0; // if the node is the start, set the pathLen to 0
        unsigned length = std::numeric_limits<unsigned>::max();
        Graph::hash_of_vertices[i].pathLen = length; // rest of the nodes to INF
        Graph::hash_of_vertices[i].reaching_edge_id = -1; // reaching edge to -1 
    }
    bestPathLen = 999999;
}

// Finding if a path exists from an intersection to a POI 

unsigned shortest_path_POI(unsigned intersect_id_start) {

    unsigned found = 0;

    map<unsigned, unsigned>::iterator iter;

    priority_queue <waveElem, vector<waveElem>, compare > wavefront;
    wavefront.push(waveElem(Graph::hash_of_vertices[intersect_id_start], NO_EDGE, 0, NO_PREV));

    unsigned prev_street;

    while (!wavefront.empty() && wavefront.top().pathLen < bestPathLen) {
        waveElem curr = wavefront.top(); // get the min
        wavefront.pop(); // erase 

        unsigned curr_id = curr.vertex.intersection_id;

        if (curr.pathLen < Graph::hash_of_vertices[curr_id].pathLen) { // if the pathLen of the min of the priority queue is smaller than the node itself

            Graph::hash_of_vertices[curr_id].pathLen = curr.pathLen;
            Graph::hash_of_vertices[curr_id].reaching_edge_id = curr.edge_id;

            iter = DataStructure::POI_look_up_table.find(curr_id); // see if the current node is one of the intersection id's that are closest to the POI's

            if (iter != DataStructure::POI_look_up_table.end()) { // if the current node is the closest intersection id of the closest poi to the start intersection id
                bestPathLen = Graph::hash_of_vertices[curr_id].pathLen;
                found = curr_id; // set it to true and continue 
            }
            for (auto i = Graph::hash_of_vertices[curr_id].adjacent_edges.begin(); i != Graph::hash_of_vertices[curr_id].adjacent_edges.end(); i++) {
                prev_street = DataStructure::street_id_of_all_street_seg_id[(*i).street_seg_id];

                if (Graph::hash_of_edges[(*i).street_seg_id].oneway == false) { // if not one way
                    if ((*i).from == curr_id) {
                        if ((Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time) < Graph::hash_of_vertices[(*i).to].pathLen) {
                            unsigned to = (*i).to;
                            Vertex to_vertex = Graph::hash_of_vertices[to];
                            if (prev_street == curr.previous_street || curr.previous_street == NO_PREV) // if same street, don't add 15 seconds 
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time, prev_street));
                            else // if different street, add 15 seconds 
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + 0.25 + (*i).street_seg_travel_time, prev_street));
                        }
                    } else {
                        if ((Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time) < Graph::hash_of_vertices[(*i).from].pathLen) {
                            Vertex from_vertex = Graph::hash_of_vertices[(*i).from];
                            if (prev_street == curr.previous_street || curr.previous_street == NO_PREV) // if same street, don't add 15 seconds 
                                wavefront.push(waveElem(from_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time, prev_street));
                            else // if different street, add 15 seconds 
                                wavefront.push(waveElem(from_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + 0.25 + (*i).street_seg_travel_time, prev_street));
                        }
                    }
                } else {
                    if (Graph::hash_of_edges[(*i).street_seg_id].oneway == true && Graph::hash_of_edges[(*i).street_seg_id].from == curr_id) {
                        // if one way and is in the right way
                        if ((Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time) < Graph::hash_of_vertices[(*i).to].pathLen) {

                            unsigned to = (*i).to;
                            Vertex to_vertex = Graph::hash_of_vertices[to];
                            if (prev_street == curr.previous_street || curr.previous_street == NO_PREV) // if same street, don't add 15 seconds 
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + (*i).street_seg_travel_time, prev_street));
                            else // if different street, add 15 seconds 
                                wavefront.push(waveElem(to_vertex, (*i).street_seg_id, Graph::hash_of_vertices[curr_id].pathLen + 0.25 + (*i).street_seg_travel_time, prev_street));
                        }
                    }
                }
            }
        }
    }

    return found;
}
