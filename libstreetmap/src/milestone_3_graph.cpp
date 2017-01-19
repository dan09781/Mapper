/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "StreetsDatabaseAPI.h"
#include <iomanip>
#include "data_structure.h"
#include "easygl_constants.h"
#include "graphics.h"
#include "milestone_3_graph.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

/************************** For Graph Class ***********************************/

//unordered_map <unsigned, Vertex > Graph::hash_of_vertices;
//unordered_map <unsigned, Edge > Graph::hash_of_edges;
vector < Vertex >  Graph::hash_of_vertices;
vector < Edge >  Graph::hash_of_edges;
vector <unsigned> Graph::all_vertices;
vector <unsigned> Graph::all_edges;
//Vertex waveElem::vertex;

Graph::Graph() {

    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        addvertex(i);
        addAdj(i);
    }

    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);
        unsigned temp_from = temp_info.from;
        unsigned temp_to = temp_info.to;
        bool temp_oneway = temp_info.oneWay;
        double temp_street_seg_travel_time = find_street_segment_travel_time(i);

        addedge(temp_from, temp_to, temp_oneway, i, temp_street_seg_travel_time);
    }
}

Graph::~Graph() {

}

void Graph::addvertex(unsigned temp_intersection_id) {

    Vertex temp_vertex = Vertex(temp_intersection_id);
    all_vertices.push_back(temp_intersection_id);
    hash_of_vertices.push_back(temp_vertex);
}

void Graph::addedge(unsigned temp_from, unsigned temp_to, bool temp_oneway, unsigned temp_street_seg_id, double temp_street_seg_travel_time) {
    Edge temp_edge = Edge(temp_from, temp_to, temp_oneway, temp_street_seg_id, temp_street_seg_travel_time);
    all_edges.push_back(temp_street_seg_id);
    hash_of_edges.push_back(temp_edge);

}

void Graph::addAdj(unsigned int_id) {
    vector <unsigned> street_segments = find_intersection_street_segments(int_id);
    for (auto i = street_segments.begin(); i != street_segments.end(); i++) {
        StreetSegmentInfo temp_info = getStreetSegmentInfo(*i);
        unsigned temp_from = temp_info.from;
        unsigned temp_to = temp_info.to;
        bool temp_oneway = temp_info.oneWay;
        double temp_street_seg_travel_time = find_street_segment_travel_time(*i);

        Edge temp_edge = Edge(temp_from, temp_to, temp_oneway, *i, temp_street_seg_travel_time);

        bool check_if = false;
        vector <Edge> check = hash_of_vertices[int_id].adjacent_edges;
        for (auto j = check.begin(); j != check.end(); j++){
            if ((*j).street_seg_id == (*i)){
                check_if = true;
            }
        }
                
        if (check_if == false) {
            hash_of_vertices[int_id].add_adj_edge(temp_edge);
        }
    }
}



/*************************** For Vertex Class ********************************/

Vertex::Vertex() {

}

Vertex::Vertex(unsigned temp_intersection_id) {
    intersection_id = temp_intersection_id;
    visited = false;
    //    adjacent_edges.push_back();
    reaching_edge_id = -1;
    unsigned length = std::numeric_limits<unsigned>::max();
    pathLen = length;
   


}

Vertex::~Vertex() {

}


void Vertex::add_adj_edge(Edge temp_edge) {
    adjacent_edges.push_back(temp_edge);
}


/**************************** For Edge Class **********************************/


Edge::Edge() {

}

Edge::Edge(unsigned temp_from, unsigned temp_to, bool temp_oneway, unsigned temp_street_seg_id, double temp_street_seg_travel_time) {
    from = temp_from;
    to = temp_to;
    oneway = temp_oneway;
    street_seg_id = temp_street_seg_id;
    street_seg_travel_time = temp_street_seg_travel_time;

}

Edge::~Edge() {

}

