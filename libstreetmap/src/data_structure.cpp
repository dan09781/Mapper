/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "data_structure.h"
#include "m1.h"
#include "m2.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <algorithm>  
#include "LatLon.h"
#include "milestone_3_graph.h"
#include "OSMDatabaseAPI.h"


using namespace std;


/********************************************* Defining for Milestone 1 ***************************************************/

unordered_multimap <string, vector<unsigned>> DataStructure::street_name_and_its_intersections; // Hash table with vectors of intersections for each street

unordered_map <int, unordered_map<int, vector<unsigned>>> DataStructure::to_find_closest_intersection; // 2-D array hold a vector of intersections

unordered_multimap <string, unsigned> DataStructure::street_name_to_street_id; // Hash table with vectors of street id's of each street

vector <unsigned> DataStructure::street_segments_idx; // Vector of street segment ids

vector <vector<unsigned>> DataStructure::intersection_with_street_segment_idx; // Vector with vectors of street segment ids at each intersections

unordered_map < unsigned, vector< unsigned>> DataStructure::street_id_to_street_seg; // Hash table with vectors of street segment ids for each street id

unordered_map <unsigned, vector<string>> DataStructure::intersection_street_names_from_intersection_ID; // Hash table with vectors of street names for each intersection

unordered_map <unsigned, vector<unsigned>> DataStructure::street_id_and_intersections; // Hash table with vectors of street ids at each intersection

unordered_map <unsigned, vector<StreetSegmentInfo>> DataStructure::hash_of_street_segment_info_at_intersection; // Hash table with vectors of StreetSegmentInfos at each intersection 

vector <StreetSegmentInfo> DataStructure::street_segment_info; // Vector of StreetSegmentInfos

unordered_map<unsigned, vector<double>> DataStructure::street_length_finder;

unordered_map <int, unordered_map<int, vector<unsigned>>> DataStructure::to_find_closest_POI;



/********************************************* Defining for Milestone 2 ***************************************************/

vector <unsigned> DataStructure::intersection_ids;

vector <unsigned> DataStructure::train_stns;

vector <unsigned> DataStructure::subway_tracks;

vector <unsigned> DataStructure::train_nodes;

vector <unsigned> DataStructure::bus_stops;

vector <unsigned> DataStructure::bus_routes;

vector <unsigned> DataStructure::selected_bus_route;

vector <unsigned> DataStructure::bus_ways;

unordered_map < OSMID, unsigned> DataStructure::OSM_street_segs_num_of_lanes;

unordered_map < OSMID, unsigned> DataStructure::OSM_And_Node_ID;

unordered_map < OSMID, unsigned> DataStructure::OSM_And_Way_ID;

unordered_map <unsigned, unsigned> DataStructure::zoom_level_3_street_printed;

vector <unsigned> DataStructure::draw_found_intersections;

vector <unsigned> DataStructure::zoom_level_3_features;

vector <unsigned> DataStructure::zoom_level_1_features;

vector <unsigned> DataStructure::highway_street_segments;

vector <unsigned> DataStructure::major_road_street_segments;
vector <unsigned> DataStructure::minor_road_street_segments;
vector <unsigned> DataStructure::highway_ramps;

vector <LatLon> DataStructure::amenity_cafe;

vector <LatLon> DataStructure::amenity_fast_food;

vector <LatLon> DataStructure::amenity_restaurant;

vector <LatLon> DataStructure::amenity_pub;

vector <LatLon> DataStructure::amenity_bar;

vector <LatLon> DataStructure::amenity_bank;

vector <LatLon> DataStructure::amenity_hospital;

vector <LatLon> DataStructure::amenity_library;

vector <LatLon> DataStructure::amenity_university;

vector <LatLon> DataStructure::amenity_pharmacy;

vector <LatLon> DataStructure::amenity_parking;

vector <LatLon> DataStructure::amenity_cinema;

vector <LatLon> DataStructure::amenity_gym;

vector <LatLon> DataStructure::amenity_else;

vector <LatLon> DataStructure::chosen_POIs;

vector <OSMID> DataStructure::vector_cafe;

vector <OSMID> DataStructure::vector_fast_food;

vector <OSMID> DataStructure::vector_restaurant;

vector <OSMID> DataStructure::vector_pub;

vector <OSMID> DataStructure::vector_bar;

vector <OSMID> DataStructure::vector_bank;

vector <OSMID> DataStructure::vector_hospital;

vector <OSMID> DataStructure::vector_library;

vector <OSMID> DataStructure::vector_university;

vector <OSMID> DataStructure::vector_pharmacy;

vector <OSMID> DataStructure::vector_parking;

vector <OSMID> DataStructure::vector_cinema;

vector <OSMID> DataStructure::vector_gym;

vector <OSMID> DataStructure::vector_else;

vector <OSMID> DataStructure::vector_POIs;

vector <unsigned> DataStructure::chosen_POI_names;

vector <StreetSegmentInfo> DataStructure::one_way_street_segment_info;

vector <unsigned> DataStructure::island_feature_id;

vector <unsigned> DataStructure::lake_feature_id;

vector <unsigned> DataStructure::stream_river_feature_id;

vector <unsigned> DataStructure::beach_feature_id;

vector <unsigned> DataStructure::pond_feature_id;

unordered_map <OSMID, unsigned> DataStructure::POI_OSMIDS;

unordered_map <unsigned, vector <double>> DataStructure::street_id_with_distance_to_the_closest_intersection;

unordered_map <unsigned, vector<double>> DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg;

unordered_map <unsigned, vector <LatLon> > DataStructure::lat_lon_of_longest_segment;

/********************************************* Defining for Milestone 3 ***************************************************/
vector <unsigned>  DataStructure::shortest_path;

unordered_map <unsigned, double> DataStructure::feature_id_to_area_of_feature;

unordered_map < unsigned, unordered_map <unsigned, unsigned > > DataStructure::intersection_to_street_segment;  

vector <unsigned> DataStructure::test;

vector <string> DataStructure::all_intersection_names;

unordered_map <string, unsigned> DataStructure::intersection_names_to_ids;

vector <string> DataStructure::all_POI_names;

unordered_multimap <string, unsigned> DataStructure::POI_names_to_ids;

map <unsigned, unsigned> DataStructure::POI_look_up_table;

vector <unsigned> DataStructure::testing_POI;

unordered_map < unsigned, unsigned > DataStructure::street_id_of_all_street_seg_id;
unordered_map < unsigned, LatLon > DataStructure::lat_lon_of_all_poi_ids;
unordered_map < unsigned, LatLon> DataStructure::lat_lon_of_all_intersection_ids;
unordered_map < unsigned , StreetSegmentInfo> DataStructure::street_seg_id_to_street_seg_info;
float DataStructure::speed_limit;

//Graph DataStructure::graph;
