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
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>


using namespace std;



/**************** Global Variables ************************/
float minLon, minLat, maxLon, maxLat;
float x_start, y_start, x_finish, y_finish;
float startArea;
float lat_average;
float temp_Area;
float x1Help, y1Help, x2Help, y2Help, x1Search, y1Search, x2Search, y2Search;

//Search bar variables
float search_bar_x1, search_bar_y1, inner_search_bar_x1, inner_search_bar_y1, inner_search_bar_y2, inner_search_bar_y3, inner_search_bar_y4, inner_search_bar_y5;
bool search_bar_clicked, search_bar_clicked_POI;
bool search_bar_show = true;
bool search_bar_show_POI = true;
bool matching_text_exists1 = false;
bool matching_text_exists2 = false;
bool matching_text_exists3 = false;
bool matching_text_exists4 = false;
bool matching_text_exists1_POI = false;
bool matching_text_exists2_POI = false;
bool matching_text_exists3_POI = false;
bool matching_text_exists4_POI = false;
bool find_intersection_enable = false;
bool find_POI_enable = false;
string final_inter;
string first_inter, second_inter, third_inter, fourth_inter, first_POI, second_POI, third_POI, fourth_POI;
vector <char> searched_text;
vector <char> searched_text_POI;
string searched;
string searched_POI;
unsigned searched_length, final_inter_id;
//End of search bar variables

//Displaying directions variables
bool display_directions_enable = false;
unsigned id1, id2;
//End of displaying directions variables

float previous_Area, current_Area;
float x_centre, y_title, x_centre_search, y_centre_search;

//Help screen variables
string help_title("Best Map NA Help Screen");
string title("How to use the map");
string general("General:");
string first_instruction("1. Use left mouse button or arrows on the interface to pan through the map");
string second_instruction("2. Use middle mouse wheel or zoom in/out buttons to zoom in or out");
string third_instruction("3. Click 'Zoom Fit' to go back to original zoom level");
string fourth_instruction("4. Click on an intersection on the map to view information about the intersection");
string fifth_instruction("5. Click 'Find' button and enter two street names to find the intersection between two streets");
string sixth_instruction("8. Click 'Clear' button to clear everything drawn on the map (intersection flag, subway map, etc)");
string spec_feat("Special Features:");
string seventh_instruction("6. Public Transit:");
string transit_instruction1("- Click 'Public Transit' button on the interface");
string transit_instruction2("- Enter between 'Subway' or 'Bus' to view corresponding transit map");
string transit_instruction3("- If you entered 'Bus', choose which bus route to view ('All' for all routes)");
string eighth_instruction("7. Display POI (Point of Interest):");
string POI_instruction1("- Click 'Display POI' button on the interface");
string POI_instruction2("- Enter the type of POI you want to view (or the number of that type on list)");
string POI_instruction3("- All POIs of entered type will be displayed on map");
string ninth_instruction("9. Click 'Find POI' to search for POI. Click 'FindIntersection' to search for intersection");
string tenth_instruction("10. Shortest path between two marks (POI or intersection) can be found by clicking 'Find Path'");
string back_instruction("Go back to map (right click)");
//End of help screen variables

bool highlight_inter = false;
bool display_transit = false;
bool printed_once = false;
bool subway_button, bus_button;
static bool display_help = false;
float clicked_x, clicked_y, temp_lon_inter, temp_lat_inter;
unsigned int_id1, int_id2;

/*********************************************/


void load_struct() {
    Graph graph;
    OSMID wayID;
    unsigned street_type;

    // Data Structure to differentiate different road types
    for (unsigned i = 0; i < getNumberOfWays(); i++) {
        const OSMWay* tempWay = getWayByIndex(i);
        wayID = tempWay->id();
        unsigned way_tag_count = getTagCount(tempWay);

        street_type = 0;

        for (unsigned j = 0; j < way_tag_count; j++) {
            std::pair<string, string> temp_pair = getTagPair(tempWay, j);
            if (temp_pair.first == "highway") {
                if (temp_pair.second == "primary" || temp_pair.second == "secondary" || temp_pair.second == "motorway" ||
                        temp_pair.second == "motorway_link" || temp_pair.second == "primary_link" || temp_pair.second == "secondary_link") {
                    if (temp_pair.second == "motorway") {
                        street_type = 3;
                        DataStructure::OSM_street_segs_num_of_lanes.insert({wayID, street_type});
                    } else if (temp_pair.second == "motorway_link") {
                        street_type = 4;
                        DataStructure::OSM_street_segs_num_of_lanes.insert({wayID, street_type});
                    } else {
                        street_type = 2;
                        DataStructure::OSM_street_segs_num_of_lanes.insert({wayID, street_type});
                    }
                }
                if (temp_pair.second == "tertiary" || temp_pair.second == "residential") {
                    street_type = 1;
                    DataStructure::OSM_street_segs_num_of_lanes.insert({wayID, street_type});
                } else {
                    street_type = 1;
                    DataStructure::OSM_street_segs_num_of_lanes.insert({wayID, street_type});
                }

            }
        }
    }

    // For when zoomed at street type == 3, only trying to print one name at each street 
    // Data structure for printing only one name per street
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {
        DataStructure::zoom_level_3_street_printed.insert({i, 0});
    }

    // Data Structure for "Display POI" button 
    // Storing different types of amenities
    for (unsigned i = 0; i < getNumberOfNodes(); i++) {

        const OSMNode* temp_node = getNodeByIndex(i);
        unsigned node_tag_count = getTagCount(temp_node);
        LatLon temp_LatLon = temp_node->coords();

        for (unsigned j = 0; j < node_tag_count; j++) {

            std::pair <string, string> temp_pair = getTagPair(temp_node, j);
            if (temp_pair.first == "amenity") {
                if (temp_pair.second == "cafe") {
                    DataStructure::amenity_cafe.push_back(temp_LatLon);
                    DataStructure::vector_cafe.push_back(temp_node->id());
                } else if (temp_pair.second == "fast_food") {
                    DataStructure::amenity_fast_food.push_back(temp_LatLon);
                    DataStructure::vector_fast_food.push_back(temp_node->id());
                } else if (temp_pair.second == "restaurant") {
                    DataStructure::amenity_restaurant.push_back(temp_LatLon);
                    DataStructure::vector_restaurant.push_back(temp_node->id());
                } else if (temp_pair.second == "pub") {
                    DataStructure::amenity_pub.push_back(temp_LatLon);
                    DataStructure::vector_pub.push_back(temp_node->id());
                } else if (temp_pair.second == "bar") {
                    DataStructure::amenity_bar.push_back(temp_LatLon);
                    DataStructure::vector_bar.push_back(temp_node->id());
                } else if (temp_pair.second == "bank") {
                    DataStructure::amenity_bank.push_back(temp_LatLon);
                    DataStructure::vector_bank.push_back(temp_node->id());
                } else if (temp_pair.second == "hospital") {
                    DataStructure::amenity_hospital.push_back(temp_LatLon);
                    DataStructure::vector_hospital.push_back(temp_node->id());
                } else if (temp_pair.second == "library") {
                    DataStructure::amenity_library.push_back(temp_LatLon);
                    DataStructure::vector_library.push_back(temp_node->id());
                } else if (temp_pair.second == "university") {
                    DataStructure::amenity_university.push_back(temp_LatLon);
                    DataStructure::vector_university.push_back(temp_node->id());
                } else if (temp_pair.second == "pharmacy") {
                    DataStructure::amenity_pharmacy.push_back(temp_LatLon);
                    DataStructure::vector_pharmacy.push_back(temp_node->id());
                } else if (temp_pair.second == "parking") {
                    DataStructure::amenity_parking.push_back(temp_LatLon);
                    DataStructure::vector_parking.push_back(temp_node->id());
                } else if (temp_pair.second == "cinema") {
                    DataStructure::amenity_cinema.push_back(temp_LatLon);
                    DataStructure::vector_cinema.push_back(temp_node->id());
                } else if (temp_pair.second == "gym") {
                    DataStructure::amenity_gym.push_back(temp_LatLon);
                    DataStructure::vector_gym.push_back(temp_node->id());
                } else {
                    DataStructure::amenity_else.push_back(temp_LatLon);
                    DataStructure::vector_else.push_back(temp_node->id());
                }
            }
        }
    }


    // Boolean variable to enable drawing subway routes
    subway_button = false;

    // Data structure for all train stations
    for (unsigned i = 0; i < getNumberOfNodes(); i++) {
        const OSMNode* tempNode = getNodeByIndex(i);
        unsigned nodeCount = getTagCount(tempNode);
        for (unsigned j = 0; j < nodeCount; j++) {
            pair<string, string> temp = getTagPair(tempNode, j);
            if (temp.first == "railway" && temp.second == "station") {
                DataStructure::train_stns.push_back(i);
            }

        }
    }

    // Data structure to get relations for all subway routes
    for (unsigned i = 0; i < getNumberOfRelations(); i++) {
        const OSMRelation* tempRelation = getRelationByIndex(i);
        unsigned relationCount = getTagCount(tempRelation);
        for (unsigned j = 0; j < relationCount; j++) {
            pair<string, string> temp = getTagPair(tempRelation, j);
            if (temp.first == "route" && temp.second == "subway") {
                DataStructure::subway_tracks.push_back(i);
            }
        }
    }


    // If members of the subway route relation is a way, put it in a vector 
    for (unsigned i = 0; i < DataStructure::subway_tracks.size(); i++) {
        const OSMRelation* tempRelation = getRelationByIndex(DataStructure::subway_tracks[i]);
        for (unsigned k = 0; k < tempRelation->members().size(); k++) {
            if (tempRelation->members()[k].type == Way)
                DataStructure::train_nodes.push_back(tempRelation->members()[k].id);
        }
    }

    // Data structure that gets node id from OSMid
    for (unsigned i = 0; i < getNumberOfNodes(); i++) {
        const OSMNode* tempNode = getNodeByIndex(i);
        DataStructure::OSM_And_Node_ID.insert({tempNode->id(), i});
    } //


    // Data structure to get one-way StreetSegmentInfos 
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);
        if (temp_info.oneWay == true) {
            DataStructure::one_way_street_segment_info.push_back(temp_info);
        }
    }

    // Data structure to get relations for all bus routes
    for (unsigned i = 0; i < getNumberOfRelations(); i++) {
        const OSMRelation* tempRelation = getRelationByIndex(i);
        unsigned relationCount = getTagCount(tempRelation);
        for (unsigned j = 0; j < relationCount; j++) {
            pair<string, string> temp = getTagPair(tempRelation, j);
            if (temp.first == "route" && temp.second == "bus") {
                DataStructure::bus_routes.push_back(i);
            }
        }
    }
    // Boolean variable to enable drawing bus routes
    bus_button = false;

    // Data structure that gets way id from OSMid
    for (unsigned i = 0; i < getNumberOfWays(); i++) {
        const OSMWay* tempWay = getWayByIndex(i);
        DataStructure::OSM_And_Way_ID.insert({tempWay->id(), i});
    }

    // Data structure for layering purpose
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        StreetSegmentInfo info = getStreetSegmentInfo(i);
        OSMID way_ID = info.wayOSMID;
        unsigned street_type = DataStructure::OSM_street_segs_num_of_lanes[way_ID];

        if (street_type == 4) {
            DataStructure::highway_ramps.push_back(i);
        } else if (street_type == 3) {
            DataStructure::highway_street_segments.push_back(i);
        } else if (street_type == 2) {
            DataStructure::major_road_street_segments.push_back(i);
        } else if (street_type == 1) {
            DataStructure::minor_road_street_segments.push_back(i);
        }
    }

    for (unsigned i = 0; i < getNumberOfFeatures(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(i);

        float area = 0;
        unsigned j = numOfFeatPoints - 1;
        for (unsigned k = 0; k < numOfFeatPoints; k++) {
            temp1 = getFeaturePoint(i, k);
            temp2 = getFeaturePoint(i, j);
            float p1_x = temp1.lon * DEG_TO_RAD * cos(lat_average);
            float p1_y = temp1.lat *DEG_TO_RAD;

            float p2_x = temp2.lon * DEG_TO_RAD * cos(lat_average);
            float p2_y = temp2.lat *DEG_TO_RAD;
            area = area + ((p1_x + p2_x)*(p2_y - p1_y));
            j = k;

        }
        area = abs(area / 2);
        DataStructure::feature_id_to_area_of_feature.insert({i, area});

    }

    // Data structure for layering purpose
    for (unsigned i = 0; i < getNumberOfFeatures(); i++) {

        FeatureType featType = getFeatureType(i);

        if (featType == FeatureType::Building) {
            DataStructure::zoom_level_1_features.push_back(i);
        } else if (featType == FeatureType::Island) {
            DataStructure::island_feature_id.push_back(i);
        } else if (featType == FeatureType::Lake) {
            bool entered_already = false;
            OSMID tempID = getFeatureOSMID(i);
            unsigned nodeID = DataStructure::OSM_And_Way_ID[tempID];
            const OSMWay* tempWay = getWayByIndex(nodeID);
            unsigned wayCount = getTagCount(tempWay);
            for (unsigned j = 0; j < wayCount; j++) {

                pair<string, string> temp = getTagPair(tempWay, j);
                if (temp.second == "lake") {
                    DataStructure::lake_feature_id.push_back(i);
                    entered_already = true;
                }
            }

            double area = DataStructure::feature_id_to_area_of_feature[i];

            if (area < 1e-7 && entered_already == false) {
                DataStructure::pond_feature_id.push_back(i);

            } else if (entered_already == false) {
                DataStructure::lake_feature_id.push_back(i);
            }


        } else if (featType == FeatureType::River || featType == FeatureType::Stream) {
            DataStructure::stream_river_feature_id.push_back(i);
        } else if (featType == FeatureType::Beach) {
            DataStructure::beach_feature_id.push_back(i);
        } else {
            DataStructure::zoom_level_3_features.push_back(i);
        }

    }
    // Data structure for "Display POI" button along with printing out the POI names 
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        OSMID tempID = getPointOfInterestOSMNodeID(i);
        DataStructure::POI_OSMIDS.insert({tempID, i});
    }



    // Initializing for the structure below  
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {
        DataStructure::street_id_with_distance_to_the_closest_intersection[i].push_back(0);
        DataStructure::street_id_with_distance_to_the_closest_intersection[i].push_back(0);
        DataStructure::street_id_with_distance_to_the_closest_intersection[i].push_back(0);
        DataStructure::street_id_with_distance_to_the_closest_intersection[i].push_back(0);
    }


    // Data structure for printing out street names
    // In order to print out the street name on the longest street segment of the street
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {

        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);

        LatLon temp1 = getIntersectionPosition(temp_info.from);
        LatLon temp2 = getIntersectionPosition(temp_info.to);

        //converting latitudes and longitudes to x-y coordinates 
        float temp1_lat = temp1.lat * DEG_TO_RAD;
        float temp1_lon = temp1.lon * DEG_TO_RAD;

        float temp2_lat = temp2.lat * DEG_TO_RAD;
        float temp2_lon = temp2.lon * DEG_TO_RAD;

        float lat_average = (maxLat + minLat) / 2;

        float x_start = temp1_lon * cos(lat_average);
        float x_finish = temp2_lon * cos(lat_average);
        float y_start = temp1_lat;
        float y_finish = temp2_lat;

        float x_avg = ((x_finish + x_start) / 2);
        float y_avg = ((y_finish + y_start) / 2);


        unsigned temp_street_id = temp_info.streetID;

        double distance = find_distance_between_two_points(temp1, temp2);

        double closest_distance = DataStructure::street_id_with_distance_to_the_closest_intersection[temp_street_id][0];

        if (closest_distance == 0 || distance < closest_distance) { // compares the distance
            DataStructure::street_id_with_distance_to_the_closest_intersection.erase(temp_street_id); // clear

            DataStructure::street_id_with_distance_to_the_closest_intersection[temp_street_id].push_back(distance); // store in the distance
            DataStructure::street_id_with_distance_to_the_closest_intersection[temp_street_id].push_back(x_avg); // x_average
            DataStructure::street_id_with_distance_to_the_closest_intersection[temp_street_id].push_back(y_avg); // y_average 
            DataStructure::street_id_with_distance_to_the_closest_intersection[temp_street_id].push_back(i); // the street segment id
        }
    }

    //initialize
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // distance 
        //DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // x_coordinate at which the name should be printed in 
        //DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // y_coordinate at which the name should be printed in 
        //DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // x_start
        //DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // y_start
        //DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // x_finish
        //DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(0); // y_finish


    }

    // getting the longest segment of the street segment 
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);
        unsigned num_curve_pts = temp_info.curvePointCount;

        if (num_curve_pts != 0) {


            LatLon temp1 = getIntersectionPosition(temp_info.from);
            LatLon temp2 = getStreetSegmentCurvePoint(i, 0);

            //converting latitudes and longitudes to x-y coordinates 
            double temp1_lat = temp1.lat * DEG_TO_RAD;
            double temp1_lon = temp1.lon * DEG_TO_RAD;

            double temp2_lat = temp2.lat * DEG_TO_RAD;
            double temp2_lon = temp2.lon * DEG_TO_RAD;

            double lat_average = (maxLat + minLat) / 2;

            double x_start = temp1_lon * cos(lat_average);
            double x_finish = temp2_lon * cos(lat_average);
            double y_start = temp1_lat;
            double y_finish = temp2_lat;

            double x_avg = ((x_finish + x_start) / 2);
            double y_avg = ((y_finish + y_start) / 2);

            double distance = find_distance_between_two_points(temp1, temp2);

            double closest_distance = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][0];

            if (closest_distance == 0 || closest_distance < distance) { // compares the distance
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg.erase(i); // clear

                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(distance); // store in the distance
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_avg); // x_average
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_avg); // y_average 

                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_start);
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_start);
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_finish);
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_finish);

                DataStructure::lat_lon_of_longest_segment.erase(i);

                DataStructure::lat_lon_of_longest_segment[i].push_back(temp1);
                DataStructure::lat_lon_of_longest_segment[i].push_back(temp2);

            }

            for (unsigned j = 0; j < num_curve_pts - 1; j++) {

                temp1 = getStreetSegmentCurvePoint(i, j);
                temp2 = getStreetSegmentCurvePoint(i, j + 1);

                //converting latitudes and longitudes to x-y coordinates 
                temp1_lat = temp1.lat * DEG_TO_RAD;
                temp1_lon = temp1.lon * DEG_TO_RAD;

                temp2_lat = temp2.lat * DEG_TO_RAD;
                temp2_lon = temp2.lon * DEG_TO_RAD;

                lat_average = (maxLat + minLat) / 2;

                x_start = temp1_lon * cos(lat_average);
                x_finish = temp2_lon * cos(lat_average);
                y_start = temp1_lat;
                y_finish = temp2_lat;

                x_avg = ((x_finish + x_start) / 2);
                y_avg = ((y_finish + y_start) / 2);

                distance = find_distance_between_two_points(temp1, temp2);

                closest_distance = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][0];

                if (closest_distance == 0 || closest_distance < distance) { // compares the distance
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg.erase(i); // clear

                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(distance); // store in the distance
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_avg); // x_average
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_avg); // y_average 
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_start);
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_start);
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_finish);
                    DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_finish);
                    DataStructure::lat_lon_of_longest_segment.erase(i);

                    DataStructure::lat_lon_of_longest_segment[i].push_back(temp1);
                    DataStructure::lat_lon_of_longest_segment[i].push_back(temp2);

                }


            }

            temp1 = getStreetSegmentCurvePoint(i, (num_curve_pts - 1));
            temp2 = getIntersectionPosition(temp_info.to);

            //converting latitudes and longitudes to x-y coordinates 
            temp1_lat = temp1.lat * DEG_TO_RAD;
            temp1_lon = temp1.lon * DEG_TO_RAD;
            temp2_lat = temp2.lat * DEG_TO_RAD;
            temp2_lon = temp2.lon * DEG_TO_RAD;

            lat_average = (maxLat + minLat) / 2;

            x_start = temp1_lon * cos(lat_average);
            x_finish = temp2_lon * cos(lat_average);
            y_start = temp1_lat;
            y_finish = temp2_lat;

            x_avg = ((x_finish + x_start) / 2);
            y_avg = ((y_finish + y_start) / 2);

            distance = find_distance_between_two_points(temp1, temp2);
            closest_distance = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][0];

            if (closest_distance < distance) { // compares the distance
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg.erase(i); // clear

                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(distance); // store in the distance
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_avg); // x_average
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_avg); // y_average 
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_start);
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_start);
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(x_finish);
                DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i].push_back(y_finish);
                DataStructure::lat_lon_of_longest_segment.erase(i);

                DataStructure::lat_lon_of_longest_segment[i].push_back(temp1);
                DataStructure::lat_lon_of_longest_segment[i].push_back(temp2);
            }

        }
    }

    //load all intersection names
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        string temp_int_name = getIntersectionName(i);
        DataStructure::all_intersection_names.push_back(temp_int_name);
    }

    //load all intersection ids corresponding to names
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        string temp_int_name = getIntersectionName(i);
        DataStructure::intersection_names_to_ids.insert({temp_int_name, i});
    }

    //load all POI names
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        string temp_int_name = getPointOfInterestName(i);
        DataStructure::all_POI_names.push_back(temp_int_name);
    }

    //load all POI ids corresponding to names
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        string temp_int_name = getPointOfInterestName(i);
        DataStructure::POI_names_to_ids.insert({temp_int_name, i});
    }
}

void draw_map() {
    load_struct();


    init_graphics("Best Map NA", t_color(0xE9, 0xE4, 0xDE));

    vector <double> max_min_lat_lons = return_lat_lons(); //Gets lat lons from data strucutre made in m1.cpp
    //Finds initial bounds of the map, using max lat, lons
    LatLon temp;
    temp = getIntersectionPosition(0);
    minLon = max_min_lat_lons[0] * DEG_TO_RAD;
    minLat = max_min_lat_lons[1] * DEG_TO_RAD;
    temp = getIntersectionPosition(getNumberOfIntersections() - 1);
    maxLon = max_min_lat_lons[2] * DEG_TO_RAD;
    maxLat = max_min_lat_lons[3] * DEG_TO_RAD;
    lat_average = (maxLat + minLat) / 2;
    //Bottom left of screen
    x_start = minLon * cos(lat_average);
    y_start = minLat;
    //Top right of screen
    x_finish = maxLon * cos(lat_average);
    y_finish = maxLat;

    // New buttons made for our project
    create_button("Window", "Help", act_on_help_button_func);
    create_button("Window", "FindIntersection", act_on_find_inter_func);
    create_button("Window", "Find POI", act_on_find_POI_func);
    create_button("Window", "Clear", act_on_clear_button_func);
    create_button("Window", "Display POI", act_on_display_POI_button_func);
    create_button("Window", "Find", act_on_find_button_func);
    create_button("Window", "Public Transit", act_on_display_public_transit_func);
    create_button("Window", "Find Path", act_on_find_path_button_func);
    create_button("Window", "Path To POI", act_on_find_path_POI_button_func);
    //    create_button("Window", "Find Path", act_on_find_path_button_func);
    t_bound_box initial_coords = t_bound_box(x_start, y_start, x_finish, y_finish);
    startArea = initial_coords.area();
    previous_Area = initial_coords.area();
    set_visible_world(initial_coords);
    event_loop(act_on_button_press, NULL, act_on_key_press, draw_screen);
    close_graphics();
}

void draw_screen() {

    clearscreen();

    // Function to draw features 
    draw_features();

    // Function to draw streets 
    draw_streets();

    // Function to draw intersection found by the "Find" button
    draw_found_highlighted();

    // Function to draw highlighted intersections
    draw_intersection_highlight();

    // Function to draw user-chosen POIs
    draw_POI_highlight();

    // Draws subway routes and stations when public transit button is pressed (when variable turns true)
    if (subway_button == true) {
        draw_subway_tracks();
        draw_train_stns();
    }

    // Draws bus routes when public transit button is pressed (when variable turns true)
    if (bus_button == true) {
        draw_bus_routes();
    }

    draw_shortests();

    //Draws help screen
    if (display_help == true)
        draw_help_screen();

    // Function to draw arrows on one-way roads
    oneway();

    // Function to print street names
    print_out_street_names();

    // Function to print out names for user-chosen POIs 
    print_out_POI_names();

    //Draw search bars
    if (find_intersection_enable == true) {
        if (search_bar_show == true)
            draw_search_bar();
        if (search_bar_clicked == false && search_bar_show == true) {
            searched_text.clear();
            draw_search_text();
        }
        if (search_bar_clicked == true)
            draw_searched_text();
    } else if (find_POI_enable == true) {
        if (search_bar_show_POI == true)
            draw_search_bar_POI();
        if (search_bar_clicked_POI == false && search_bar_show_POI == true) {
            searched_text_POI.clear();
            draw_search_text_POI();
        }
        if (search_bar_clicked_POI == true)
            draw_searched_text_POI();
    }


    //Display travel directions on screen
    if (display_directions_enable == true)
        display_directions();

    if (find_intersection_enable == true) {
        if (search_bar_clicked == true)
            set_keypress_input(true);
        else if (search_bar_clicked == false)
            set_keypress_input(false);
    } else if (find_POI_enable == true) {
        if (search_bar_clicked_POI == true)
            set_keypress_input(true);
        else if (search_bar_clicked_POI == false)
            set_keypress_input(false);
    }

    if (display_help == true)
        update_message("Instructions shown on screen");
    else
        update_message("Welcome to Group 92's Map");

}
//Function to draw all the streets, at different zoom levels. 

void draw_streets() {

    vector <unsigned> street_segs;
    StreetSegmentInfo info;
    LatLon temp_start, temp_finish;
    unsigned start, finish;

    //Gets the limits of the current screen, for selective display.
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();


    //Draws the vector of major road segments from data structure. (Main Roads)
    for (auto i = DataStructure::major_road_street_segments.begin(); i != DataStructure::major_road_street_segments.end(); i++) {

        info = getStreetSegmentInfo(*i);
        start = info.from;
        finish = info.to;
        temp_start = getIntersectionPosition(start);
        temp_finish = getIntersectionPosition(finish);

        unsigned num_of_curve_points = info.curvePointCount;

        // Selective display, checks if segment is inside current screen.
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            if (num_of_curve_points != 0) { //if there is curve points 

                draw_street_seg_with_curve_point(temp_start, getStreetSegmentCurvePoint(*i, 0), info); //calls draw function for streets, draws "from" intersection to first curve point

                for (unsigned k = 0; k < num_of_curve_points - 1; k++) { //prints seg

                    draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, k), getStreetSegmentCurvePoint(*i, k + 1), info); //draws segments curve points
                }

                draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, num_of_curve_points - 1), temp_finish, info); //draws last curve point to the "to" intersection
            } else //if there were no curve points

                draw_street_seg_with_curve_point(temp_start, temp_finish, info);



        }
    }
    //Draws the minor roads from structure made in load struct. (Residential Streets, and other)
    if (temp_Area / startArea < 0.1) {
        for (auto i = DataStructure::minor_road_street_segments.begin(); i != DataStructure::minor_road_street_segments.end(); i++) {

            info = getStreetSegmentInfo(*i);
            start = info.from;
            finish = info.to;
            temp_start = getIntersectionPosition(start);
            temp_finish = getIntersectionPosition(finish);

            unsigned num_of_curve_points = info.curvePointCount;

            // Selective display, checks if segment is inside current screen.
            if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                    ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

                if (num_of_curve_points != 0) { //if there is curve points 

                    draw_street_seg_with_curve_point(temp_start, getStreetSegmentCurvePoint(*i, 0), info);

                    for (unsigned k = 0; k < num_of_curve_points - 1; k++) {

                        draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, k), getStreetSegmentCurvePoint(*i, k + 1), info);
                    }

                    draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, num_of_curve_points - 1), temp_finish, info);

                } else
                    draw_street_seg_with_curve_point(temp_start, temp_finish, info);

            }
        }
    }
    //Draws highway ramps for large highways, from vector made in load struct
    for (auto i = DataStructure::highway_ramps.begin(); i != DataStructure::highway_ramps.end(); i++) {

        info = getStreetSegmentInfo(*i);
        start = info.from;
        finish = info.to;
        temp_start = getIntersectionPosition(start);
        temp_finish = getIntersectionPosition(finish);

        unsigned num_of_curve_points = info.curvePointCount;

        // Selective display, checks if segment is inside current screen.
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            if (num_of_curve_points != 0) { //if there is curve points 

                draw_street_seg_with_curve_point(temp_start, getStreetSegmentCurvePoint(*i, 0), info);

                for (unsigned k = 0; k < num_of_curve_points - 1; k++) {
                    draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, k), getStreetSegmentCurvePoint(*i, k + 1), info);
                }

                draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, num_of_curve_points - 1), temp_finish, info);
            } else
                draw_street_seg_with_curve_point(temp_start, temp_finish, info);

        }
    }

    //Draws the highway street segments from strucutre made in load_struct
    for (auto i = DataStructure::highway_street_segments.begin(); i != DataStructure::highway_street_segments.end(); i++) {

        info = getStreetSegmentInfo(*i);
        start = info.from;
        finish = info.to;
        temp_start = getIntersectionPosition(start);
        temp_finish = getIntersectionPosition(finish);

        unsigned num_of_curve_points = info.curvePointCount;

        // Selective display, checks if segment is inside current screen.
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            if (num_of_curve_points != 0) { //if there is curve points 

                draw_street_seg_with_curve_point(temp_start, getStreetSegmentCurvePoint(*i, 0), info);

                for (unsigned k = 0; k < num_of_curve_points - 1; k++) {
                    draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, k), getStreetSegmentCurvePoint(*i, k + 1), info);
                }

                draw_street_seg_with_curve_point(getStreetSegmentCurvePoint(*i, num_of_curve_points - 1), temp_finish, info);
            } else
                draw_street_seg_with_curve_point(temp_start, temp_finish, info);
        }
    }

    //   vector <unsigned> moo = find_path_between_intersections(1043, 8873);

}

//Function that is called by draw_streets() and draws each segment with different sizes at different zoom levels. And different colors depending on street_type.

void draw_street_seg_with_curve_point(LatLon position_1, LatLon position_2, StreetSegmentInfo info) {

    float radius;
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();

    float point1_lat, point1_lon;
    float point2_lat, point2_lon;
    float lat_average;
    float x_start, x_finish;
    float y_start, y_finish;

    OSMID way_ID = info.wayOSMID;
    unsigned street_type;
    point1_lat = position_1.lat * DEG_TO_RAD;
    point1_lon = position_1.lon * DEG_TO_RAD;

    point2_lat = position_2.lat * DEG_TO_RAD;
    point2_lon = position_2.lon * DEG_TO_RAD;

    lat_average = (maxLat + minLat) / 2;

    x_start = point1_lon * cos(lat_average);
    x_finish = point2_lon * cos(lat_average);
    y_start = point1_lat;
    y_finish = point2_lat;

    //Gets current area of the screen, to compare it with starting area. 
    temp_Area = get_visible_world().area();
    street_type = DataStructure::OSM_street_segs_num_of_lanes[way_ID]; //gets street_type from database

    // Selective display, checks if segment is inside current screen.
    if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
            ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

        //Given different ratios between temp_area(current screen) and startArea (initial screen). The linewidths and what is drawn is different.
        //Line widths get larger, as the ratio between the temp and start get smaller.
        //Largest number, displays no minor roads.
        if (temp_Area / startArea < 0.0001) {//smallest zoom level, screen cannot zoom past this (Modifed Graphics.cpp, handle zoom function)
            radius = 400e-9;
            if (street_type == 4) { //Highway ramps
                setcolor(ORANGE);
                setlinewidth(28);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);


            } else if (street_type == 3) { //Highway
                setcolor(ORANGE);
                setlinewidth(30);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 580e-9, 0, 360);
                fillarc(x_finish, y_finish, 580e-9, 0, 360);

            } else if (street_type == 2) {// Major roads

                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(26);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 540e-9, 0, 360);
                fillarc(x_finish, y_finish, 540e-9, 0, 360);

            } else if (street_type == 1) { //Minor roads
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(22);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 510e-9, 0, 360);
                fillarc(x_finish, y_finish, 510e-9, 0, 360);

            }
        } else if (temp_Area / startArea < 0.0008) {
            radius = 410e-9;
            if (street_type == 4) {
                setcolor(ORANGE);
                setlinewidth(20);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);
            } else if (street_type == 3) {
                setcolor(ORANGE);
                setlinewidth(23);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);
            } else if (street_type == 2) {
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(15);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 440e-9, 0, 360);
                fillarc(x_finish, y_finish, 440e-9, 0, 360);


            } else if (street_type == 1) {
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(10);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, radius, 0, 360);
                fillarc(x_finish, y_finish, radius, 0, 360);
            }
        } else if (temp_Area / startArea < 0.001) {
            radius = 380e-9;
            if (street_type == 4) {
                setcolor(ORANGE);
                setlinewidth(11);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 410e-9, 0, 360);
                fillarc(x_finish, y_finish, 410e-9, 0, 360);
            } else if (street_type == 3) {
                setcolor(ORANGE);
                setlinewidth(14);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 410e-9, 0, 360);
                fillarc(x_finish, y_finish, 410e-9, 0, 360);
            } else if (street_type == 2) {

                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(10);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, radius, 0, 360);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 1) {
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(6);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 400e-9, 0, 360);
                fillarc(x_finish, y_finish, 400e-9, 0, 360);
            }
        } else if (temp_Area / startArea < 0.01) {
            radius = 250e-9;
            if (street_type == 4) {
                setcolor(ORANGE);
                setlinewidth(8);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 3) {
                setcolor(ORANGE);
                setlinewidth(10);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 2) {
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(6);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 1) {
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(2);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            }
        } else if (temp_Area / startArea < 0.1) {
            //cout << "0.5" << endl;
            if (street_type == 4) {
                setcolor(ORANGE);
                setlinewidth(5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 3) {
                setcolor(ORANGE);
                setlinewidth(7);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 2) {

                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(3.5);
                drawline(x_start, y_start, x_finish, y_finish);

            } else if (street_type == 1) {
                setcolor(0xDA, 0xD6, 0xCD);
                setlinestyle(SOLID);
                setlinewidth(1.5);
                drawline(x_start, y_start, x_finish, y_finish);
            }

        } else if (temp_Area / startArea < 10) { //Largest zoom level, past initial screen size

            if (street_type == 4) {
                setcolor(ORANGE);
                setlinewidth(4);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 3) {
                setcolor(ORANGE);
                setlinewidth(5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 2) {
                setcolor(WHITE);
                setlinestyle(SOLID);
                setlinewidth(3);
                drawline(x_start, y_start, x_finish, y_finish);

            }
        }
    }
}

void act_on_button_press(float x, float y, t_event_buttonPressed event) {

    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                               */
    float lat_average = (maxLat + minLat) / 2;
    float x_lon = (x / DEG_TO_RAD) / cos(lat_average);
    float y_lat = y / DEG_TO_RAD;

    // LatLon of the click
    LatLon clickedPos;
    clickedPos.lon = x_lon;
    clickedPos.lat = y_lat;

    if (display_help == true && x >= x1Help + x_centre * 0.7 && x <= x2Help - x_centre * 0.7 && y >= y1Help + y_title * 0.1 && y <= y1Help + y_title * 0.2) {
        set_visible_world(x_start, y_start, x_finish, y_finish);
        display_help = false;
        search_bar_show = true;
        search_bar_show_POI = true;
    }
    unsigned closestIntersection = find_closest_intersection(clickedPos); // finding the closest intersection respect to the click 
    //    int_id = closestIntersection;
    //    find_path_between_intersections(closestIntersection, );
    //    for (auto i = DataStructure::shortest_path.begin(); i != DataStructure::shortest_path.end(); i++) {
    //        StreetSegmentInfo info = getStreetSegmentInfo(*i);
    //        cout << "*From: " << info.from << " To:  " << info.to << "  " << *i << " " << endl;
    //    }
    LatLon interPos = getIntersectionPosition(closestIntersection); // LatLon of the closest intersection
    if (find_intersection_enable == true && x >= inner_search_bar_x1 && x <= x2Search && y >= inner_search_bar_y1 && y <= y2Search) {
        search_bar_clicked = true;
    } else if (find_intersection_enable == true && searched_text.size() != 0 && matching_text_exists1 == true && x >= search_bar_x1 && y >= inner_search_bar_y2 && x <= x2Search && y <= search_bar_y1) {
        searched_text.clear();
        copy(first_inter.begin(), first_inter.end(), back_inserter(searched_text));
        draw_searched_text();
    } else if (find_intersection_enable == true && searched_text.size() != 0 && matching_text_exists2 == true && x >= search_bar_x1 && y >= inner_search_bar_y3 && x <= x2Search && y < inner_search_bar_y2) {
        searched_text.clear();
        copy(second_inter.begin(), second_inter.end(), back_inserter(searched_text));
        draw_searched_text();
    } else if (find_intersection_enable == true && searched_text.size() != 0 && matching_text_exists3 == true && x >= search_bar_x1 && y >= inner_search_bar_y4 && x <= x2Search && y < inner_search_bar_y3) {
        searched_text.clear();
        copy(third_inter.begin(), third_inter.end(), back_inserter(searched_text));
        draw_searched_text();
    } else if (find_intersection_enable == true && searched_text.size() != 0 && matching_text_exists4 == true && x >= search_bar_x1 && y >= inner_search_bar_y5 && x <= x2Search && y < inner_search_bar_y4) {
        searched_text.clear();
        copy(fourth_inter.begin(), fourth_inter.end(), back_inserter(searched_text));
        draw_searched_text();
    } else if (find_POI_enable == true && x >= inner_search_bar_x1 && x <= x2Search && y >= inner_search_bar_y1 && y <= y2Search) {
        search_bar_clicked_POI = true;
    } else if (find_POI_enable == true && searched_text_POI.size() != 0 && matching_text_exists1_POI == true && x >= search_bar_x1 && y >= inner_search_bar_y2 && x <= x2Search && y <= search_bar_y1) {
        searched_text_POI.clear();
        copy(first_POI.begin(), first_POI.end(), back_inserter(searched_text_POI));
        draw_searched_text_POI();
    } else if (find_POI_enable == true && searched_text_POI.size() != 0 && matching_text_exists2_POI == true && x >= search_bar_x1 && y >= inner_search_bar_y3 && x <= x2Search && y < inner_search_bar_y2) {
        searched_text_POI.clear();
        copy(second_POI.begin(), second_POI.end(), back_inserter(searched_text_POI));
        draw_searched_text_POI();
    } else if (find_POI_enable == true && searched_text_POI.size() != 0 && matching_text_exists3_POI == true && x >= search_bar_x1 && y >= inner_search_bar_y4 && x <= x2Search && y < inner_search_bar_y3) {
        searched_text_POI.clear();
        copy(third_POI.begin(), third_POI.end(), back_inserter(searched_text_POI));
        draw_searched_text_POI();
    } else if (find_POI_enable == true && searched_text_POI.size() != 0 && matching_text_exists4_POI == true && x >= search_bar_x1 && y >= inner_search_bar_y5 && x <= x2Search && y < inner_search_bar_y4) {
        searched_text_POI.clear();
        copy(fourth_POI.begin(), fourth_POI.end(), back_inserter(searched_text_POI));
        draw_searched_text_POI();
    } else if ((x_lon <= interPos.lon + 0.0002 && x_lon >= interPos.lon - 0.0002) && (y_lat <= interPos.lat + 0.0002 && y_lat >= interPos.lat - 0.0002)) { // The range where we check by 
        DataStructure::intersection_ids.push_back(closestIntersection); // Push it into the vector "intersection_ids" and later, we draw and display information on it
        printed_once = true;
        search_bar_clicked = false;
        if (find_intersection_enable == true)
            draw_searched_text();
        else if (find_POI_enable == true)
            draw_searched_text_POI();
    } else if (search_bar_clicked == true && searched_text.size() == 0)
        search_bar_clicked = false;
    else if (search_bar_clicked_POI == true && searched_text_POI.size() == 0)
        search_bar_clicked_POI = false;

    draw_screen(); // Go back to draw_screen();

}

void draw_intersection_highlight() {

    for (unsigned k = 0; k < DataStructure::intersection_ids.size(); k++) {

        LatLon temp1 = getIntersectionPosition(DataStructure::intersection_ids[k]);
        float temp1_lat = temp1.lat * DEG_TO_RAD;
        float temp1_lon = temp1.lon * DEG_TO_RAD;
        float lat_average = (maxLat + minLat) / 2;
        float x_start = temp1_lon * cos(lat_average);
        float y_start = temp1_lat;
        setcolor(RED);

        fillarc(x_start, y_start + 0.0000039, 0.000002, 0, 180);
        t_point points[3];

        points [0] = {x_start, y_start};
        points [1] = {x_start + 0.000002, y_start + 0.000004};
        points [2] = {x_start - 0.000002, y_start + 0.000004};

        fillpoly(points, 3);
        setcolor(BLACK);
        fillarc(x_start, y_start + 0.0000039, 0.0000009, 0, 360);

        vector< string> street_names = find_intersection_street_names(DataStructure::intersection_ids[k]);

        if (printed_once == true) {
            cout << "At the intersection ID: " << DataStructure::intersection_ids[k] << " the intersection street names are: " << endl;
            for (auto i = street_names.begin(); i != street_names.end(); i++) {
                cout << *i << ". ";
            }
            cout << endl;
        }

    }
    printed_once = false;
}


//Draws features seperated by strctures, and in different orders, and zoom levels.

void draw_features() {
    float p1_lon, p1_lat, p2_lon, p2_lat;
    temp_Area = get_visible_world().area();


    //Draws all lake features first, as base layer
    for (auto i = DataStructure::lake_feature_id.begin(); i != DataStructure::lake_feature_id.end(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(*i);
        temp1 = getFeaturePoint(*i, 0);
        temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);
        //Gets lat, lon of feature
        p1_lon = temp1.lon;
        p1_lat = temp1.lat;
        p2_lon = temp2.lon;
        p2_lat = temp2.lat;
        setcolor(0xB3, 0xD6, 0xFF);

        t_point points[numOfFeatPoints - 1];
        //Makes the points of a polygon of the lake, and then fills it after.
        for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
            temp1 = getFeaturePoint(*i, k);
            p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average); //converts each point to x,y
            p1_lat = temp1.lat *DEG_TO_RAD;
            points [k] = {p1_lon, p1_lat};
        }
        fillpoly(points, numOfFeatPoints - 1);

    }
    //Draws all islnds
    for (auto i = DataStructure::island_feature_id.begin(); i != DataStructure::island_feature_id.end(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(*i);
        temp1 = getFeaturePoint(*i, 0);
        temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);


        p1_lon = temp1.lon;
        p1_lat = temp1.lat;
        p2_lon = temp2.lon;
        p2_lat = temp2.lat;
        setcolor(0xE9, 0xE4, 0xDE);

        t_point points[numOfFeatPoints - 1];

        for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
            temp1 = getFeaturePoint(*i, k);
            p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average);
            p1_lat = temp1.lat *DEG_TO_RAD;
            points [k] = {p1_lon, p1_lat};
        }
        fillpoly(points, numOfFeatPoints - 1);
    }
    //Draws Parks, Greenspaces, and other features that went into this structure in load_strcut
    for (auto i = DataStructure::zoom_level_3_features.begin(); i != DataStructure::zoom_level_3_features.end(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(*i);
        temp1 = getFeaturePoint(*i, 0);
        temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);
        p1_lon = temp1.lon;
        p1_lat = temp1.lat;
        p2_lon = temp2.lon;
        p2_lat = temp2.lat;

        FeatureType featType = getFeatureType(*i);

        if (featType == FeatureType::Park) {
            setcolor(0xCA, 0xE0, 0xAA);
        } else if (featType == FeatureType::Golfcourse) {
            setcolor(GREEN);
        } else if (featType == FeatureType::Greenspace) {
            setcolor(0xCA, 0xE0, 0xAA);
        } else if (featType == FeatureType::Shoreline) {
            setcolor(BLACK);
        } else if (featType == FeatureType::Unknown) {
            setcolor(0x8C, 0x8C, 0x8C);
        }

        //checking if the feature is open or closed, and then either draws a line(open), or polygon (closed)
        if ((p1_lon != p2_lon) && (p1_lat != p2_lat)) { //draw line
            for (unsigned k = 0; k < numOfFeatPoints - 2; k++) {
                // setlinewidth(1);
                drawline(getFeaturePoint(*i, k).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k).lat*DEG_TO_RAD, getFeaturePoint(*i, k + 1).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k + 1).lat * DEG_TO_RAD);
            }
        } else { //polygon
            t_point points[numOfFeatPoints - 1];

            for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
                temp1 = getFeaturePoint(*i, k);
                p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average);
                p1_lat = temp1.lat *DEG_TO_RAD;
                points [k] = {p1_lon, p1_lat};
            }
            fillpoly(points, numOfFeatPoints - 1);
        }
    }

    //draws streams and rivers
    for (auto i = DataStructure::stream_river_feature_id.begin(); i != DataStructure::stream_river_feature_id.end(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(*i);
        temp1 = getFeaturePoint(*i, 0);
        temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);
        p1_lon = temp1.lon;
        p1_lat = temp1.lat;
        p2_lon = temp2.lon;
        p2_lat = temp2.lat;

        FeatureType featType = getFeatureType(*i);


        if (featType == FeatureType::River) {
            setlinewidth(0.75);
            setcolor(0xB3, 0xD6, 0xFF);
        } else if (featType == FeatureType::Stream) {
            setlinewidth(0.75);
            setcolor(0xB3, 0xD6, 0xFF);
        }
        //checks open, and closed features
        if ((p1_lon != p2_lon) && (p1_lat != p2_lat)) {
            for (unsigned k = 0; k < numOfFeatPoints - 2; k++) {
                drawline(getFeaturePoint(*i, k).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k).lat*DEG_TO_RAD, getFeaturePoint(*i, k + 1).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k + 1).lat * DEG_TO_RAD);
            }
        } else {
            t_point points[numOfFeatPoints - 1];

            for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
                temp1 = getFeaturePoint(*i, k);
                p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average);
                p1_lat = temp1.lat *DEG_TO_RAD;
                points [k] = {p1_lon, p1_lat};
            }
            fillpoly(points, numOfFeatPoints - 1);
        }
    }

    for (auto i = DataStructure::pond_feature_id.begin(); i != DataStructure::pond_feature_id.end(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(*i);
        temp1 = getFeaturePoint(*i, 0);
        temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);
        p1_lon = temp1.lon;
        p1_lat = temp1.lat;
        p2_lon = temp2.lon;
        p2_lat = temp2.lat;

        FeatureType featType = getFeatureType(*i);


        if (featType == FeatureType::River) {
            setlinewidth(0.75);
            setcolor(0xB3, 0xD6, 0xFF);
        } else if (featType == FeatureType::Stream) {
            setlinewidth(0.75);
            setcolor(0xB3, 0xD6, 0xFF);
        }
        //checks open, and closed features
        if ((p1_lon != p2_lon) && (p1_lat != p2_lat)) {
            for (unsigned k = 0; k < numOfFeatPoints - 2; k++) {
                drawline(getFeaturePoint(*i, k).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k).lat*DEG_TO_RAD, getFeaturePoint(*i, k + 1).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k + 1).lat * DEG_TO_RAD);
            }
        } else {
            t_point points[numOfFeatPoints - 1];

            for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
                temp1 = getFeaturePoint(*i, k);
                p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average);
                p1_lat = temp1.lat *DEG_TO_RAD;
                points [k] = {p1_lon, p1_lat};
            }
            fillpoly(points, numOfFeatPoints - 1);
        }
    }



    //draws beaches
    for (auto i = DataStructure::beach_feature_id.begin(); i != DataStructure::beach_feature_id.end(); i++) {
        LatLon temp1, temp2;
        unsigned numOfFeatPoints = getFeaturePointCount(*i);
        temp1 = getFeaturePoint(*i, 0);
        temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);
        p1_lon = temp1.lon;
        p1_lat = temp1.lat;
        p2_lon = temp2.lon;
        p2_lat = temp2.lat;


        setcolor(0xF9, 0xF2, 0xC4);

        if ((p1_lon != p2_lon) && (p1_lat != p2_lat)) {
            for (unsigned k = 0; k < numOfFeatPoints - 2; k++) {
                drawline(getFeaturePoint(*i, k).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k).lat*DEG_TO_RAD, getFeaturePoint(*i, k + 1).lon * DEG_TO_RAD * cos(lat_average), getFeaturePoint(*i, k + 1).lat * DEG_TO_RAD);
            }
        } else {
            t_point points[numOfFeatPoints - 1];

            for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
                temp1 = getFeaturePoint(*i, k);
                p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average);
                p1_lat = temp1.lat *DEG_TO_RAD;
                points [k] = {p1_lon, p1_lat};
            }
            fillpoly(points, numOfFeatPoints - 1);
        }
    }



    //if the current screen is zoomed in enough, then the buildings will be displayed    
    if (temp_Area / startArea < 0.001) {
        for (auto i = DataStructure::zoom_level_1_features.begin(); i != DataStructure::zoom_level_1_features.end(); i++) {
            LatLon temp1, temp2;
            unsigned numOfFeatPoints = getFeaturePointCount(*i);
            temp1 = getFeaturePoint(*i, 0);
            temp2 = getFeaturePoint(*i, numOfFeatPoints - 1);


            p1_lon = temp1.lon;
            p1_lat = temp1.lat;
            p2_lon = temp2.lon;
            p2_lat = temp2.lat;
            setcolor(0xCC, 0xCC, 0xCC);

            t_point points[numOfFeatPoints - 1];

            for (unsigned k = 0; k < numOfFeatPoints - 1; k++) {
                temp1 = getFeaturePoint(*i, k);
                p1_lon = temp1.lon * DEG_TO_RAD * cos(lat_average);
                p1_lat = temp1.lat *DEG_TO_RAD;
                points [k] = {p1_lon, p1_lat};
            }
            fillpoly(points, numOfFeatPoints - 1);
        }
    }
}

// Prints out street names 

void print_out_street_names() {

    // Variables used for selective display
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();


    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {

        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);
        string temp_street_name = getStreetName(temp_info.streetID);
        unsigned temp_street_id = temp_info.streetID;

        int length_of_the_name = temp_street_name.length();
        unsigned longest_id = DataStructure::street_id_with_distance_to_the_closest_intersection[temp_street_id][3];
        //if (longest_id == i){
        LatLon temp1 = getIntersectionPosition(temp_info.from);
        LatLon temp2 = getIntersectionPosition(temp_info.to);

        //        double temp_distance = find_distance_between_two_points(temp1 , temp2);
        int temp_distance_in_pixels = find_pixels_between_two_points(temp1, temp2);

        int name_length_in_pixels;

        // Converting the latitudes and longitudes to x-y coordinates 
        double temp1_lat = temp1.lat * DEG_TO_RAD;
        double temp1_lon = temp1.lon * DEG_TO_RAD;

        double temp2_lat = temp2.lat * DEG_TO_RAD;
        double temp2_lon = temp2.lon * DEG_TO_RAD;

        double lat_average = (maxLat + minLat) / 2;

        double x_start = temp1_lon * cos(lat_average);
        double x_finish = temp2_lon * cos(lat_average);
        double y_start = temp1_lat;
        double y_finish = temp2_lat;

        double x_avg = ((x_finish + x_start) / 2);
        double y_avg = ((y_finish + y_start) / 2);

        unsigned num_curve_pts = temp_info.curvePointCount;


        // If it has curve points
        // Taking the average of all the points including the curve points
        if (num_curve_pts != 0) {
            x_avg = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][1];
            y_avg = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][2];
            x_start = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][3];
            y_start = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][4];
            x_finish = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][5];
            y_finish = DataStructure::street_seg_id_with_vector_of_length_of_the_biggest_street_seg[i][6];
            temp1 = DataStructure::lat_lon_of_longest_segment[i][0];
            temp2 = DataStructure::lat_lon_of_longest_segment[i][1];
            temp_distance_in_pixels = find_pixels_between_two_points(temp1, temp2);
        }

        double temp_hypotenuse = find_distance_between_two_points(temp1, temp2);

        StreetSegmentInfo info = getStreetSegmentInfo(i);
        OSMID way_ID = info.wayOSMID;
        unsigned street_ID = info.streetID;
        unsigned street_type = DataStructure::OSM_street_segs_num_of_lanes[way_ID];

        double temp_opposite = ((abs(y_finish - y_start)) * EARTH_RADIUS_IN_METERS);
        double temp_adjacent = ((abs(x_finish - x_start)) * EARTH_RADIUS_IN_METERS);
        int angle = 0;

        // if the road is slanted to the right      like this ----> /
        if (((y_start < y_finish) && (x_finish > x_start)) || ((y_finish < y_start) && (x_start > x_finish))) {

            angle = (((asin(temp_opposite / temp_hypotenuse)) / (DEG_TO_RAD))); // returns angle in degrees

            if ((y_start < y_finish) && (x_finish > x_start)) // if the one way is facing the point on the bottom of the road
                angle = (((asin(temp_opposite / temp_hypotenuse)) / (DEG_TO_RAD))); // returns angle in degrees

        } else if (y_start == y_finish) {
            angle = 0;
        } else if (x_start == x_finish) {
            angle = 270;
        }// if the road is slanted to the left      like this ----> \                                 *                      
        else if (((y_start > y_finish) && (x_finish > x_start)) || ((y_finish > y_start) && (x_start > x_finish))) {

            angle = (-((acos(temp_adjacent / temp_hypotenuse)) / (DEG_TO_RAD)));

            if ((y_start > y_finish) && (x_finish > x_start)) // if the one way is facing the point on the bottom of the road
                angle = (-((acos(temp_adjacent / temp_hypotenuse)) / (DEG_TO_RAD)));
        }


        // selective display
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) && ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            // List of unnecessary road names world-wide
            string no_print1 = "Ramp";
            string no_print2 = "Exchange";
            string no_print3 = "Collector";
            string no_print4 = "Express";
            string no_print5 = "to";
            string no_print6 = "To";
            size_t dont_print1 = temp_street_name.find(no_print1);
            size_t dont_print2 = temp_street_name.find(no_print2);
            size_t dont_print3 = temp_street_name.find(no_print3);
            size_t dont_print4 = temp_street_name.find(no_print4);
            size_t dont_print5 = temp_street_name.find(no_print5);
            size_t dont_print6 = temp_street_name.find(no_print6);

            // If all those list of words aren't in the street name
            // Enter the If case
            if (dont_print1 == string::npos && dont_print2 == string::npos && dont_print3 == string::npos && dont_print4 == string::npos
                    && dont_print5 == string::npos && dont_print6 == string::npos) {



                setcolor(BLACK);
                //unsigned printed_z3 = DataStructure::zoom_level_3_street_printed[street_ID]; // Structure to print out one name per street
                if ((temp_Area / startArea < 0.0001) && (temp_street_name != "<unknown>")) {
                    //                    if (printed_z3 == 0) {  // For printing once per street

                    if (street_type == 3) {
                        name_length_in_pixels = length_of_the_name * 21;

                        if (name_length_in_pixels < temp_distance_in_pixels) {


                            settextrotation(angle);
                            setfontsize(15);
                            drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);
                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 2) {
                        name_length_in_pixels = length_of_the_name * 17;

                        if (name_length_in_pixels < temp_distance_in_pixels) {

                            settextrotation(angle);
                            setfontsize(13);
                            drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);
                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 1) {
                        name_length_in_pixels = length_of_the_name * 15;

                        if (name_length_in_pixels < temp_distance_in_pixels) {

                            settextrotation(angle);
                            setfontsize(11);
                            drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);
                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    }
                    //                    }
                } else if ((temp_Area / startArea < 0.0008) && (temp_street_name != "<unknown>")) {
                    //                    if (printed_z3 == 0) {
                    if (street_type == 3) {
                        name_length_in_pixels = length_of_the_name * 15;

                        if (name_length_in_pixels < temp_distance_in_pixels) {


                            settextrotation(angle);
                            setfontsize(11);
                            drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);
                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 2) {
                        name_length_in_pixels = length_of_the_name * 12;

                        if (name_length_in_pixels < temp_distance_in_pixels) {

                            settextrotation(angle);
                            setfontsize(9);

                            if (i % 2 == 0) // to avoid clutter
                                drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 1) {
                        name_length_in_pixels = length_of_the_name * 11;

                        if (name_length_in_pixels < temp_distance_in_pixels) {

                            settextrotation(angle);
                            setfontsize(8);

                            if (i % 3 == 0) // to avoid clutter 
                                drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    }
                    //                    }
                } else if ((temp_Area / startArea < 0.001) && (temp_street_name != "<unknown>")) {
                    //                    if (printed_z3 == 0) {
                    if (street_type == 3) {
                        name_length_in_pixels = length_of_the_name * 13;

                        if (name_length_in_pixels < temp_distance_in_pixels) {
                            settextrotation(angle);
                            setfontsize(10);

                            drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);
                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 2) {

                        name_length_in_pixels = length_of_the_name * 11;

                        if (name_length_in_pixels < temp_distance_in_pixels) {
                            settextrotation(angle);
                            setfontsize(8);

                            if ((i % 2) == 0) // to avoid clutter
                                drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 1) {
                        name_length_in_pixels = length_of_the_name * 8;

                        if (name_length_in_pixels < temp_distance_in_pixels) {

                            settextrotation(angle);
                            setfontsize(6);

                            if ((i % 3) == 0) // to avoid clutter
                                drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    }
                    //                    }
                } else if ((temp_Area / startArea < 0.01) && (temp_street_name != "<unknown>")) {
                    //                    if (printed_z3 == 0) {
                    if (street_type == 3) {
                        name_length_in_pixels = length_of_the_name * 12;

                        if (name_length_in_pixels < temp_distance_in_pixels) {

                            settextrotation(angle);
                            setfontsize(9);

                            if (i % 2 == 0) // to avoid clutter
                                drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    } else if (street_type == 2) {

                        name_length_in_pixels = length_of_the_name * 9;

                        if (name_length_in_pixels < temp_distance_in_pixels) {
                            settextrotation(angle);
                            setfontsize(7);

                            if ((i % 3) == 0) // to avoid clutter
                                drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    }
                    //                    }
                } else if ((temp_Area / startArea < 0.1) && (temp_street_name != "<unknown>")) {
                    //                    if (printed_z3 == 0) {
                    if (street_type == 3) {
                        name_length_in_pixels = length_of_the_name * 9;

                        if (name_length_in_pixels < temp_distance_in_pixels) {
                            settextrotation(angle);
                            setfontsize(7);

                            //                            if (i % 2 == 0) // to avoid clutter
                            drawtext(x_avg, y_avg, temp_street_name, FLT_MAX, FLT_MAX);

                            DataStructure::zoom_level_3_street_printed.erase(street_ID);
                            DataStructure::zoom_level_3_street_printed.insert({street_ID, 1});
                        }
                    }
                    //                    }
                }
                // text rotation back to 0
                settextrotation(0);
            }
        }
    }
    // setting them all back to 0 after 
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {
        DataStructure::zoom_level_3_street_printed.erase(i);
        DataStructure::zoom_level_3_street_printed.insert({i, 0});
    }
    // }
}

// Prints out POI names for the user-chosen POIs in the "Display POI" button

void print_out_POI_names() {

    // Variables for selective display
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();


    for (auto i = DataStructure::chosen_POI_names.begin(); i != DataStructure::chosen_POI_names.end(); i++) {
        LatLon temp1 = getPointOfInterestPosition(*i);
        string temp_POI_name = getPointOfInterestName(*i);

        // converting latitudes and longitudes to x-y coordinates 
        float temp1_lat = temp1.lat * DEG_TO_RAD;
        float temp1_lon = temp1.lon * DEG_TO_RAD;


        float lat_average = (maxLat + minLat) / 2;

        float x_start = temp1_lon * cos(lat_average);
        float y_start = temp1_lat;

        // selective display
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            setcolor(0xB3, 0x00, 0x00);

            setfontsize(8);

            // Setting the boundary of the text (POI names)
            t_bound_box textsquare = t_bound_box(t_point((x_start - 0.000005), (y_start - 0.000005)), t_point((x_start + 0.000005), (y_start + 0.000005)));

            // Print the names out onto the set boundary
            drawtext_in(textsquare, temp_POI_name);

            setcolor(0xFF, 0x00, 0x66);

            // Drawing little indicators of the POIs
            fillarc(x_start, y_start, 0.0000002, 0, 360);
        }
    }
}

void act_on_help_button_func(void (*drawscreen_ptr) (void)) {
    display_help = true;
    search_bar_show = false;
    search_bar_show_POI = false;
    set_visible_world(x_start - 100, y_start, x_finish - 100, y_finish);
    x1Help = get_visible_world().left();
    y1Help = get_visible_world().bottom();
    x2Help = get_visible_world().right();
    y2Help = get_visible_world().top();
    x_centre = (x2Help - x1Help) / 2;
    y_title = (y2Help - y1Help) / 2;
    draw_screen();
}

void draw_help_screen() {
    //set colors and draw necessary graphics for help screen
    setcolor(WHITE);
    fillrect(x1Help, y1Help, x2Help, y2Help);
    setcolor(BLACK);
    fillrect(x1Help, y1Help + ((y2Help - y1Help) / 2)+((y2Help - y1Help) / 2)*0.94, x2Help, y2Help);
    setcolor(BLUE);
    fillrect(x1Help, y1Help + ((y2Help - y1Help) / 2)+((y2Help - y1Help) / 2)*0.95, x2Help, y2Help);
    setcolor(DARKGREY);
    fillrect(x1Help + x_centre * 0.7, y1Help + y_title * 0.1, x2Help - x_centre * 0.7, y1Help + y_title * 0.2);
    setcolor(WHITE);
    setfontsize(11);
    drawtext(x1Help + x_centre * 0.2, y1Help + y_title + y_title * 0.975, help_title, FLT_MAX, FLT_MAX);
    setcolor(BLACK);
    setfontsize(30);
    drawtext(x1Help + x_centre, y1Help + y_title + y_title * 0.87, title, FLT_MAX, FLT_MAX);
    setfontsize(20);
    //draws texts for help screen pre initialized globally
    drawtext(x1Help + x_centre * 0.8, y1Help + y_title + y_title * 0.78, first_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.735, y1Help + y_title + y_title * 0.69, second_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.56, y1Help + y_title + y_title * 0.60, third_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.847, y1Help + y_title + y_title * 0.51, fourth_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.97, y1Help + y_title + y_title * 0.42, fifth_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.225, y1Help + y_title + y_title * 0.33, seventh_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.58, y1Help + y_title + y_title * 0.24, transit_instruction1, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.815, y1Help + y_title + y_title * 0.15, transit_instruction2, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.851, y1Help + y_title + y_title * 0.06, transit_instruction3, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.389, y1Help + y_title * 0.97, eighth_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.561, y1Help + y_title * 0.88, POI_instruction1, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.872, y1Help + y_title * 0.79, POI_instruction2, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.638, y1Help + y_title * 0.70, POI_instruction3, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 1.01, y1Help + y_title * 0.61, sixth_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.905, y1Help + y_title * 0.52, ninth_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre * 0.98, y1Help + y_title * 0.43, tenth_instruction, FLT_MAX, FLT_MAX);
    drawtext(x1Help + x_centre, y1Help + y_title * 0.15, back_instruction, FLT_MAX, FLT_MAX);
}

// Prints out the message after user uses the "Find" button

void act_on_find_button_func(void (*drawscreen_ptr) (void)) {

    // Message showed at the bottom of the window
    update_message("Please enter two streets in the terminal.");

    string street_one, street_two;
    vector <unsigned> intersection_ids_of_streets;

    cout << "Enter street number one: " << endl; // For user input
    getline(cin, street_one);

    cout << "Enter street number two: " << endl; // For user input
    getline(cin, street_two);

    // We use the M1 function to find vector of intersections between the 2 streets
    intersection_ids_of_streets = find_intersection_ids_from_street_names(street_one, street_two);

    // Message displayed for the user
    cout << "The intersection(s) of the roads " << street_one << " and " << street_two << "are: " << endl;
    for (auto i = intersection_ids_of_streets.begin(); i != intersection_ids_of_streets.end(); i++) {

        LatLon temp = getIntersectionPosition(*i);

        cout << "ID: " << *i << " Lat: " << temp.lat << " Lon: " << temp.lon << endl;
        DataStructure::draw_found_intersections.push_back(*i);
    }
}

// Draws indicators at the intersection id the user found

void draw_found_highlighted() {
    LatLon temp1;
    for (auto i = DataStructure::draw_found_intersections.begin(); i != DataStructure::draw_found_intersections.end(); i++) {
        temp1 = getIntersectionPosition(*i);
        float temp1_lat = temp1.lat * DEG_TO_RAD;
        float temp1_lon = temp1.lon * DEG_TO_RAD;

        float lat_average = (maxLat + minLat) / 2;

        float x_start = temp1_lon * cos(lat_average);
        float y_start = temp1_lat;
        float temp_Area = get_visible_world().area();

        if ((temp_Area / startArea) < 0.01) {
            setcolor(BLUE);
            fillarc(x_start, y_start + 0.0000039, 0.000002, 0, 180);
            t_point points[3];

            points [0] = {x_start, y_start};
            points [1] = {x_start + 0.000002, y_start + 0.000004};
            points [2] = {x_start - 0.000002, y_start + 0.000004};

            fillpoly(points, 3);
            setcolor(BLACK);
            fillarc(x_start, y_start + 0.0000039, 0.0000009, 0, 360);
        } else if (temp_Area / startArea < 0.1) {
            setcolor(BLUE);
            fillarc(x_start, y_start + 0.000016, 0.000008, 0, 180);
            t_point points[3];

            points [0] = {x_start, y_start};
            points [1] = {x_start + 0.000008, y_start + 0.000016};
            points [2] = {x_start - 0.000008, y_start + 0.000016};

            fillpoly(points, 3);
            setcolor(BLACK);
            fillarc(x_start, y_start + 0.000008, 0.0000018, 0, 360);
        } else if (temp_Area / startArea < 2) {
            setcolor(BLUE);
            fillarc(x_start, y_start + 0.000040, 0.000024, 0, 180);
            t_point points[3];

            points [0] = {x_start, y_start};
            points [1] = {x_start + 0.000024, y_start + 0.000040};
            points [2] = {x_start - 0.000024, y_start + 0.000040};

            fillpoly(points, 3);
            setcolor(BLACK);
            fillarc(x_start, y_start + 0.000040, 0.0000080, 0, 360);
        }
    }
}

// Clears everything on the map

void act_on_clear_button_func(void (*drawscreen_ptr) (void)) {

    DataStructure::draw_found_intersections.clear();
    DataStructure::intersection_ids.clear();
    DataStructure::chosen_POIs.clear();
    DataStructure::chosen_POI_names.clear();
    DataStructure::bus_ways.clear();
    DataStructure::selected_bus_route.clear();
    DataStructure::test.clear();
    DataStructure::shortest_path.clear();
    find_intersection_enable = false;
    find_POI_enable = false;
    search_bar_clicked = false;
    search_bar_clicked_POI = false;
    searched_text.clear();
    searched_text_POI.clear();
    display_directions_enable = false;
    subway_button = false;
    update_message("Cleared."); // at the bottom of the window 
    cout << "Cleared." << endl; // in the terminal 
    draw_screen();
}

// Prints out messages for the user in the terminal once "Display POI" button is pressed

void act_on_display_POI_button_func(void (*drawscreen_ptr) (void)) {
    string choice;

    update_message("List of POIs shown in terminal.");

    cout << "Enter the type of POI you want displayed on you map" << endl;

    cout << "1. Coffee\n" << "2. Fast Food\n" << "3. Restaurant\n" << "4. Pub\n" << "5. Bar\n" << "6. Bank\n"
            << "7. Hospital\n" << "8. University\n" << "9. Pharmacy\n" << "10. Parking\n" << "11. Cinema\n"
            << "12. Gym\n" << "13. Other" << endl;

    getline(cin, choice);
    cout << "\n" << endl;


    // Looking at the user choices
    // Pushes into vectors to draw indicators AND print out its names
    if (choice == "Coffee" || choice == "1") {
        vector <LatLon> coffee_place = DataStructure::amenity_cafe;
        vector <OSMID> coffee_vec = DataStructure::vector_cafe;
        for (auto iter = coffee_place.begin(); iter != coffee_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = coffee_vec.begin(); i != coffee_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Fast Food" || choice == "2") {
        vector <LatLon> fast_food_place = DataStructure::amenity_fast_food;
        vector <OSMID> fast_food_vec = DataStructure::vector_fast_food;
        for (auto iter = fast_food_place.begin(); iter != fast_food_place.end(); iter++) {
            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = fast_food_vec.begin(); i != fast_food_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Restaurant" || choice == "3") {
        vector <LatLon> restaurant_place = DataStructure::amenity_restaurant;
        vector <OSMID> restaurant_vec = DataStructure::vector_restaurant;

        for (auto iter = restaurant_place.begin(); iter != restaurant_place.end(); iter++) {
            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = restaurant_vec.begin(); i != restaurant_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Pub" || choice == "4") {
        vector <LatLon> pub_place = DataStructure::amenity_pub;
        vector <OSMID> pub_vec = DataStructure::vector_pub;

        for (auto iter = pub_place.begin(); iter != pub_place.end(); iter++) {
            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = pub_vec.begin(); i != pub_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Bar" || choice == "5") {
        vector <LatLon> bar_place = DataStructure::amenity_bar;
        vector <OSMID> bar_vec = DataStructure::vector_bar;

        for (auto iter = bar_place.begin(); iter != bar_place.end(); iter++) {
            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = bar_vec.begin(); i != bar_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Bank" || choice == "6") {
        vector <LatLon> bank_place = DataStructure::amenity_bank;
        vector <OSMID> bank_vec = DataStructure::vector_bank;

        for (auto iter = bank_place.begin(); iter != bank_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = bank_vec.begin(); i != bank_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Hospital" || choice == "7") {

        vector <LatLon> hospital_place = DataStructure::amenity_hospital;
        vector <OSMID> hospital_vec = DataStructure::vector_hospital;

        for (auto iter = hospital_place.begin(); iter != hospital_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = hospital_vec.begin(); i != hospital_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "University" || choice == "8") {
        vector <LatLon> university_place = DataStructure::amenity_university;
        vector <OSMID> university_vec = DataStructure::vector_university;

        for (auto iter = university_place.begin(); iter != university_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = university_vec.begin(); i != university_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Pharmacy" || choice == "9") {
        vector <LatLon> pharmacy_place = DataStructure::amenity_pharmacy;
        vector <OSMID> pharmacy_vec = DataStructure::vector_pharmacy;
        for (auto iter = pharmacy_place.begin(); iter != pharmacy_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = pharmacy_vec.begin(); i != pharmacy_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Parking" || choice == "10") {
        vector <LatLon> parking_place = DataStructure::amenity_parking;
        vector <OSMID> parking_vec = DataStructure::vector_parking;
        for (auto iter = parking_place.begin(); iter != parking_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = parking_vec.begin(); i != parking_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Cinema" || choice == "11") {
        vector <LatLon> cinema_place = DataStructure::amenity_cinema;
        vector <OSMID> cinema_vec = DataStructure::vector_cinema;
        for (auto iter = cinema_place.begin(); iter != cinema_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = cinema_vec.begin(); i != cinema_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Gym" || choice == "12") {
        vector <LatLon> gym_place = DataStructure::amenity_gym;
        vector <OSMID> gym_vec = DataStructure::vector_gym;
        for (auto iter = gym_place.begin(); iter != gym_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = gym_vec.begin(); i != gym_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else if (choice == "Other" || choice == "13") {
        vector <LatLon> else_place = DataStructure::amenity_else;
        vector <OSMID> else_vec = DataStructure::vector_else;
        for (auto iter = else_place.begin(); iter != else_place.end(); iter++) {

            DataStructure::chosen_POIs.push_back(*iter);
        }

        for (auto i = else_vec.begin(); i != else_vec.end(); i++) {
            unsigned POI_id = DataStructure::POI_OSMIDS[*i];
            DataStructure::chosen_POI_names.push_back(POI_id);
        }
    } else {
        cout << "Invalid Choice" << endl;
    }

    draw_screen();

}

// Draws the user-chosen POIs

void draw_POI_highlight() {

    LatLon temp1;
    for (auto iter = DataStructure::chosen_POIs.begin(); iter != DataStructure::chosen_POIs.end(); iter++) {
        temp1 = *iter;
        float temp1_lat = temp1.lat * DEG_TO_RAD;
        float temp1_lon = temp1.lon * DEG_TO_RAD;

        float lat_average = ((maxLat + minLat) / 2);

        float x_start = temp1_lon * cos(lat_average);
        float y_start = temp1_lat;

        setcolor(GREEN);
        fillarc(x_start, y_start + 0.0000039, 0.000002, 0, 180);
        t_point points[3];

        points [0] = {x_start, y_start};
        points [1] = {x_start + 0.000002, y_start + 0.000004};
        points [2] = {x_start - 0.000002, y_start + 0.000004};

        fillpoly(points, 3);
        setcolor(BLACK);
        fillarc(x_start, y_start + 0.0000039, 0.0000009, 0, 360);

    }
}


// Once "Public Transit" button is pressed

void act_on_display_public_transit_func(void (*drawscreen_ptr) (void)) {
    string transit_choice;
    update_message("List of public transit shown in terminal");


    //ask user which public transit should be displayed
    cout << "Enter the public transit you want displayed on the map" << endl;

    cout << "1. Subway\n" << "2. Bus\n" << endl;


    getline(cin, transit_choice);


    //if subway wants to be displayed, enable the variable so it draws in "draw_screen" function
    if (transit_choice == "Subway") {
        subway_button = true;
        update_message("Subway Map");
        //if bus wants to be displayed, enable the variable so it draws in "draw_screen" function
    } else if (transit_choice == "Bus") {
        bus_button = true;
        bool found = false;
        string choice;

        //ask user which bus route should be displayed
        cout << "\nEnter the bus route number you want displayed on the map ('All' for entire bus route)" << endl;

        getline(cin, choice);

        //all bus routes are are pushed in the vector to be drawn
        if (choice == "All") {
            for (unsigned i = 0; i < DataStructure::bus_routes.size(); i++) {
                const OSMRelation* tempRelation = getRelationByIndex(DataStructure::bus_routes[i]);
                for (unsigned k = 0; k < tempRelation->members().size(); k++) {
                    if (tempRelation->members()[k].type == Way)
                        DataStructure::bus_ways.push_back(tempRelation->members()[k].id);
                }
            }
            update_message("All bus routes");
        }//selected bus route is pushed in the vector to be drawn
        else {

            for (unsigned i = 0; i < DataStructure::bus_routes.size(); i++) {
                const OSMRelation* tempRelation = getRelationByIndex(DataStructure::bus_routes[i]);
                unsigned relationCount = getTagCount(tempRelation);
                for (unsigned j = 0; j < relationCount; j++) {
                    pair<string, string> temp = getTagPair(tempRelation, j);
                    if (temp.first == "ref" && temp.second == choice) {
                        DataStructure::selected_bus_route.push_back(DataStructure::bus_routes[i]);
                        found = true;
                    }
                }
                if (found == true)
                    break;
            }
            for (unsigned i = 0; i < DataStructure::selected_bus_route.size(); i++) {
                const OSMRelation* tempRelation = getRelationByIndex(DataStructure::selected_bus_route[i]);
                for (unsigned k = 0; k < tempRelation->members().size(); k++) {
                    if (tempRelation->members()[k].type == Way)
                        DataStructure::bus_ways.push_back(tempRelation->members()[k].id);
                }
            }
            update_message("SelectedBus Route");
        }
    }
    draw_screen();
}


// Draws subway routes

void draw_subway_tracks() {
    vector <unsigned> tempVec;
    //go through all the subway relations
    for (unsigned j = 0; j < DataStructure::subway_tracks.size(); j++) {
        const OSMRelation* tempRelation = getRelationByIndex(DataStructure::subway_tracks[j]);
        //temporary vector to store the number of ways in each relation to be used to distinguish colors of each subway route
        for (unsigned k = 0; k < tempRelation->members().size(); k++) {
            if (tempRelation->members()[k].type == Way)
                tempVec.push_back(tempRelation->members()[k].id);
        }
        //draw all the routes
        for (unsigned i = 0; i < tempVec.size(); i++) {
            const OSMWay* tempWay = getWayByIndex(DataStructure::OSM_And_Way_ID[tempVec[i]]);
            for (unsigned k = 0; k < tempWay->ndrefs().size() - 1; k++) {
                LatLon temp1 = getNodeByIndex(DataStructure::OSM_And_Node_ID[tempWay->ndrefs()[k]])->coords();
                LatLon temp2 = getNodeByIndex(DataStructure::OSM_And_Node_ID[tempWay->ndrefs()[k + 1]])->coords();
                float lat_average = (maxLat + minLat) / 2;

                float temp1_lat = temp1.lat * DEG_TO_RAD;
                float temp1_lon = temp1.lon * DEG_TO_RAD;
                float temp2_lat = temp2.lat * DEG_TO_RAD;
                float temp2_lon = temp2.lon * DEG_TO_RAD;

                float x_start = temp1_lon * cos(lat_average);
                float y_start = temp1_lat;

                float x_end = temp2_lon * cos(lat_average);
                float y_end = temp2_lat;
                setlinewidth(5);
                setcolor (GREEN + j + 7);
                drawline(x_start, y_start, x_end, y_end);
            }
        }
        tempVec.clear();
    }
}


// Draws bus routes

void draw_bus_routes() {
    //go through the vector that has desired bus routes stored in and draw the bus routes
    for (unsigned i = 0; i < DataStructure::bus_ways.size(); i++) {
        const OSMWay* tempWay = getWayByIndex(DataStructure::OSM_And_Way_ID[DataStructure::bus_ways[i]]);
        for (unsigned k = 0; k < tempWay->ndrefs().size() - 1; k++) {
            LatLon temp1 = getNodeByIndex(DataStructure::OSM_And_Node_ID[tempWay->ndrefs()[k]])->coords();
            LatLon temp2 = getNodeByIndex(DataStructure::OSM_And_Node_ID[tempWay->ndrefs()[k + 1]])->coords();
            float lat_average = (maxLat + minLat) / 2;

            float temp1_lat = temp1.lat * DEG_TO_RAD;
            float temp1_lon = temp1.lon * DEG_TO_RAD;
            float temp2_lat = temp2.lat * DEG_TO_RAD;
            float temp2_lon = temp2.lon * DEG_TO_RAD;

            float x_start = temp1_lon * cos(lat_average);
            float y_start = temp1_lat;

            float x_end = temp2_lon * cos(lat_average);
            float y_end = temp2_lat;
            setcolor(RED);
            setlinewidth(3);
            drawline(x_start, y_start, x_end, y_end);
        }
    }
}


// Draws subway stations

void draw_train_stns() {
    //go through the subway ways
    for (unsigned i = 0; i < DataStructure::train_nodes.size(); i++) {
        const OSMWay* tempWay = getWayByIndex(DataStructure::OSM_And_Way_ID[DataStructure::train_nodes[i]]);
        //go through the nodes of subway ways
        for (unsigned k = 0; k < tempWay->ndrefs().size(); k++) {
            //go through the vector that has all the train stations 
            for (unsigned j = 0; j < DataStructure::train_stns.size(); j++) {
                //compare nodes of subway ways with all train stations and if they match, draw as subway stations
                if (DataStructure::OSM_And_Node_ID[tempWay->ndrefs()[k]] == DataStructure::train_stns[j]) {
                    LatLon temp1 = getNodeByIndex(DataStructure::OSM_And_Node_ID[tempWay->ndrefs()[k]])->coords();
                    float lat_average = (maxLat + minLat) / 2;


                    //converting latitudes and longitudes to x-y coordinates 
                    float temp1_lat = temp1.lat * DEG_TO_RAD;
                    float temp1_lon = temp1.lon * DEG_TO_RAD;

                    float x_start = temp1_lon * cos(lat_average);
                    float y_start = temp1_lat;


                    setcolor(RED);
                    fillarc(x_start, y_start, 0.000008, 0, 360);
                }
            }
        }
    }
}

// Draws arrows on one-way roads 

void oneway() {

    // vector of StreetSegmentInfos that are one-way
    vector <StreetSegmentInfo> temp_one_way = DataStructure::one_way_street_segment_info;

    // variables needed for selective display
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();

    for (auto iter = temp_one_way.begin(); iter != temp_one_way.end(); iter++) {

        StreetSegmentInfo temp_info = *iter;

        unsigned num_curve_pts = temp_info.curvePointCount;

        LatLon temp1 = getIntersectionPosition(temp_info.from);
        LatLon temp2 = getIntersectionPosition(temp_info.to);


        //converting latitudes and longitudes to x-y coordinates 
        float temp1_lat = temp1.lat * DEG_TO_RAD;
        float temp1_lon = temp1.lon * DEG_TO_RAD;

        float temp2_lat = temp2.lat * DEG_TO_RAD;
        float temp2_lon = temp2.lon * DEG_TO_RAD;

        float lat_average = (maxLat + minLat) / 2;

        float x_start = temp1_lon * cos(lat_average);
        float x_finish = temp2_lon * cos(lat_average);
        float y_start = temp1_lat;
        float y_finish = temp2_lat;

        double x_avg = ((x_finish + x_start) / 2.0000000000);
        double y_avg = ((y_finish + y_start) / 2.0000000000);

        // Use of Pythagorean Theorem
        double temp_hypotenuse = find_distance_between_two_points(temp1, temp2);
        double temp_opposite = (abs(y_finish - y_start)) * EARTH_RADIUS_IN_METERS;
        double temp_adjacent = (abs(x_finish - x_start)) * EARTH_RADIUS_IN_METERS;

        int angle = 0;


        // if the road is slanted to the right      like this ----> /
        if (((y_start < y_finish) && (x_finish > x_start)) || ((y_finish < y_start) && (x_start > x_finish))) {

            angle = (((asin(temp_opposite / temp_hypotenuse)) / (DEG_TO_RAD))); // returns angle in degrees

            if ((y_start < y_finish) && (x_finish > x_start)) // if the one way is facing the point on the bottom of the road
                angle = (((asin(temp_opposite / temp_hypotenuse)) / (DEG_TO_RAD)) + 540); // returns angle in degrees

        } else if (y_start == y_finish) {
            if (x_finish > x_start)
                angle = 180;
            else
                angle = 0;
        }// if the road is slanted to the left      like this ----> \                                 *                      
        else if (((y_start > y_finish) && (x_finish > x_start)) || ((y_finish > y_start) && (x_start > x_finish))) {

            angle = (360 - ((acos(temp_adjacent / temp_hypotenuse)) / (DEG_TO_RAD)));

            if ((y_start > y_finish) && (x_finish > x_start)) // if the one way is facing the point on the bottom of the road
                angle = (540 - ((acos(temp_adjacent / temp_hypotenuse)) / (DEG_TO_RAD)));
        }


        // selective display
        // only re-draws the current window boundary instead of the whole map
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            // draw the arrow     
            if ((temp_Area / startArea < 0.0008) && (num_curve_pts < 3) && (temp_info.from % 2 == 0)) {
                setcolor(0x00, 0x00, 0x66);
                setfontsize(9);
                settextrotation(angle);
                drawtext(x_avg, y_avg, "<--", FLT_MAX, FLT_MAX);
            }
        }
    }
    // Rotating the text back to 0
    settextrotation(0);
}

int find_pixels_between_two_points(LatLon point1, LatLon point2) {
    float point1_lat, point1_lon;
    float point2_lat, point2_lon;
    float x1, x2, y1, y2;
    float distance;
    float lat_average;
    int x1_pixel, x2_pixel, y1_pixel, y2_pixel;

    //converts both points lat and lons to RAD from DEG
    point1_lat = point1.lat * DEG_TO_RAD;
    point1_lon = point1.lon * DEG_TO_RAD;

    point2_lat = point2.lat * DEG_TO_RAD;
    point2_lon = point2.lon * DEG_TO_RAD;

    //computers the lat average of both the points
    lat_average = (maxLat + minLat) / 2;

    //converts lat and lon of both points to x and y
    x1 = point1_lon * cos(lat_average);
    x2 = point2_lon * cos(lat_average);
    y1 = point1_lat;
    y2 = point2_lat;

    x1_pixel = xworld_to_pixel(x1);
    x2_pixel = xworld_to_pixel(x2);
    y1_pixel = yworld_to_pixel(y1);
    y2_pixel = yworld_to_pixel(y2);


    //squares the distance difference of both x_pixel and y_pixel
    double term1 = (y2_pixel - y1_pixel)*(y2_pixel - y1_pixel);
    double term2 = (x2_pixel - x1_pixel)*(x2_pixel - x1_pixel);

    int return_this = sqrt(term1 + term2);

    return return_this;
}

void draw_shortests() {

    StreetSegmentInfo info;
    LatLon temp_start, temp_finish;
    unsigned start, finish;

    //Gets the limits of the current screen, for selective display.
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();


    for (auto i = DataStructure::shortest_path.begin(); i != DataStructure::shortest_path.end(); i++) {

        info = getStreetSegmentInfo(*i);
        start = info.from;
        finish = info.to;
        temp_start = getIntersectionPosition(start);
        temp_finish = getIntersectionPosition(finish);

        unsigned num_of_curve_points = info.curvePointCount;

        // Selective display, checks if segment is inside current screen.
        if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
                ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

            if (num_of_curve_points != 0) { //if there is curve points 

                draw_shortest_edges_with_curve_point(temp_start, getStreetSegmentCurvePoint(*i, 0), info);

                for (unsigned k = 0; k < num_of_curve_points - 1; k++) {
                    draw_shortest_edges_with_curve_point(getStreetSegmentCurvePoint(*i, k), getStreetSegmentCurvePoint(*i, k + 1), info);
                }

                draw_shortest_edges_with_curve_point(getStreetSegmentCurvePoint(*i, num_of_curve_points - 1), temp_finish, info);
            } else
                draw_shortest_edges_with_curve_point(temp_start, temp_finish, info);
        }
    }
}

void draw_shortest_edges_with_curve_point(LatLon position_1, LatLon position_2, StreetSegmentInfo info) {

    float radius;
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();

    float point1_lat, point1_lon;
    float point2_lat, point2_lon;
    float lat_average;
    float x_start, x_finish;
    float y_start, y_finish;

    OSMID way_ID = info.wayOSMID;
    unsigned street_type;
    point1_lat = position_1.lat * DEG_TO_RAD;
    point1_lon = position_1.lon * DEG_TO_RAD;

    point2_lat = position_2.lat * DEG_TO_RAD;
    point2_lon = position_2.lon * DEG_TO_RAD;

    lat_average = (maxLat + minLat) / 2;

    x_start = point1_lon * cos(lat_average);
    x_finish = point2_lon * cos(lat_average);
    y_start = point1_lat;
    y_finish = point2_lat;

    //Gets current area of the screen, to compare it with starting area. 
    temp_Area = get_visible_world().area();
    street_type = DataStructure::OSM_street_segs_num_of_lanes[way_ID]; //gets street_type from database
    setcolor(BLUE);
    setlinewidth(22);

    // Selective display, checks if segment is inside current screen.
    if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
            ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

        //Given different ratios between temp_area(current screen) and startArea (initial screen). The linewidths and what is drawn is different.
        //Line widths get larger, as the ratio between the temp and start get smaller.
        //Largest number, displays no minor roads.


        if (temp_Area / startArea < 0.0001) {//smallest zoom level, screen cannot zoom past this (Modifed Graphics.cpp, handle zoom function)
            radius = 400e-9;
            if (street_type == 4) { //Highway ramps

                setlinewidth(26);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);


            } else if (street_type == 3) { //Highway

                setlinewidth(26);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 580e-9, 0, 360);
                fillarc(x_finish, y_finish, 580e-9, 0, 360);

            } else if (street_type == 2) {// Major roads


                setlinestyle(SOLID);
                setlinewidth(26);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 540e-9, 0, 360);
                fillarc(x_finish, y_finish, 540e-9, 0, 360);

            } else if (street_type == 1) { //Minor roads

                setlinestyle(SOLID);
                setlinewidth(26);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 510e-9, 0, 360);
                fillarc(x_finish, y_finish, 510e-9, 0, 360);

            }
        } else if (temp_Area / startArea < 0.0008) {
            radius = 410e-9;
            if (street_type == 4) {

                setlinewidth(25);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);
            } else if (street_type == 3) {

                setlinewidth(15);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);
            } else if (street_type == 2) {

                setlinestyle(SOLID);
                setlinewidth(15);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 440e-9, 0, 360);
                fillarc(x_finish, y_finish, 440e-9, 0, 360);


            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(15);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, radius, 0, 360);
                fillarc(x_finish, y_finish, radius, 0, 360);
            }
        } else if (temp_Area / startArea < 0.001) {
            radius = 380e-9;
            if (street_type == 4) {

                setlinewidth(10);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 410e-9, 0, 360);
                fillarc(x_finish, y_finish, 410e-9, 0, 360);
            } else if (street_type == 3) {

                setlinewidth(10);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 410e-9, 0, 360);
                fillarc(x_finish, y_finish, 410e-9, 0, 360);
            } else if (street_type == 2) {


                setlinestyle(SOLID);
                setlinewidth(10);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, radius, 0, 360);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(10);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 400e-9, 0, 360);
                fillarc(x_finish, y_finish, 400e-9, 0, 360);
            }
        } else if (temp_Area / startArea < 0.01) {
            radius = 250e-9;
            if (street_type == 4) {

                setlinewidth(6);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 3) {

                setlinewidth(6);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 2) {

                setlinestyle(SOLID);
                setlinewidth(6);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(6);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            }
        } else if (temp_Area / startArea < 0.1) {
            //cout << "0.5" << endl;
            if (street_type == 4) {

                setlinewidth(3.5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 3) {

                setlinewidth(3.5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 2) {


                setlinestyle(SOLID);
                setlinewidth(3.5);
                drawline(x_start, y_start, x_finish, y_finish);

            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(3.5);
                drawline(x_start, y_start, x_finish, y_finish);
            }

        } else if (temp_Area / startArea < 10) { //Largest zoom level, past initial screen size

            if (street_type == 4) {

                setlinewidth(5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 3) {

                setlinewidth(5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 2) {

                setlinestyle(SOLID);
                setlinewidth(5);
                drawline(x_start, y_start, x_finish, y_finish);

            }
        }
    }
}

#include <X11/keysym.h>

void act_on_key_press(char c, int keysym) {
    // function to handle keyboard press event, the ASCII character is returned
    // along with an extended code (keysym) on X11 to represent non-ASCII
    // characters like the arrow keys.

    std::cout << "Key press: char is " << c << std::endl;

#ifdef X11// Extended keyboard codes only supported for X11 for now
    switch (keysym) {
        case XK_Left:
            std::cout << "Left Arrow" << std::endl;
            break;
        case XK_Right:
            std::cout << "Right Arrow" << std::endl;
            break;
        case XK_Up:
            std::cout << "Up Arrow" << std::endl;
            break;
        case XK_Down:
            std::cout << "Down Arrow" << std::endl;
            break;
            //Store the character that the user enters in vectors used for comparing with intersection and POI names
        case XK_A:
            if (find_intersection_enable == true)
                searched_text.push_back('A');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('A');
            break;
        case XK_B:
            if (find_intersection_enable == true)
                searched_text.push_back('B');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('B');
            break;
        case XK_C:
            if (find_intersection_enable == true)
                searched_text.push_back('C');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('C');
            break;
        case XK_D:
            if (find_intersection_enable == true)
                searched_text.push_back('D');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('D');
            break;
        case XK_E:
            if (find_intersection_enable == true)
                searched_text.push_back('E');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('E');
            break;
        case XK_F:
            if (find_intersection_enable == true)
                searched_text.push_back('F');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('F');
            break;
        case XK_G:
            if (find_intersection_enable == true)
                searched_text.push_back('G');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('G');
            break;
        case XK_H:
            if (find_intersection_enable == true)
                searched_text.push_back('H');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('H');
            break;
        case XK_I:
            if (find_intersection_enable == true)
                searched_text.push_back('I');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('I');
            break;
        case XK_J:
            if (find_intersection_enable == true)
                searched_text.push_back('J');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('J');
            break;
        case XK_K:
            if (find_intersection_enable == true)
                searched_text.push_back('K');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('K');
            break;
        case XK_L:
            if (find_intersection_enable == true)
                searched_text.push_back('L');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('L');
            break;
        case XK_M:
            if (find_intersection_enable == true)
                searched_text.push_back('M');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('M');
            break;
        case XK_N:
            if (find_intersection_enable == true)
                searched_text.push_back('N');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('N');
            break;
        case XK_O:
            if (find_intersection_enable == true)
                searched_text.push_back('O');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('O');
            break;
        case XK_P:
            if (find_intersection_enable == true)
                searched_text.push_back('P');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('P');
            break;
        case XK_Q:
            if (find_intersection_enable == true)
                searched_text.push_back('Q');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('Q');
            break;
        case XK_R:
            if (find_intersection_enable == true)
                searched_text.push_back('R');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('R');
            break;
        case XK_S:
            if (find_intersection_enable == true)
                searched_text.push_back('S');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('S');
            break;
        case XK_T:
            if (find_intersection_enable == true)
                searched_text.push_back('T');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('T');
            break;
        case XK_U:
            if (find_intersection_enable == true)
                searched_text.push_back('U');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('U');
            break;
        case XK_V:
            if (find_intersection_enable == true)
                searched_text.push_back('V');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('V');
            break;
        case XK_W:
            if (find_intersection_enable == true)
                searched_text.push_back('W');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('W');
            break;
        case XK_X:
            if (find_intersection_enable == true)
                searched_text.push_back('X');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('X');
            break;
        case XK_Y:
            if (find_intersection_enable == true)
                searched_text.push_back('Y');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('Y');
            break;
        case XK_Z:
            if (find_intersection_enable == true)
                searched_text.push_back('Z');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('Z');
            break;
        case XK_a:
            if (find_intersection_enable == true)
                searched_text.push_back('a');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('a');
            break;
        case XK_b:
            if (find_intersection_enable == true)
                searched_text.push_back('b');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('b');
            break;
        case XK_c:
            if (find_intersection_enable == true)
                searched_text.push_back('c');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('c');
            break;
        case XK_d:
            if (find_intersection_enable == true)
                searched_text.push_back('d');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('d');
            break;
        case XK_e:
            if (find_intersection_enable == true)
                searched_text.push_back('e');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('e');
            break;
        case XK_f:
            if (find_intersection_enable == true)
                searched_text.push_back('f');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('f');
            break;
        case XK_g:
            if (find_intersection_enable == true)
                searched_text.push_back('g');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('g');
            break;
        case XK_h:
            if (find_intersection_enable == true)
                searched_text.push_back('h');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('h');
            break;
        case XK_i:
            if (find_intersection_enable == true)
                searched_text.push_back('i');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('i');
            break;
        case XK_j:
            if (find_intersection_enable == true)
                searched_text.push_back('j');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('j');
            break;
        case XK_k:
            if (find_intersection_enable == true)
                searched_text.push_back('k');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('k');
            break;
        case XK_l:
            if (find_intersection_enable == true)
                searched_text.push_back('l');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('l');
            break;
        case XK_m:
            if (find_intersection_enable == true)
                searched_text.push_back('m');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('m');
            break;
        case XK_n:
            if (find_intersection_enable == true)
                searched_text.push_back('n');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('n');
            break;
        case XK_o:
            if (find_intersection_enable == true)
                searched_text.push_back('o');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('o');
            break;
        case XK_p:
            if (find_intersection_enable == true)
                searched_text.push_back('p');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('p');
            break;
        case XK_q:
            if (find_intersection_enable == true)
                searched_text.push_back('q');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('q');
            break;
        case XK_r:
            if (find_intersection_enable == true)
                searched_text.push_back('r');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('r');
            break;
        case XK_s:
            if (find_intersection_enable == true)
                searched_text.push_back('s');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('s');
            break;
        case XK_t:
            if (find_intersection_enable == true)
                searched_text.push_back('t');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('t');
            break;
        case XK_u:
            if (find_intersection_enable == true)
                searched_text.push_back('u');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('u');
            break;
        case XK_v:
            if (find_intersection_enable == true)
                searched_text.push_back('v');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('v');
            break;
        case XK_w:
            if (find_intersection_enable == true)
                searched_text.push_back('w');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('w');
            break;
        case XK_x:
            if (find_intersection_enable == true)
                searched_text.push_back('x');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('x');
            break;
        case XK_y:
            if (find_intersection_enable == true)
                searched_text.push_back('y');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('y');
            break;
        case XK_z:
            if (find_intersection_enable == true)
                searched_text.push_back('z');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('z');
            break;
        case XK_space:
            if (find_intersection_enable == true)
                searched_text.push_back(' ');
            else if (find_POI_enable == true)
                searched_text_POI.push_back(' ');
            break;
        case XK_BackSpace:
            //If backspace is entered, remove the last character of the vector
            if (find_intersection_enable == true) {
                if (searched_text.size() != 0) {
                    searched_text.pop_back();
                    setcolor(WHITE);
                    if (searched_text.size() == 0)
                        draw_screen();
                }
            }
            if (find_POI_enable == true) {
                if (searched_text_POI.size() != 0) {
                    searched_text_POI.pop_back();
                    setcolor(WHITE);
                    if (searched_text_POI.size() == 0)
                        draw_screen();
                }
            }
            break;
        case XK_ampersand:
            if (find_intersection_enable == true)
                searched_text.push_back('&');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('&');
            break;
        case XK_1:
            if (find_intersection_enable == true)
                searched_text.push_back('1');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('1');
            break;
        case XK_2:
            if (find_intersection_enable == true)
                searched_text.push_back('2');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('2');
            break;
        case XK_3:
            if (find_intersection_enable == true)
                searched_text.push_back('3');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('3');
            break;
        case XK_4:
            if (find_intersection_enable == true)
                searched_text.push_back('4');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('4');
            break;
        case XK_5:
            if (find_intersection_enable == true)
                searched_text.push_back('5');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('5');
            break;
        case XK_6:
            if (find_intersection_enable == true)
                searched_text.push_back('6');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('6');
            break;
        case XK_7:
            if (find_intersection_enable == true)
                searched_text.push_back('7');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('7');
            break;
        case XK_8:
            if (find_intersection_enable == true)
                searched_text.push_back('8');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('8');
            break;
        case XK_9:
            if (find_intersection_enable == true)
                searched_text.push_back('9');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('9');
            break;
        case XK_0:
            if (find_intersection_enable == true)
                searched_text.push_back('0');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('0');
            break;
        case XK_apostrophe:
            if (find_intersection_enable == true)
                searched_text.push_back('\'');
            else if (find_POI_enable == true)
                searched_text_POI.push_back('\'');
            break;
        case XK_Return:
            //When enter is pressed and the name user entered matches a POI name or an intersection name, display on screen
            if (find_intersection_enable == true) {
                if (searched_text.empty() != true) {
                    searched = string(searched_text.begin(), searched_text.end());
                    if (DataStructure::intersection_names_to_ids.find(searched) != DataStructure::intersection_names_to_ids.end()) {
                        DataStructure::intersection_ids.push_back(DataStructure::intersection_names_to_ids[searched]);
                        searched_text.clear();
                        search_bar_clicked = false;
                        LatLon tmp_pos = getIntersectionPosition(DataStructure::intersection_names_to_ids[searched]);
                        float point1_lat = tmp_pos.lat * DEG_TO_RAD;
                        float point1_lon = tmp_pos.lon * DEG_TO_RAD;

                        float lat_average = (maxLat + minLat) / 2;

                        float x_tmp = point1_lon * cos(lat_average);
                        float y_tmp = point1_lat;
                        set_visible_world(x_tmp - 0.00006, y_tmp - 0.00006, x_tmp + 0.00006, y_tmp + 0.00006);
                        find_intersection_enable = false;
                    }
                }
                draw_screen();
            } else if (find_POI_enable == true) {
                if (searched_text_POI.empty() != true) {
                    searched_POI = string(searched_text_POI.begin(), searched_text_POI.end());
                    if (DataStructure::POI_names_to_ids.find(searched_POI) != DataStructure::POI_names_to_ids.end()) {
                        auto range = DataStructure::POI_names_to_ids.equal_range(searched_POI);
                        for (auto it = range.first; it != range.second; ++it) {
                            DataStructure::chosen_POI_names.push_back(it->second);
                            LatLon temp_pos = getPointOfInterestPosition(it->second);
                            DataStructure::chosen_POIs.push_back(temp_pos);
                        }
                        searched_text_POI.clear();
                        search_bar_clicked_POI = false;
                        find_POI_enable = false;
                    }
                }
                draw_screen();
            }
            break;


        default:
            //std::cout << "keysym (extended code) is " << keysym << std::endl;
            break;
    }
    if (find_intersection_enable == true) //Draw the name searched by user one more time;
        draw_searched_text();
    else if (find_POI_enable == true)
        draw_searched_text_POI();
#endif
}

void draw_search_bar() {
    //Set coordinates for search bar
    x1Search = get_visible_world().left();
    y1Search = get_visible_world().bottom();
    x2Search = get_visible_world().right();
    y2Search = get_visible_world().top();
    x_centre_search = (x2Search - x1Search) / 2;
    y_centre_search = (y2Search - y1Search) / 2;
    search_bar_x1 = x1Search + x_centre_search + x_centre_search * 0.05;
    search_bar_y1 = y1Search + y_centre_search + y_centre_search * 0.9;
    //Draw the search bar
    setcolor(DARKGREY);
    fillrect(search_bar_x1, search_bar_y1, x2Search, y2Search);
    setcolor(WHITE);
    inner_search_bar_x1 = x1Search + x_centre_search + x_centre_search * 0.06;
    inner_search_bar_y1 = y1Search + y_centre_search + y_centre_search * 0.91;
    fillrect(inner_search_bar_x1, inner_search_bar_y1, x2Search, y2Search);
}

void draw_search_text() {
    //Draw default search bar text telling user to click to search
    string search_text("Click to search intersection");
    setfontsize(30);
    setcolor(DARKGREY);
    float search_centre_x = search_bar_x1 + (x2Search - search_bar_x1) / 2;
    float search_centre_y = search_bar_y1 + (y2Search - search_bar_y1) / 2;
    drawtext(search_centre_x, search_centre_y, search_text, FLT_MAX, FLT_MAX);
}

//Draws what user enters on search bar character by character

void draw_searched_text() {
    //Vector that stores matches between user input and stored intersection names
    vector <string> matching_text;

    //Draw inner search bar to make it look nicer
    setcolor(DARKGREY);
    fillrect(search_bar_x1, search_bar_y1, x2Search, y2Search);
    setcolor(WHITE);
    inner_search_bar_x1 = x1Search + x_centre_search + x_centre_search * 0.06;
    inner_search_bar_y1 = y1Search + y_centre_search + y_centre_search * 0.91;
    inner_search_bar_y2 = y1Search + y_centre_search + y_centre_search * 0.81;
    inner_search_bar_y3 = y1Search + y_centre_search + y_centre_search * 0.71;
    inner_search_bar_y4 = y1Search + y_centre_search + y_centre_search * 0.61;
    inner_search_bar_y5 = y1Search + y_centre_search + y_centre_search * 0.51;
    fillrect(inner_search_bar_x1, inner_search_bar_y1, x2Search, y2Search);

    //What user has searched
    searched = string(searched_text.begin(), searched_text.end());

    //If user input partially matches intersection names, store it in a vector
    const char *tmp1 = searched.c_str();
    //    cout << strlen(tmp1) << endl;
    for (unsigned i = 0; i < DataStructure::all_intersection_names.size(); i++) {
        const char *tmp2 = DataStructure::all_intersection_names[i].c_str();
        if (strncmp(tmp2, tmp1, strlen(tmp1)) == 0 && strlen(tmp1) != 0) {
            for (unsigned z = 0; z < strlen(tmp2); z++) {
                if (tmp2[z] == '&')
                    matching_text.push_back(DataStructure::all_intersection_names[i]);
            }
        }
    }

    //Centre coordinate for texts to be drawn on the search bar
    float search_centre_x = search_bar_x1 + (x2Search - search_bar_x1) / 2;
    float search_centre_y = search_bar_y1 + (y2Search - search_bar_y1) / 2;

    if (strlen(tmp1) != 0 && search_bar_clicked == true) {
        //Draw matching texts as a drop down menu
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y2, x2Search, search_bar_y1);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(search_bar_x1, inner_search_bar_y2, x2Search, inner_search_bar_y2);
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y3, x2Search, inner_search_bar_y2);
        setcolor(BLACK);
        drawline(search_bar_x1, inner_search_bar_y3, x2Search, inner_search_bar_y3);
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y4, x2Search, inner_search_bar_y3);
        setcolor(BLACK);
        drawline(search_bar_x1, inner_search_bar_y4, x2Search, inner_search_bar_y4);
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y5, x2Search, inner_search_bar_y4);
        setcolor(BLACK);
        setfontsize(10);
        float text_centre_y1 = inner_search_bar_y2 + (search_bar_y1 - inner_search_bar_y2) / 2;
        float text_centre_y2 = inner_search_bar_y3 + (inner_search_bar_y2 - inner_search_bar_y3) / 2;
        float text_centre_y3 = inner_search_bar_y4 + (inner_search_bar_y3 - inner_search_bar_y4) / 2;
        float text_centre_y4 = inner_search_bar_y5 + (inner_search_bar_y4 - inner_search_bar_y5) / 2;
        if (matching_text.size() > 0) {
            drawtext(search_centre_x, text_centre_y1, matching_text[0], FLT_MAX, FLT_MAX);
            matching_text_exists1 = true;
            first_inter = matching_text[0];
        } else if (matching_text.size() == 0)
            matching_text_exists1 = false;
        if (matching_text.size() > 1) {
            drawtext(search_centre_x, text_centre_y2, matching_text[1], FLT_MAX, FLT_MAX);
            matching_text_exists2 = true;
            second_inter = matching_text[1];
        } else if (matching_text.size() <= 1)
            matching_text_exists2 = false;
        if (matching_text.size() > 2) {
            drawtext(search_centre_x, text_centre_y3, matching_text[2], FLT_MAX, FLT_MAX);
            matching_text_exists3 = true;
            third_inter = matching_text[2];
        } else if (matching_text.size() <= 2)
            matching_text_exists3 = false;
        if (matching_text.size() > 3) {
            drawtext(search_centre_x, text_centre_y4, matching_text[3], FLT_MAX, FLT_MAX);
            matching_text_exists4 = true;
            fourth_inter = matching_text[3];
        } else if (matching_text.size() <= 3)
            matching_text_exists3 = false;
        setcolor(BLACK);
        setfontsize(15);
        drawtext(search_centre_x, search_centre_y, searched, FLT_MAX, FLT_MAX);
        //If user input is empty, draw default search bar text
    } else if (strlen(tmp1) == 0 && search_bar_clicked == false) {
        draw_search_text();
    }

}

void act_on_find_inter_func(void (*drawscreen_ptr) (void)) {
    find_intersection_enable = true;
    find_POI_enable = false;
    search_bar_clicked_POI = false;
    searched_text_POI.clear();
    draw_screen();
}

void act_on_find_POI_func(void (*drawscreen_ptr) (void)) {
    find_POI_enable = true;
    find_intersection_enable = false;
    search_bar_clicked = false;
    searched_text.clear();
    draw_screen();
}

void draw_search_bar_POI() {
    //Set coordinates for search bar for POI
    x1Search = get_visible_world().left();
    y1Search = get_visible_world().bottom();
    x2Search = get_visible_world().right();
    y2Search = get_visible_world().top();
    x_centre_search = (x2Search - x1Search) / 2;
    y_centre_search = (y2Search - y1Search) / 2;
    search_bar_x1 = x1Search + x_centre_search + x_centre_search * 0.05;
    search_bar_y1 = y1Search + y_centre_search + y_centre_search * 0.9;
    //Draw search bar for POI
    setcolor(DARKGREY);
    fillrect(search_bar_x1, search_bar_y1, x2Search, y2Search);
    setcolor(WHITE);
    inner_search_bar_x1 = x1Search + x_centre_search + x_centre_search * 0.06;
    inner_search_bar_y1 = y1Search + y_centre_search + y_centre_search * 0.91;
    fillrect(inner_search_bar_x1, inner_search_bar_y1, x2Search, y2Search);
}

void draw_search_text_POI() {
    //Draw default search text for POI
    string search_text("Click to search POI");
    setfontsize(30);
    setcolor(DARKGREY);
    float search_centre_x = search_bar_x1 + (x2Search - search_bar_x1) / 2;
    float search_centre_y = search_bar_y1 + (y2Search - search_bar_y1) / 2;
    drawtext(search_centre_x, search_centre_y, search_text, FLT_MAX, FLT_MAX);
}

void draw_searched_text_POI() {
    //Vector that stores matches between user input and stored POI names
    vector <string> matching_text_POI;

    //Draw inner search bar
    setcolor(DARKGREY);
    fillrect(search_bar_x1, search_bar_y1, x2Search, y2Search);
    setcolor(WHITE);
    inner_search_bar_x1 = x1Search + x_centre_search + x_centre_search * 0.06;
    inner_search_bar_y1 = y1Search + y_centre_search + y_centre_search * 0.91;
    inner_search_bar_y2 = y1Search + y_centre_search + y_centre_search * 0.81;
    inner_search_bar_y3 = y1Search + y_centre_search + y_centre_search * 0.71;
    inner_search_bar_y4 = y1Search + y_centre_search + y_centre_search * 0.61;
    inner_search_bar_y5 = y1Search + y_centre_search + y_centre_search * 0.51;
    fillrect(inner_search_bar_x1, inner_search_bar_y1, x2Search, y2Search);

    //What user has searched
    searched_POI = string(searched_text_POI.begin(), searched_text_POI.end());

    //If what user entered partially matches POI names stored, then store the POI name in a vector
    const char *tmp1 = searched_POI.c_str();
    //    cout << strlen(tmp1) << endl;
    for (unsigned i = 0; i < DataStructure::all_POI_names.size(); i++) {
        const char *tmp2 = DataStructure::all_POI_names[i].c_str();
        if (strncmp(tmp2, tmp1, strlen(tmp1)) == 0 && strlen(tmp1) != 0) {
            matching_text_POI.push_back(DataStructure::all_POI_names[i]);
        }
    }

    //Centre for texts on search bar
    float search_centre_x = search_bar_x1 + (x2Search - search_bar_x1) / 2;
    float search_centre_y = search_bar_y1 + (y2Search - search_bar_y1) / 2;
    if (strlen(tmp1) != 0 && search_bar_clicked_POI == true) {
        //Draw matching POI names as a drop down menu below search bar
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y2, x2Search, search_bar_y1);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(search_bar_x1, inner_search_bar_y2, x2Search, inner_search_bar_y2);
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y3, x2Search, inner_search_bar_y2);
        setcolor(BLACK);
        drawline(search_bar_x1, inner_search_bar_y3, x2Search, inner_search_bar_y3);
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y4, x2Search, inner_search_bar_y3);
        setcolor(BLACK);
        drawline(search_bar_x1, inner_search_bar_y4, x2Search, inner_search_bar_y4);
        setcolor(WHITE);
        fillrect(search_bar_x1, inner_search_bar_y5, x2Search, inner_search_bar_y4);
        setcolor(BLACK);
        setfontsize(10);
        float text_centre_y1 = inner_search_bar_y2 + (search_bar_y1 - inner_search_bar_y2) / 2;
        float text_centre_y2 = inner_search_bar_y3 + (inner_search_bar_y2 - inner_search_bar_y3) / 2;
        float text_centre_y3 = inner_search_bar_y4 + (inner_search_bar_y3 - inner_search_bar_y4) / 2;
        float text_centre_y4 = inner_search_bar_y5 + (inner_search_bar_y4 - inner_search_bar_y5) / 2;
        if (matching_text_POI.size() > 0) {
            drawtext(search_centre_x, text_centre_y1, matching_text_POI[0], FLT_MAX, FLT_MAX);
            matching_text_exists1_POI = true;
            first_POI = matching_text_POI[0];
        } else if (matching_text_POI.size() == 0)
            matching_text_exists1_POI = false;
        if (matching_text_POI.size() > 1) {
            drawtext(search_centre_x, text_centre_y2, matching_text_POI[1], FLT_MAX, FLT_MAX);
            matching_text_exists2_POI = true;
            second_POI = matching_text_POI[1];
        } else if (matching_text_POI.size() <= 1)
            matching_text_exists2_POI = false;
        if (matching_text_POI.size() > 2) {
            drawtext(search_centre_x, text_centre_y3, matching_text_POI[2], FLT_MAX, FLT_MAX);
            matching_text_exists3_POI = true;
            third_POI = matching_text_POI[2];
        } else if (matching_text_POI.size() <= 2)
            matching_text_exists3_POI = false;
        if (matching_text_POI.size() > 3) {
            drawtext(search_centre_x, text_centre_y4, matching_text_POI[3], FLT_MAX, FLT_MAX);
            matching_text_exists4_POI = true;
            fourth_POI = matching_text_POI[3];
        } else if (matching_text_POI.size() <= 3)
            matching_text_exists3_POI = false;
        setcolor(BLACK);
        setfontsize(15);
        drawtext(search_centre_x, search_centre_y, searched_POI, FLT_MAX, FLT_MAX);

        //if user input is empty, draw default search text 
    } else if (strlen(tmp1) == 0 && search_bar_clicked_POI == false) {
        draw_search_text_POI();
    }

}

void draw_shortest_edges_with_curve_point2(LatLon position_1, LatLon position_2, StreetSegmentInfo info) {

    float radius;
    float left = get_visible_world().left();
    float right = get_visible_world().right();
    float bottom = get_visible_world().bottom();
    float top = get_visible_world().top();

    float point1_lat, point1_lon;
    float point2_lat, point2_lon;
    float lat_average;
    float x_start, x_finish;
    float y_start, y_finish;

    OSMID way_ID = info.wayOSMID;
    unsigned street_type;
    point1_lat = position_1.lat * DEG_TO_RAD;
    point1_lon = position_1.lon * DEG_TO_RAD;

    point2_lat = position_2.lat * DEG_TO_RAD;
    point2_lon = position_2.lon * DEG_TO_RAD;

    lat_average = (maxLat + minLat) / 2;

    x_start = point1_lon * cos(lat_average);
    x_finish = point2_lon * cos(lat_average);
    y_start = point1_lat;
    y_finish = point2_lat;

    //Gets current area of the screen, to compare it with starting area. 
    temp_Area = get_visible_world().area();
    street_type = DataStructure::OSM_street_segs_num_of_lanes[way_ID]; //gets street_type from database
    setcolor(RED);

    // Selective display, checks if segment is inside current screen.
    if (((left < x_start) || (left < x_finish)) && ((right > x_start) || (right > x_finish)) &&
            ((bottom < y_start) || (bottom < y_finish)) && ((top > y_start) || (top > y_finish))) {

        //Given different ratios between temp_area(current screen) and startArea (initial screen). The linewidths and what is drawn is different.
        //Line widths get larger, as the ratio between the temp and start get smaller.
        //Largest number, displays no minor roads.


        if (temp_Area / startArea < 0.0001) {//smallest zoom level, screen cannot zoom past this (Modifed Graphics.cpp, handle zoom function)
            radius = 400e-9;
            if (street_type == 4) { //Highway ramps

                setlinewidth(28);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);


            } else if (street_type == 3) { //Highway

                setlinewidth(30);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 580e-9, 0, 360);
                fillarc(x_finish, y_finish, 580e-9, 0, 360);

            } else if (street_type == 2) {// Major roads


                setlinestyle(SOLID);
                setlinewidth(26);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 540e-9, 0, 360);
                fillarc(x_finish, y_finish, 540e-9, 0, 360);

            } else if (street_type == 1) { //Minor roads

                setlinestyle(SOLID);
                setlinewidth(22);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 510e-9, 0, 360);
                fillarc(x_finish, y_finish, 510e-9, 0, 360);

            }
        } else if (temp_Area / startArea < 0.0008) {
            radius = 410e-9;
            if (street_type == 4) {

                setlinewidth(20);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);
            } else if (street_type == 3) {

                setlinewidth(23);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 660e-9, 0, 360);
                fillarc(x_finish, y_finish, 660e-9, 0, 360);
            } else if (street_type == 2) {

                setlinestyle(SOLID);
                setlinewidth(15);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 440e-9, 0, 360);
                fillarc(x_finish, y_finish, 440e-9, 0, 360);


            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(10);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, radius, 0, 360);
                fillarc(x_finish, y_finish, radius, 0, 360);
            }
        } else if (temp_Area / startArea < 0.001) {
            radius = 380e-9;
            if (street_type == 4) {

                setlinewidth(11);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 410e-9, 0, 360);
                fillarc(x_finish, y_finish, 410e-9, 0, 360);
            } else if (street_type == 3) {

                setlinewidth(14);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 410e-9, 0, 360);
                fillarc(x_finish, y_finish, 410e-9, 0, 360);
            } else if (street_type == 2) {


                setlinestyle(SOLID);
                setlinewidth(10);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, radius, 0, 360);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(6);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_start, y_start, 400e-9, 0, 360);
                fillarc(x_finish, y_finish, 400e-9, 0, 360);
            }
        } else if (temp_Area / startArea < 0.01) {
            radius = 250e-9;
            if (street_type == 4) {

                setlinewidth(8);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 3) {

                setlinewidth(10);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 2) {

                setlinestyle(SOLID);
                setlinewidth(6);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(2);
                drawline(x_start, y_start, x_finish, y_finish);
                fillarc(x_finish, y_finish, radius, 0, 360);
            }
        } else if (temp_Area / startArea < 0.1) {
            //cout << "0.5" << endl;
            if (street_type == 4) {

                setlinewidth(5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 3) {

                setlinewidth(7);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 2) {


                setlinestyle(SOLID);
                setlinewidth(3.5);
                drawline(x_start, y_start, x_finish, y_finish);

            } else if (street_type == 1) {

                setlinestyle(SOLID);
                setlinewidth(1.5);
                drawline(x_start, y_start, x_finish, y_finish);
            }

        } else if (temp_Area / startArea < 10) { //Largest zoom level, past initial screen size

            if (street_type == 4) {

                setlinewidth(4);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 3) {

                setlinewidth(5);
                setlinestyle(SOLID);
                drawline(x_start, y_start, x_finish, y_finish);
            } else if (street_type == 2) {

                setlinestyle(SOLID);
                setlinewidth(3);
                drawline(x_start, y_start, x_finish, y_finish);

            }
        }
    }

}

void act_on_find_path_button_func(void (*drawscreen_ptr) (void)) {
    if (DataStructure::intersection_ids.empty() == false) {
        id1 = DataStructure::intersection_ids[0];
        id2 = DataStructure::intersection_ids[1];
        display_directions_enable = true;
        find_path_between_intersections(id1, id2);
        draw_screen();
    }
}


//Display directions when path is being shown

void display_directions() {
    //Marking start point and finish point of the path
    string start("Start");
    string finish("Finish");

    //2 points that form the path
    LatLon int_position1 = getIntersectionPosition(DataStructure::intersection_ids[0]);
    LatLon int_position2 = getIntersectionPosition(DataStructure::intersection_ids[1]);

    //Get x,y positions from latlon
    float int1_lat = int_position1.lat * DEG_TO_RAD;
    float int1_lon = int_position1.lon * DEG_TO_RAD;

    float int2_lat = int_position2.lat * DEG_TO_RAD;
    float int2_lon = int_position2.lon * DEG_TO_RAD;

    float lat_average = (maxLat + minLat) / 2;

    float x_start = int1_lon * cos(lat_average);
    float x_finish = int2_lon * cos(lat_average);
    float y_start = int1_lat;
    float y_finish = int2_lat;

    //Draw start/finish marks on screen
    setfontsize(20);
    setcolor(RED);
    drawtext(x_start, y_start, start, FLT_MAX, FLT_MAX);
    drawtext(x_finish, y_finish, finish, FLT_MAX, FLT_MAX);

    //Define variables used for displaying directions and coordinates for drawing
    unsigned start_node = DataStructure::intersection_ids[0];
    unsigned to_from_connected, current_street_id, previous_street_id;
    setcolor(WHITE);
    x1Search = get_visible_world().left();
    y1Search = get_visible_world().bottom();
    x2Search = get_visible_world().right();
    y2Search = get_visible_world().top();
    x_centre_search = (x2Search - x1Search) / 2;
    y_centre_search = (y2Search - y1Search) / 2;
    float text_centre_x = ((x1Search + x_centre_search * 0.8) - x1Search) / 2;

    //Draw tab on the left that will display travel directions
    fillrect(x1Search, y1Search, x1Search + x_centre_search * 0.8, y2Search);
    setcolor(BLACK);
    setlinewidth(3);
    float mult_factor = 0.95;
    float street_length;
    int counter = 0;
    string first_street, second_street;
    unsigned starting_each_street, starting_each_street_for, end_previous_street, starting_previous_street, end_current_street;
    drawline(x1Search + x_centre_search * 0.8, y1Search, x1Search + x_centre_search * 0.8, y2Search);
    vector<unsigned> street_segs_path = find_path_between_intersections(id1, id2);
    ;

    //Go through the street segments of the found path
    for (unsigned i = 0; i < street_segs_path.size(); i++) {
        string direction_text, distance_text, name_text; //Direction of street, distance of street, name of street
        StreetSegmentInfo temp_info = getStreetSegmentInfo(street_segs_path[i]);

        //Mark street if of previous street segment
        if (i > 0) {
            StreetSegmentInfo temp_info2 = getStreetSegmentInfo(street_segs_path[i - 1]);
            previous_street_id = temp_info2.streetID;
        }
        current_street_id = temp_info.streetID;

        //If from/to nodes are not connected, switch them
        if (i == 0 && temp_info.from != start_node) {
            unsigned temp = temp_info.from;
            temp_info.from = temp_info.to;
            temp_info.to = temp;
        }

        //When its the first street segment of the path
        if (i == 0) {
            to_from_connected = temp_info.to;
            street_length = (int) find_street_segment_length(street_segs_path[i]);
            name_text = getStreetName(current_street_id);
            starting_each_street = DataStructure::intersection_ids[0];
            setfontsize(12);
            setcolor(BLACK);
            drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * 0.95, name_text, FLT_MAX, FLT_MAX);
        }

        //If from/to nodes are not connected, switch them
        if (i > 0 && temp_info.from != to_from_connected) {
            unsigned temp = temp_info.from;
            temp_info.from = temp_info.to;
            temp_info.to = temp;
        }

        //If street has changed
        if (i > 0 && current_street_id != previous_street_id) {
            distance_text = "Go for " + to_string((int) street_length) + "meters";
            setfontsize(8);
            setcolor(BLACK);
            drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor - 0.03), distance_text, FLT_MAX, FLT_MAX);
            street_length = (int) find_street_segment_length(street_segs_path[i]);
            name_text = getStreetName(current_street_id);
        }

        //If street is the same and last street segment has been reached
        if (current_street_id == previous_street_id && street_segs_path[i] == street_segs_path.back()) {
            distance_text = "Go for " + to_string((int) street_length) + "meters";
            setfontsize(8);
            setcolor(BLACK);
            drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor - 0.03), distance_text, FLT_MAX, FLT_MAX);
        }

        //If street has changed and last street segment has been reached
        if (current_street_id != previous_street_id && street_segs_path[i] == street_segs_path.back()) {
            starting_each_street_for = starting_each_street;
        }

        //When still on the same street as before
        if (i > 0 && current_street_id == previous_street_id) {
            street_length = street_length + (int) find_street_segment_length(street_segs_path[i]);
        }
        to_from_connected = temp_info.to;

        //When street has changed
        if (i > 0 && current_street_id != previous_street_id) {
            //Set up points to calculate angle
            StreetSegmentInfo temp = getStreetSegmentInfo(street_segs_path[i - 1]);
            if (temp.to != temp_info.from) {
                unsigned temp_swap = temp.to;
                temp.to = temp.from;
                temp.from = temp_swap;
            }
            LatLon temp1 = getIntersectionPosition(temp.from);
            LatLon temp2 = getIntersectionPosition(temp_info.to);
            starting_each_street = end_previous_street;
            end_previous_street = temp_info.from;
            LatLon temp3 = getIntersectionPosition(temp_info.from);
            float temp3_lat = temp3.lat * DEG_TO_RAD;
            float temp3_lon = temp3.lon * DEG_TO_RAD;

            float lat_average = (maxLat + minLat) / 2;

            float x_temp3 = temp3_lon * cos(lat_average);
            float y_temp3 = temp3_lat;
            //converting latitudes and longitudes to x-y coordinates 
            float temp1_lat = temp1.lat * DEG_TO_RAD;
            float temp1_lon = temp1.lon * DEG_TO_RAD;

            float temp2_lat = temp2.lat * DEG_TO_RAD;
            float temp2_lon = temp2.lon * DEG_TO_RAD;

            float x_start = temp1_lon * cos(lat_average);
            float x_finish = temp2_lon * cos(lat_average);
            float y_start = temp1_lat;
            float y_finish = temp2_lat;

//            setcolor(BLUE);
//            fillarc(x_finish, y_finish, 660e-8, 0, 360);
//            setcolor(BLACK);
//            fillarc(x_temp3, y_temp3, 660e-8, 0, 360);
//            setcolor(RED);
//            fillarc(x_start, y_start, 660e-8, 0, 360);

            double x_avg = ((x_finish + x_start) / 2.0000000000);
            double y_avg = ((y_finish + y_start) / 2.0000000000);

            // Use of Pythagorean Theorem
            double temp_hypotenuse = find_distance_between_two_points(temp1, temp2);
            double temp_adjacent = (abs(find_distance_between_two_points(temp1, temp3))) * EARTH_RADIUS_IN_METERS;
            double temp_opposite = (abs(find_distance_between_two_points(temp3, temp2))) * EARTH_RADIUS_IN_METERS;
            double angle = 0;
            //Calculate angle depending on the street orientation, and display which direction should be taken according to the angles
            if (y_start < y_finish) {
                if (((y_start < y_finish) && (x_finish > x_start))) {
                    if (y_avg < y_temp3) {
                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly right onto";
                        else if (angle > 25)
                            direction_text = "Turn right onto";
                    } else if (y_avg > y_temp3) {
                        // cout << y_start << "  " << y_avg << "  " << y_temp3 << endl;
                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly left onto";
                        else if (angle > 25)
                            direction_text = "Turn left onto";
                    }
                } else if (((y_finish > y_start) && (x_start > x_finish))) {
                    if (y_avg < y_temp3) {
                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly left onto";
                        else if (angle > 25)
                            direction_text = "Turn left onto";
                    } else if (y_avg > y_temp3) {
                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly right onto";
                        else if (angle > 25)
                            direction_text = "Turn right onto";
                    }
                }
            }
            if (y_start > y_finish) {
                if (((y_finish < y_start) && (x_start < x_finish))) {
                    // cout << y_avg << "  " << y_temp3;
                    if (y_avg > y_temp3) {
                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly left onto";
                        else if (angle > 25)
                            direction_text = "Turn left onto";
                    } else if (y_avg < y_temp3) {
                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly right onto";
                        else if (angle > 25)
                            direction_text = "Turn right onto";
                    }
                } else if (((y_finish < y_start) && (x_start > x_finish))) {
                    if (y_avg < y_temp3) {
                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly left onto";
                        else if (angle > 25)
                            direction_text = "Turn left onto";
                    } else if (y_avg > y_temp3) {
                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
                        if (angle <= 10)
                            direction_text = "Go straight onto";
                        else if (angle > 10 && angle <= 25)
                            direction_text = "Turn slightly right onto";
                        else if (angle > 25)
                            direction_text = "Turn right onto";
                    }
                }
            }
            //Draw the changed street name
            mult_factor = mult_factor - 0.11;
            setfontsize(12);
            setcolor(BLACK);
            drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search*mult_factor, name_text, FLT_MAX, FLT_MAX);
            setfontsize(8);
            setcolor(BLACK);
            drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor + 0.03), direction_text, FLT_MAX, FLT_MAX);
            counter++;

        }//If it's the first change in streets within a path
        else if (i == 0) {
            //will only enter this if statement once because of this variable
            //Set up coordinates to calculate angles
            unsigned end_of_current_street;
            LatLon temp1 = getIntersectionPosition(temp_info.from);
            for (unsigned k = 1; getStreetSegmentInfo(street_segs_path[k - 1]).streetID == getStreetSegmentInfo(street_segs_path[k]).streetID; k++) {
                StreetSegmentInfo temp_for_swap = getStreetSegmentInfo(street_segs_path[k]);
                end_of_current_street = temp_for_swap.to;
            }
            LatLon temp2 = getIntersectionPosition(end_of_current_street);
            end_previous_street = temp_info.from;
            //starting_each_street = temp_info.from;

            //converting latitudes and longitudes to x-y coordinates 
            float temp1_lat = temp1.lat * DEG_TO_RAD;
            float temp1_lon = temp1.lon * DEG_TO_RAD;

            float temp2_lat = temp2.lat * DEG_TO_RAD;
            float temp2_lon = temp2.lon * DEG_TO_RAD;

            float lat_average = (maxLat + minLat) / 2;

            float x_start = temp1_lon * cos(lat_average);
            float x_finish = temp2_lon * cos(lat_average);
            float y_start = temp1_lat;
            float y_finish = temp2_lat;

            double x_avg = ((x_finish + x_start) / 2.0000000000);
            double y_avg = ((y_finish + y_start) / 2.0000000000);

            // Use of Pythagorean Theorem
            double temp_hypotenuse = find_distance_between_two_points(temp1, temp2);
            double temp_adjacent = (abs(y_finish - y_start)) * EARTH_RADIUS_IN_METERS;
            double temp_opposite = (abs(x_finish - x_start)) * EARTH_RADIUS_IN_METERS;
            double angle = 0;
            //        if (i == 0) {
            angle = (((asin(temp_opposite / temp_hypotenuse)) / (DEG_TO_RAD))); // returns angle in degrees

            //Calculate angles depending on the street orientation and display appropriate directions corresponding to the angles
            if (y_start < y_finish) {
                if (((y_start < y_finish) && (x_finish > x_start))) {
                    if (angle <= 30)
                        direction_text = "Go north on";
                    else if (angle > 30 && angle <= 70)
                        direction_text = "Go north east on";
                    else if (angle > 70)
                        direction_text = "Go east on";
                } else if (((y_finish > y_start) && (x_start > x_finish))) {
                    if (angle <= 30)
                        direction_text = "Go north on";
                    else if (angle > 30 && angle <= 70)
                        direction_text = "Go north west on";
                    else if (angle > 70)
                        direction_text = "Go west on";
                }
                setfontsize(8);
                setcolor(BLACK);
                drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor + 0.03), direction_text, FLT_MAX, FLT_MAX);
            }
            if (y_start > y_finish) {
                if (((y_finish < y_start) && (x_start < x_finish))) {
                    if (angle <= 30)
                        direction_text = "Go south on";
                    else if (angle > 30 && angle <= 70)
                        direction_text = "Go south east on";
                    else if (angle > 70)
                        direction_text = "Go east on";
                } else if (((y_finish < y_start) && (x_start > x_finish))) {
                    if (angle <= 30)
                        direction_text = "Go south on";
                    else if (angle > 30 && angle <= 70)
                        direction_text = "Go south west on";
                    else if (angle > 70)
                        direction_text = "Go west on";
                }
                setfontsize(8);
                setcolor(BLACK);
                drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor + 0.03), direction_text, FLT_MAX, FLT_MAX);
            }
            //Draw changed street name
        }

        //If the last street segment is a different street from the previous street segment
//        if (i > 0 && current_street_id != previous_street_id && street_segs_path[i] == street_segs_path.back()) {
//            //Set up coordinates to calculate angles
//            LatLon temp1 = getIntersectionPosition(starting_each_street);
//            LatLon temp2 = getIntersectionPosition(temp_info.to);
//            LatLon temp3 = getIntersectionPosition(temp_info.from);
//            float temp3_lat = temp3.lat * DEG_TO_RAD;
//            float temp3_lon = temp3.lon * DEG_TO_RAD;
//
//            float lat_average = (maxLat + minLat) / 2;
//
//            float x_temp3 = temp3_lon * cos(lat_average);
//            float y_temp3 = temp3_lat;
//            //converting latitudes and longitudes to x-y coordinates 
//            float temp1_lat = temp1.lat * DEG_TO_RAD;
//            float temp1_lon = temp1.lon * DEG_TO_RAD;
//
//            float temp2_lat = temp2.lat * DEG_TO_RAD;
//            float temp2_lon = temp2.lon * DEG_TO_RAD;
//
//            float x_start = temp1_lon * cos(lat_average);
//            float x_finish = temp2_lon * cos(lat_average);
//            float y_start = temp1_lat;
//            float y_finish = temp2_lat;
//
//            double x_avg = ((x_finish + x_start) / 2.0000000000);
//            double y_avg = ((y_finish + y_start) / 2.0000000000);
//
//            // Use of Pythagorean Theorem
//            double temp_hypotenuse = find_distance_between_two_points(temp1, temp2);
//            double temp_adjacent = (abs(find_distance_between_two_points(temp1, temp3))) * EARTH_RADIUS_IN_METERS;
//            double temp_opposite = (abs(find_distance_between_two_points(temp3, temp2))) * EARTH_RADIUS_IN_METERS;
//            double angle = 0;
//
//            //Calculate angles depending on the street orientation and display directions corresponding to the angles
//            if (y_start < y_finish) {
//
//                if (((y_start < y_finish) && (x_finish > x_start))) {
//                    if (y_avg < y_temp3) {
//                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly right onto";
//                        else if (angle > 15)
//                            direction_text = "Turn right onto";
//                    } else if (y_avg > y_temp3) {
//                        //cout << y_start << "  " << y_avg << "  " << y_temp3 << endl;
//                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly left onto";
//                        else if (angle > 15)
//                            direction_text = "Turn left onto";
//                    }
//                } else if (((y_finish > y_start) && (x_start > x_finish))) {
//                    if (y_avg < y_temp3) {
//                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly left onto";
//                        else if (angle > 15)
//                            direction_text = "Turn left onto";
//                    } else if (y_avg > y_temp3) {
//                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly right onto";
//                        else if (angle > 15)
//                            direction_text = "Turn right onto";
//                    }
//                }
//                setfontsize(8);
//                setcolor(BLACK);
//                drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor + 0.03), direction_text, FLT_MAX, FLT_MAX);
//            }
//            if (y_start > y_finish) {
//                if (((y_finish < y_start) && (x_start < x_finish))) {
//                    //cout << y_avg << "  " << y_temp3;
//                    if (y_avg > y_temp3) {
//                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly left onto";
//                        else if (angle > 15)
//                            direction_text = "Turn left onto";
//                    } else if (y_avg < y_temp3) {
//                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly right onto";
//                        else if (angle > 15)
//                            direction_text = "Turn right onto";
//                    }
//                } else if (((y_finish < y_start) && (x_start > x_finish))) {
//                    if (y_avg < y_temp3) {
//                        angle = (((atan(temp_adjacent / temp_opposite)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly left onto";
//                        else if (angle > 15)
//                            direction_text = "Turn left onto";
//                    } else if (y_avg > y_temp3) {
//                        angle = (((atan(temp_opposite / temp_adjacent)) / (DEG_TO_RAD))); // returns angle in degrees
//                        if (angle <= 5)
//                            direction_text = "Go straight onto";
//                        else if (angle > 5 && angle <= 15)
//                            direction_text = "Turn slightly right onto";
//                        else if (angle > 15)
//                            direction_text = "Turn right onto";
//                    }
//                }
//                setfontsize(8);
//                setcolor(BLACK);
//                drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor + 0.03), direction_text, FLT_MAX, FLT_MAX);
//            }
//            //Display the distance of this last street segment
//            setfontsize(8);
//            setcolor(BLACK);
//            street_length = (int) find_street_segment_length(street_segs_path[i]);
//            distance_text = "Go for " + to_string((int) street_length) + "meters";
//            drawtext(x1Search + text_centre_x, y1Search + y_centre_search + y_centre_search * (mult_factor - 0.03), distance_text, FLT_MAX, FLT_MAX);
//            counter++;
//        }
    }
}

void act_on_find_path_POI_button_func(void (*drawscreen_ptr) (void)) {
    if (DataStructure::intersection_ids.empty() == false) {
        string POI;
        unsigned id1 = DataStructure::intersection_ids[0];
        update_message("Displayed on Terminal.");

        cout << "Please enter the name of the intersection: " << endl;

        getline(cin, POI);

        find_path_to_point_of_interest(id1, POI);
        draw_screen();
    }
}

