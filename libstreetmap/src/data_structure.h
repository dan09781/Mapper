/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   data_structure.h
 * Author: leejoo23
 *
 * Created on February 21, 2016, 11:00 PM
 */

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <algorithm> 
#include "m1.h"
#include "m2.h"
#include "LatLon.h"
#include "OSMDatabaseAPI.h"
//#include "milestone_3_graph.h"


using namespace std;


class DataStructure {
public:
    
    /******************************************** Milestone 1 Data Structures **********************************************************************/
    static unordered_multimap <string, vector<unsigned>> street_name_and_its_intersections; // Hash table with vectors of intersections for each street

    static unordered_map <int, unordered_map<int, vector<unsigned>>> to_find_closest_intersection; // 2-D array hold a vector of intersections

    static unordered_multimap <string, unsigned> street_name_to_street_id; // Hash table with vectors of street id's of each street

    static vector <unsigned> street_segments_idx; // Vector of street segment ids

    static vector <vector<unsigned>> intersection_with_street_segment_idx; // Vector with vectors of street segment ids at each intersections

    static unordered_map < unsigned, vector< unsigned>> street_id_to_street_seg; // Hash table with vectors of street segment ids for each street id

    static unordered_map <unsigned, vector<string>> intersection_street_names_from_intersection_ID; // Hash table with vectors of street names for each intersection

    static unordered_map <unsigned, vector<unsigned>> street_id_and_intersections; // Hash table with vectors of street ids at each intersection

    static unordered_map <unsigned, vector<StreetSegmentInfo>> hash_of_street_segment_info_at_intersection; // Hash table with vectors of StreetSegmentInfos at each intersection 

    static  vector <StreetSegmentInfo> street_segment_info; // Vector of StreetSegmentInfos

    static unordered_map<unsigned, vector<double>> street_length_finder;

    static unordered_map <int, unordered_map<int, vector<unsigned>>> to_find_closest_POI;




    /**************************************************** Milestone 2 Data Structures *********************************************/
//    float minLon, minLat, maxLon, maxLat;
//    float x_start, y_start, x_finish, y_finish;
//    float startArea;
//    float lat_average;
//    float temp_Area;
//
//    float previous_Area, current_Area;
//    bool highlight_inter = false;
//    bool display_transit = false;
//    bool printed_once = false;
//
//    float clicked_x, clicked_y, temp_lon_inter, temp_lat_inter;
    static vector <unsigned> intersection_ids;

    static vector <unsigned> train_stns;
    static vector <unsigned> subway_tracks;
    static vector <unsigned> train_nodes;
    static vector <unsigned> bus_stops;
    static vector <unsigned> bus_routes;
    static vector <unsigned> selected_bus_route;
    static vector <unsigned> bus_ways;
    static unordered_map < OSMID, unsigned> OSM_street_segs_num_of_lanes;

    static unordered_map < OSMID, unsigned> OSM_And_Node_ID;
    static unordered_map < OSMID, unsigned> OSM_And_Way_ID;
    static unordered_map <unsigned, unsigned> zoom_level_3_street_printed;
    static vector <unsigned> draw_found_intersections;
    static vector <unsigned> zoom_level_3_features;
    static vector <unsigned> zoom_level_1_features;
    static vector <unsigned> highway_street_segments;
    static  vector <unsigned> major_road_street_segments;
    static vector <unsigned> minor_road_street_segments;
    static vector <unsigned> highway_ramps;

    static vector <LatLon> amenity_cafe;
    static vector <LatLon> amenity_fast_food;
    static vector <LatLon> amenity_restaurant;
    static vector <LatLon> amenity_pub;
    static vector <LatLon> amenity_bar;
    static vector <LatLon> amenity_bank;
    static vector <LatLon> amenity_hospital;
    static vector <LatLon> amenity_library;
    static vector <LatLon> amenity_university;
    static vector <LatLon> amenity_pharmacy;
    static vector <LatLon> amenity_parking;
    static vector <LatLon> amenity_cinema;
    static vector <LatLon> amenity_gym;
    static vector <LatLon> amenity_else;
    static vector <LatLon> chosen_POIs;

    static vector <OSMID> vector_cafe;
    static vector <OSMID> vector_fast_food;
    static vector <OSMID> vector_restaurant;
    static vector <OSMID> vector_pub;
    static vector <OSMID> vector_bar;
    static vector <OSMID> vector_bank;
    static vector <OSMID> vector_hospital;
    static vector <OSMID> vector_library;
    static vector <OSMID> vector_university;
    static vector <OSMID> vector_pharmacy;
    static vector <OSMID> vector_parking;
    static vector <OSMID> vector_cinema;
    static vector <OSMID> vector_gym;
    static vector <OSMID> vector_else;
    static vector <OSMID> vector_POIs;
 
    static vector <unsigned> chosen_POI_names;


    static vector <StreetSegmentInfo> one_way_street_segment_info;

    static vector <unsigned> island_feature_id;
    static vector <unsigned> lake_feature_id;
    static vector <unsigned> stream_river_feature_id;
    static vector <unsigned> beach_feature_id;
    static vector <unsigned> pond_feature_id;
//    bool subway_button, bus_button;

    static unordered_map <OSMID, unsigned> POI_OSMIDS;
    static unordered_map <unsigned, vector <double> > street_id_with_distance_to_the_closest_intersection;
    
    static unordered_map <unsigned, vector<double> > street_seg_id_with_vector_of_length_of_the_biggest_street_seg;
    static unordered_map <unsigned, vector <LatLon> > lat_lon_of_longest_segment;
    static unordered_map <unsigned, double >feature_id_to_area_of_feature;
    /**************************************************** Milestone 3 Data Structures *********************************************/
    static vector <unsigned> shortest_path;
    static unordered_map < unsigned, unordered_map <unsigned, unsigned> >  intersection_to_street_segment; 
    static vector <unsigned> test;
    static vector <string> all_intersection_names;
    static unordered_map <string, unsigned> intersection_names_to_ids;
    static vector <string> all_POI_names;
    static unordered_multimap <string, unsigned> POI_names_to_ids;
    static map <unsigned, unsigned> POI_look_up_table;
    static vector <unsigned> testing_POI;
    
    static unordered_map < unsigned, unsigned > street_id_of_all_street_seg_id;
    static unordered_map < unsigned, LatLon > lat_lon_of_all_poi_ids;
    static unordered_map < unsigned, LatLon> lat_lon_of_all_intersection_ids;
    static unordered_map <unsigned, StreetSegmentInfo> street_seg_id_to_street_seg_info;
    static float speed_limit;
//    static Graph graph;
    
};

#endif /* DATA_STRUCTURE_H */

