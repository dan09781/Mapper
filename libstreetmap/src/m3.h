#pragma once
#include <string>
#include "StreetsDatabaseAPI.h"
#include <iomanip>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include "graphics.h"
#include "easygl_constants.h"
#include <list>
#include "milestone_3_graph.h"


// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end);


// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path);


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, std::string point_of_interest_name);

bool shortest_path(unsigned intersect_id_start, unsigned end_id);

std::vector<unsigned> bfsTraceBack(unsigned end_id);

//void draw_shortest_path (vector <unsigned> segments_shortest_path);
//
//void draw_edge_with_curve_point(LatLon position_1, LatLon position_2, StreetSegmentInfo info);
void set_all_node_visited_false(unsigned intersect_id_start);
double A_star_heuristic(unsigned a, unsigned b);
bool check_distances(unsigned a, unsigned b, unsigned c);
double heuristic (unsigned a, unsigned b);
unsigned shortest_path_POI(unsigned intersect_id_start);