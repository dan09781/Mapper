/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   milestone_3_graph.h
 * Author: leejoo23
 *
 * Created on February 28, 2016, 4:55 PM
 */

#ifndef MILESTONE_3_GRAPH_H
#define MILESTONE_3_GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <algorithm> 
#include "m1.h"
#include "m2.h"
#include "LatLon.h"
#include "OSMDatabaseAPI.h"

using namespace std;

class Edge {
public:
     unsigned to;
     unsigned from;
     bool oneway;
     unsigned street_seg_id;
     double street_seg_travel_time;

    Edge();
    Edge(unsigned temp_from, unsigned temp_to, bool temp_oneway, unsigned temp_street_seg_id, double temp_street_seg_travel_time);
    ~Edge();


    friend class Vertex;
    friend class Graph;
};

class Vertex {
public:
     unsigned intersection_id;
     bool visited;
     vector<Edge> adjacent_edges; // index as from's and vector of to's
     unsigned reaching_edge_id;
     double pathLen;
    

    Vertex();
    Vertex(unsigned temp_intersection_id);

    void add_adj_edge(Edge);

    ~Vertex();
    friend class Edge;
    friend class Graph;


};

class Graph {
public:
    static vector < Vertex > hash_of_vertices;
    static vector < Edge > hash_of_edges;
    static vector <unsigned> all_vertices;
    static vector <unsigned> all_edges;
    Graph();
    ~Graph();
    void addvertex(unsigned temp_intersection_id);
    void addedge(unsigned temp_from, unsigned temp_to, bool temp_oneway, unsigned temp_street_seg_id, double temp_street_seg_travel_time);
    void addAdj(unsigned int_id);
  
    
    friend class Edge;
    friend class Vertex;

};

struct waveElem {
    Vertex vertex;
    unsigned edge_id;
    double pathLen;
    unsigned previous_street;
   
    
    waveElem(Vertex v, unsigned id, double p_len,  unsigned prev_street) {
        vertex = v;
        edge_id = id;
        pathLen = p_len;        
        previous_street = prev_street;
       
    }           
};

 struct compare  
 {  
   bool operator()(const waveElem& l, const waveElem& r)  
   {  
       return l.pathLen > r.pathLen;  
   }  
 }; 

#endif /* MILESTONE_3_GRAPH_H */

