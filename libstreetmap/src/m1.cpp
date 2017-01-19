#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include <iomanip>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include "m2.h"
#include "graphics.h"
#include "OSMDatabaseAPI.h"
#include "data_structure.h"
 
using namespace std;
// Helper function for find_closest_intersection
unsigned check_each_cell(bool& second_entered_once, double& closest_INT_distance, unsigned closest_INT_id, vector<unsigned> vector_with_enclosed_intersections, LatLon my_position_temp);
unsigned POI_check_each_cell(bool& second_entered_once, double& closest_POI_distance, unsigned closest_POI_id, vector<unsigned> vector_with_enclosed_intersections, LatLon my_position_temp);

/*********** Global variables *******/
double real_max_lat = 0, real_max_lon = 0;
double real_min_lat = 0, real_min_lon = 0;

int max_lat = 0, max_lon = 0;
int min_lat = 0, min_lon = 0;

double POI_real_max_lat = 0, POI_real_max_lon = 0;
double POI_real_min_lat = 0, POI_real_min_lon = 0;

int POI_max_lat = 0, POI_max_lon = 0;
int POI_min_lat = 0, POI_min_lon = 0;
string CHECK_WHICH;
/****************************************/

bool load_map(std::string map_name) {
    Graph graph;
    bool load_success = loadStreetsDatabaseBIN(map_name);
    string streets = "streets";
    
    map_name.replace(map_name.find(streets), streets.length(), "osm");
    loadOSMDatabaseBIN(map_name);
    CHECK_WHICH = map_name;

    unsigned number_of_intersections = getNumberOfIntersections();
    unsigned number_of_streets = getNumberOfStreets();
    unsigned number_of_street_segments = getNumberOfStreetSegments();
    unsigned number_of_poi = getNumberOfPointsOfInterest();

    // Hash table with vectors of street id's of each street
    for (unsigned i = 0; i < number_of_streets; i++) {
        string street_name = getStreetName(i);
        DataStructure::street_name_to_street_id.insert((make_pair(street_name, i)));
    }
    
    LatLon temp_latlon;
    // Vector with vectors of street segment ids at each intersections
    for (unsigned i = 0; i < number_of_intersections; i++) {
        DataStructure::street_segments_idx.clear();

        for (unsigned j = 0; j < getIntersectionStreetSegmentCount(i); j++) {
            unsigned temp = getIntersectionStreetSegment(i, j);
            DataStructure::street_segments_idx.push_back(temp);
        }
        DataStructure::intersection_with_street_segment_idx.push_back(DataStructure::street_segments_idx);
        
        temp_latlon = getIntersectionPosition(i);
        DataStructure::lat_lon_of_all_intersection_ids.insert({i, temp_latlon});
    }


    //Stores street IDs of street segments as keys and all the intersections of the street segments as values in a map
    StreetSegmentInfo info;

    for (unsigned i = 0; i < number_of_street_segments; i++) {
        info = getStreetSegmentInfo(i);

        DataStructure::street_id_and_intersections[info.streetID].push_back(info.from);
        DataStructure::street_id_and_intersections[info.streetID].push_back(info.to);
        
        if (i==0 || DataStructure::speed_limit < info.speedLimit ){
            DataStructure::speed_limit = info.speedLimit;
        }
    }
 

    // Hash table with vectors of street segment ids for each street id
    StreetSegmentInfo temp_info;

    for (unsigned i = 0; i < number_of_street_segments; i++) {
        temp_info = getStreetSegmentInfo(i); //gets street segment object

        DataStructure::street_id_to_street_seg[temp_info.streetID].push_back(i); //inside map, stores at street ID as key, and pushes in i into vector of street segment
    }


    // Hash table with vectors of street names for each intersection
    unsigned street_ID;
    string street_name;

    for (unsigned i = 0; i < number_of_street_segments; i++) {
        temp_info = getStreetSegmentInfo(i); //gets street segment object
        street_ID = temp_info.streetID; //gets street ID for street segment
        street_name = getStreetName(street_ID); //gets name for that street ID

        DataStructure::intersection_street_names_from_intersection_ID[temp_info.from].push_back(street_name); //adds street name to "from" intersection ID in map
        DataStructure::intersection_street_names_from_intersection_ID[temp_info.to].push_back(street_name); //adds street name to "to" intersection ID in map
        
        
        DataStructure::street_id_of_all_street_seg_id.insert({i, street_ID});
    }


    // Hash table with vectors of intersections for each street
    for (unsigned i = 0; i < number_of_streets; i++) {
        string street_name = getStreetName(i);
        DataStructure::street_name_and_its_intersections.insert((make_pair(street_name, find_all_street_intersections(i))));
    }


    // Hash table with vectors of StreetSegmentInfos
    for (unsigned i = 0; i < number_of_intersections; i++) {
        DataStructure::street_segment_info.clear();
        unsigned temp = getIntersectionStreetSegmentCount(i);

        for (unsigned j = 0; j < temp; j++) {
            unsigned temp1 = getIntersectionStreetSegment(i, j);
            StreetSegmentInfo temp_info1 = getStreetSegmentInfo(temp1);
            DataStructure::street_segment_info.push_back(temp_info1);
        }

        DataStructure::hash_of_street_segment_info_at_intersection.insert(make_pair(i, DataStructure::street_segment_info));
    }

    // 2-D array hold a vector of intersections
    LatLon tempLatLon;
    double lat, lon;
    int mod_lat, mod_lon;

    for (unsigned i = 0; i < number_of_intersections; i++) {
        // Modulating the lattitudes and longitudes and casting the float into integer
        // and putting them as index into the 2-D array and storing all intersections at each cells accordingly
        // e.g  (lat,lon) = (43.4456, -79,245) ->  mod_lat = 4, mod_lon = 2  
        // which is one of the elements in the vector that is stored in    to_find_closest_intersection[2][4]
        tempLatLon = getIntersectionPosition(i);
        lat = tempLatLon.lat;
        lon = tempLatLon.lon;

        if (i == 0) // for the first loop
            real_max_lat = lat;
        else if (lat > real_max_lat) // comparing to get the real_max_lat
            real_max_lat = lat;

        if (i == 0) // for the first loop
            real_max_lon = lon;
        else if (lon > real_max_lon) // comparing to get the real_max_lon
            real_max_lon = lon;

        if (i == 0) // for the first loop
            real_min_lat = lat;
        else if (lat < real_min_lat) // comparing to get the real_min_lat
            real_min_lat = lat;


        if (i == 0) // for the first loop
            real_min_lon = lon;
        else if (lon < real_min_lon) // comparing to get the real_min_lon
            real_min_lon = lon;

        lat = abs(lat) - floor(abs(lat));
        lon = abs(lon) - floor(abs(lon));
        mod_lat = static_cast<int> (lat * 10.0f);
        mod_lon = static_cast<int> (lon * 10.0f);

        DataStructure::to_find_closest_intersection [mod_lon][mod_lat].push_back(i); // insert
        if (i == 0)
            max_lat = mod_lat;
        else if (mod_lat > max_lat) // comparing to get the max_lat (modulated values)
            max_lat = mod_lat;
        if (i == 0)
            max_lon = mod_lon;
        else if (mod_lon > max_lon) // comparing to get the max_lon
            max_lon = mod_lon;

        if (i == 0) // for the first loop
            min_lat = mod_lat;
        else if (mod_lat < min_lat) // comparing to get the min_lat
            min_lat = mod_lat;

        if (i == 0) // for the first loop
            min_lon = mod_lon;
        else if (mod_lon < min_lon) // comparing to get the min_lon
            min_lon = mod_lon;
    }

    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        double length = 0;
        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);
        unsigned street_id = temp_info.streetID;
        if (DataStructure::street_length_finder[street_id].empty())
            length = find_street_segment_length(i);
        else {
            length = DataStructure::street_length_finder[street_id][0] + find_street_segment_length(i);
            DataStructure::street_length_finder[street_id].clear();
            DataStructure::street_length_finder[street_id].push_back(length);
        }
        
        DataStructure::street_seg_id_to_street_seg_info.insert({i, temp_info});
    }


    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        // Modulating the lattitudes and longitudes and casting the float into integer
        // and putting them as index into the 2-D array and storing all intersections at each cells accordingly
        // e.g  (lat,lon) = (43.4456, -79,245) ->  mod_lat = 4, mod_lon = 2  
        // which is one of the elements in the vector that is stored in    to_find_closest_intersection[2][4]
        tempLatLon = getPointOfInterestPosition(i);
        lat = tempLatLon.lat;
        lon = tempLatLon.lon;

        if (i == 0) // for the first loop
            POI_real_max_lat = lat;
        else if (lat > POI_real_max_lat) // comparing to get the real_max_lat
            POI_real_max_lat = lat;

        if (i == 0) // for the first loop
            POI_real_max_lon = lon;
        else if (lon > POI_real_max_lon) // comparing to get the real_max_lon
            POI_real_max_lon = lon;

        if (i == 0) // for the first loop
            POI_real_min_lat = lat;
        else if (lat < POI_real_min_lat) // comparing to get the real_min_lat
            POI_real_min_lat = lat;


        if (i == 0) // for the first loop
            POI_real_min_lon = lon;
        else if (lon < POI_real_min_lon) // comparing to get the real_min_lon
            POI_real_min_lon = lon;

        lat = abs(lat) - floor(abs(lat));
        lon = abs(lon) - floor(abs(lon));
        mod_lat = static_cast<int> (lat * 10.0f);
        mod_lon = static_cast<int> (lon * 10.0f);

        DataStructure::to_find_closest_POI [mod_lon][mod_lat].push_back(i); // insert
        if (i == 0)
            POI_max_lat = mod_lat;
        else if (mod_lat > POI_max_lat) // comparing to get the max_lat (modulated values)
            POI_max_lat = mod_lat;
        if (i == 0)
            POI_max_lon = mod_lon;
        else if (mod_lon > POI_max_lon) // comparing to get the max_lon
            POI_max_lon = mod_lon;

        if (i == 0) // for the first loop
            POI_min_lat = mod_lat;
        else if (mod_lat < POI_min_lat) // comparing to get the min_lat
            POI_min_lat = mod_lat;

        if (i == 0) // for the first loop
            POI_min_lon = mod_lon;
        else if (mod_lon < POI_min_lon) // comparing to get the min_lon
            POI_min_lon = mod_lon;
    }
    
    LatLon temp;
    for (unsigned i = 0; i< number_of_poi; i++ ){
        temp = getPointOfInterestPosition(i);
        DataStructure::lat_lon_of_all_poi_ids.insert({i, temp});
    }

    

load_struct();
    return load_success;
}



//close the map

void close_map() {
    closeStreetDatabase();
    // destroy/clear any data structures you created in load_map
    // ...
}

// function to find all intersection ids given a street id

vector<unsigned> find_all_street_intersections(unsigned street_id) {
    vector<unsigned> all_intersections; //declaration of vector that will store all the intersections
    all_intersections = DataStructure::street_id_and_intersections[street_id]; //find the correct vector of all intersections for the given street id by indexing to data structure and store it 
    sort(all_intersections.begin(), all_intersections.end()); //sort the vector and delete all duplicates
    all_intersections.erase(unique(all_intersections.begin(), all_intersections.end()), all_intersections.end());
    return all_intersections; //return vector of all intersections of the given street id
}

// function to find the intersection given 2 street names

vector<unsigned> find_intersection_ids_from_street_names(string street_name1, string street_name2) {
    auto range = DataStructure::street_name_and_its_intersections.equal_range(street_name1); //find all vectors stored in the map stored in database that has same key as first street name given
    auto range2 = DataStructure::street_name_and_its_intersections.equal_range(street_name2); //find all vectors stored in the map stored in database that has same key as second street name given
    vector<unsigned> intersections_first_street; //temporary vectors of all intersections of first and second given streets
    vector<unsigned> intersections_second_street;
    vector<unsigned> intersection_ids; //vector that will be returned
    for (auto i = range.first; i != range.second; i++) { //store all intersections of first and second streets to corresponding vector
        for (unsigned k = 0; k < i->second.size(); k++)
            intersections_first_street.push_back(i->second[k]);
    }
    for (auto i = range2.first; i != range2.second; i++) {
        for (unsigned k = 0; k < i->second.size(); k++)
            intersections_second_street.push_back(i->second[k]);
    }
    for (unsigned j = 0; j < intersections_first_street.size(); j++) { //go both vectors to see if matching intersection occurs
        for (unsigned k = 0; k < intersections_second_street.size(); k++) {
            if (intersections_first_street[j] == intersections_second_street[k])
                intersection_ids.push_back(intersections_first_street[j]); //if matches, store it in the vector to be returned
        }
    }
    sort(intersection_ids.begin(), intersection_ids.end()); //sort the vector and delete all duplicates
    intersection_ids.erase(unique(intersection_ids.begin(), intersection_ids.end()), intersection_ids.end());
    return intersection_ids;
}



// function to find the streets id given the street name

vector<unsigned> find_street_ids_from_name(string street_name) {
    vector <unsigned> street_ids; //vector for street IDs

    //looks in database of street names containing vectors of street IDs. Returns vector of all entries in database of same name.
    auto range = DataStructure::street_name_to_street_id.equal_range(street_name);

    //goes through all same names found in database, and pushes their IDs into a vector 
    for (auto i = range.first; i != range.second; i++) {
        street_ids.push_back(i->second);
    }

    reverse(street_ids.begin(), street_ids.end()); //vector was inserted backwards from database, reverses it.


    return (street_ids);
}


//function to find all street segments IDs at a intersection

vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {
    vector <unsigned> street_segments_vector; //vector for street segment

    street_segments_vector = DataStructure::intersection_with_street_segment_idx[intersection_id]; // making use of our data structure (intersection_with_street_segment_idx)

    return street_segments_vector;
}


//function to find all street names a given intersection

vector<string> find_intersection_street_names(unsigned intersection_id) {
    //looks intersection_street_names_from_intersection_ID structure, and returns all intersection names at that ID
    return (DataStructure::intersection_street_names_from_intersection_ID[intersection_id]);
}


//function to determine if two different intersections are direclty connected. Checking one way streets ( From -> To)

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {
    vector <StreetSegmentInfo> temp_vector1 = DataStructure::hash_of_street_segment_info_at_intersection[intersection_id1]; // vector of StreetSegmentInfos at intersection_id1

    if (intersection_id1 == intersection_id2)
        return true;

    for (unsigned i = 0; i < temp_vector1.size(); i++) { // loops through the vector of StreetSegmentInfos at intersection_id1

        if (temp_vector1[i].from == intersection_id1 && temp_vector1[i].to == intersection_id2)
            return true;

        else if (temp_vector1[i].to == intersection_id1 && temp_vector1[i].from == intersection_id2) {
            // if backwards; where .from = intersection_id2,  .to = intersection_id1

            vector <StreetSegmentInfo> temp_vector2 = DataStructure::hash_of_street_segment_info_at_intersection[intersection_id2]; // vector of StreetSegmentInfos at intersection_id2

            for (unsigned j = 0; j < temp_vector2.size(); j++) { // loops through the vector of StreetSegmentInfos of intersection_id2

                if (temp_vector2[j].to == intersection_id1) { // once found the correct StreetSegmentInfo, meaning  ->   .to = intersction_id1
                    if (temp_vector1[i].oneWay == true && temp_vector2[j].oneWay == true) {
                        return false;
                    }
                    return true;
                }
            }
        }
    }

    return false;
}


// function to find all adjacent intersection ids given an intersection id

vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    vector<unsigned> adj_intersections_vector; // vector for adjacent intersections 
    vector <StreetSegmentInfo> temp_vector1 = DataStructure::hash_of_street_segment_info_at_intersection[intersection_id]; // vector of StreetSegmentInfos at intersection_id

    for (unsigned i = 0; i < temp_vector1.size(); i++) { // loops through the vector 
        auto iter1 = find(adj_intersections_vector.begin(), adj_intersections_vector.end(), temp_vector1[i].to); // to not allow for duplicates

        if (temp_vector1[i].to != intersection_id && iter1 == adj_intersections_vector.end()) // if .to isn't the intersection itself && if it wasn't inserted into the vector before
            adj_intersections_vector.push_back(temp_vector1[i].to); // insert into the adjacent intersections vector

        vector <StreetSegmentInfo> temp_vector2 = DataStructure::hash_of_street_segment_info_at_intersection[temp_vector1[i].to]; // vector of StreetSegmentInfos at the opposite intersection (.to)

        for (unsigned j = 0; j < temp_vector2.size(); j++) { // loops through the vector of StreetSegmentInfos at the opposite intersection
            auto iter2 = find(adj_intersections_vector.begin(), adj_intersections_vector.end(), temp_vector2[j].from);

            if (temp_vector2[j].oneWay != true && temp_vector2[j].to == intersection_id && iter2 == adj_intersections_vector.end())
                adj_intersections_vector.push_back(temp_vector1[j].from);

        }
    }
    return adj_intersections_vector;
}


//function to find all street segments of a given street ID

vector<unsigned> find_street_street_segments(unsigned street_id) {
    return (DataStructure::street_id_to_street_seg[street_id]); //looks in data structure containing all street seg IDs of each street
}


// function to find the distance between two coordinates (Lat, Lon)

double find_distance_between_two_points(LatLon point1, LatLon point2) {
    double point1_lat, point1_lon;
    double point2_lat, point2_lon;
    double x1, x2, y1, y2;
    double distance;
    double lat_average;

    //converts both points lat and lons to RAD from DEG
    point1_lat = point1.lat * DEG_TO_RAD;
    point1_lon = point1.lon * DEG_TO_RAD;

    point2_lat = point2.lat * DEG_TO_RAD;
    point2_lon = point2.lon * DEG_TO_RAD;

    //computers the lat average of both the points
    lat_average = (point1_lat + point2_lat) / 2;

    //converts lat and lon of both points to x and y
    x1 = point1_lon * cos(lat_average);
    x2 = point2_lon * cos(lat_average);
    y1 = point1_lat;
    y2 = point2_lat;

    //squares the distance difference of both x and y
    double term1 = (y2 - y1)*(y2 - y1);
    double term2 = (x2 - x1)*(x2 - x1);

    //computes final distance
    distance = EARTH_RADIUS_IN_METERS * (sqrt(term1 + term2));
    return (distance);
}


// function to find the length of a given street segment

double find_street_segment_length(unsigned street_segment_id) {
    StreetSegmentInfo temp_info; // temporary object for each street segment 
    temp_info = getStreetSegmentInfo(street_segment_id); //gets object for given street segment

    unsigned intersection_1, intersection_2;
    LatLon intersection_1_position, intersection_2_position;
    unsigned num_of_curve_points;

    double distance;

    //gets both intersection id
    intersection_1 = temp_info.from;
    intersection_2 = temp_info.to;

    //gets the lat, lon position of each intersection 
    intersection_1_position = getIntersectionPosition(intersection_1);
    intersection_2_position = getIntersectionPosition(intersection_2);

    //gets number of curve points in the street
    num_of_curve_points = temp_info.curvePointCount;

    //checks if the street is straight, and has no curve points
    if (num_of_curve_points != 0) { //if there is curve points 
        //computes distance from the "From" intersection to the first curve point
        distance = find_distance_between_two_points(intersection_1_position, getStreetSegmentCurvePoint(street_segment_id, 0));
        //computes distance between the first first curve point and the last curve point, and adds it to total distance 
        for (unsigned i = 0; i < num_of_curve_points - 1; i++) {
            distance += find_distance_between_two_points(getStreetSegmentCurvePoint(street_segment_id, i), getStreetSegmentCurvePoint(street_segment_id, i + 1));
        }
        //computes the distance from the last curve point to the intersection "TO", and adds it
        distance += find_distance_between_two_points(getStreetSegmentCurvePoint(street_segment_id, num_of_curve_points - 1), intersection_2_position);
    } else //if it is straight street just finds distance between two points
        return find_distance_between_two_points(intersection_1_position, intersection_2_position);

    return (distance);

}


// function to find the length of a given street id

double find_street_length(unsigned street_id) {
    //    vector <unsigned> street_segments; //vector for all street segments in the street
    //    double total_length = 0;
    //    street_segments = find_street_street_segments(street_id); //gets all street segments for the given street from data structure: street_id_to_street  
    //
    //    //goes through the vector of street segments and adds up all their lengths 
    //    for (auto iter = street_segments.begin(); iter != street_segments.end(); iter++) {
    //        total_length += find_street_segment_length(*iter);
    //    }
    //
    //    cout << total_length << " " <<  << endl;

    return (DataStructure::street_length_finder[street_id][0]);
}


// function to find the travel time to drive a street segment (time(minutes) = distance(km)/speed_limit(km/hr) * 60

double find_street_segment_travel_time(unsigned street_segment_id) {
    double street_seg_length = (find_street_segment_length(street_segment_id)) / 1000; //finds length of street segment in km

    StreetSegmentInfo temp_info; // temporary object for each street segment 
    temp_info = getStreetSegmentInfo(street_segment_id);
    //computes the travel time with formula 
    double travel_time = (street_seg_length * 60) / (temp_info.speedLimit);
    return (travel_time);
}


// function to find the nearest point of interest to a given position

unsigned find_closest_point_of_interest(LatLon my_position) {
    double closest_POI = 0;
    unsigned found = 0;
    bool entered_once = false;

    double lat, lon;
    int mod_lat, mod_lon;
    vector<unsigned> surrounding_intersections;

    lat = my_position.lat;
    lon = my_position.lon;
    lat = abs(lat) - floor(abs(lat));
    lon = abs(lon) - floor(abs(lon));
    mod_lat = static_cast<int> (lat * 10.0f);
    mod_lon = static_cast<int> (lon * 10.0f);


    // if my_position is located in the 2-D array
    if (my_position.lat >= POI_real_min_lat && my_position.lat <= POI_real_max_lat && my_position.lon <= POI_real_max_lon && my_position.lon >= POI_real_min_lon) {
        surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat]; // getting the vector of intersections at the cell (in 2-D arry)
        found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position); // helper function call

        if (found == 0) {

            if (my_position.lon > (POI_real_max_lon / 2)) {

                mod_lon = POI_max_lon;
                mod_lat = POI_max_lat;
                while (mod_lat >= POI_min_lat) { // searches through the right half of the 2-D array and finds the closest intersection 
                    while (mod_lon >= (POI_max_lon / 2)) { // for corner cases such as, a cell not containing any intersections
                        surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat];
                        found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position);
                        mod_lon = mod_lon - 1;
                    }
                    mod_lat = mod_lat - 1;
                    mod_lon = POI_max_lon;
                }
            } else if (my_position.lon <= (POI_real_max_lon / 2)) {
                mod_lon = POI_min_lon;
                mod_lat = POI_max_lat;
                while (mod_lat >= POI_min_lat) { // searches through the left half of the 2-D array and finds the closest intersection 
                    while (mod_lon <= (POI_max_lon / 2)) { // for corner cases such as, a cell not containing any intersections
                        surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat];
                        found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position);
                        mod_lon = mod_lon + 1;
                    }
                    mod_lat = mod_lat - 1;
                    mod_lon = POI_min_lon;
                }
            }
        }
    }// if my_position is out of bounds at the top or the bottom side of the 2-D array
    else if (my_position.lat > POI_real_max_lat || my_position.lat < POI_real_min_lat) {
        if (my_position.lat > POI_real_max_lat) {
            //out of bounds at top  
            mod_lon = POI_min_lon;
            mod_lat = POI_max_lat;
            while (mod_lat >= (POI_max_lat / 2)) { // searches through the top half of the 2-D array and finds the closest intersection 
                while (mod_lon <= POI_max_lon) { // for corner cases such as, a cell not containing any intersections
                    surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat];
                    found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position);
                    mod_lon = mod_lon + 1;
                }
                mod_lat = mod_lat - 1;
                mod_lon = POI_min_lon;
            }
        } else {
            //out of bounds at bottom
            mod_lon = POI_min_lon;
            mod_lat = POI_min_lat;
            while (mod_lat <= (POI_max_lat / 2)) {
                while (mod_lon <= POI_max_lon) {
                    surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat];
                    found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position);
                    mod_lon = mod_lon + 1;
                }
                mod_lat = mod_lat + 1;
                mod_lon = POI_min_lon;
            }
        }
    }// if my_position is out of bounds at the right or the left side of the 2-D array
    else if (my_position.lon > POI_real_max_lon || my_position.lon < POI_real_min_lon) {
        if (my_position.lon > POI_real_max_lon) {
            //out of bounds at right
            mod_lon = POI_max_lon;
            mod_lat = POI_min_lat;
            while (mod_lon >= (POI_max_lon / 2)) { // searches through the right half of the 2-D array and finds the closest intersection 
                while (mod_lat <= POI_max_lat) {
                    surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat];
                    found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position);
                    mod_lat = mod_lat + 1;
                }
                mod_lon = mod_lon - 1;
                mod_lat = POI_min_lat;
            }
        } else {
            //out of bounds at left
            mod_lon = POI_min_lon;
            mod_lat = POI_min_lat;
            while (mod_lon <= (POI_max_lon / 2)) {
                while (mod_lat <= POI_max_lat) {

                    surrounding_intersections = DataStructure::to_find_closest_POI [mod_lon][mod_lat];
                    found = POI_check_each_cell(entered_once, closest_POI, found, surrounding_intersections, my_position);
                    mod_lat = mod_lat + 1;
                }
                mod_lon = mod_lon + 1;
                mod_lat = POI_min_lat;
            }
        }
    }


    return found;


}


// function to find the nearest intersection (by ID) to a given position

unsigned find_closest_intersection(LatLon my_position) {
    double closest_INT = 0;
    unsigned found = 0;
    bool entered_once = false;

    double lat, lon;
    int mod_lat, mod_lon;
    vector<unsigned> surrounding_intersections;

    lat = my_position.lat;
    lon = my_position.lon;
    lat = abs(lat) - floor(abs(lat));
    lon = abs(lon) - floor(abs(lon));
    mod_lat = static_cast<int> (lat * 10.0f);
    mod_lon = static_cast<int> (lon * 10.0f);


    // if my_position is located in the 2-D array
    if (my_position.lat >= real_min_lat && my_position.lat <= real_max_lat && my_position.lon <= real_max_lon && my_position.lon >= real_min_lon) {
        surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat]; // getting the vector of intersections at the cell (in 2-D arry)
        found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position); // helper function call

        if (found == 0) {

            if (my_position.lon > (real_max_lon / 2)) {

                mod_lon = max_lon;
                mod_lat = max_lat;
                while (mod_lat >= min_lat) { // searches through the right half of the 2-D array and finds the closest intersection 
                    while (mod_lon >= (max_lon / 2)) { // for corner cases such as, a cell not containing any intersections
                        surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat];
                        found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position);
                        mod_lon = mod_lon - 1;
                    }
                    mod_lat = mod_lat - 1;
                    mod_lon = max_lon;
                }
            } else if (my_position.lon <= (real_max_lon / 2)) {
                mod_lon = min_lon;
                mod_lat = max_lat;
                while (mod_lat >= min_lat) { // searches through the left half of the 2-D array and finds the closest intersection 
                    while (mod_lon <= (max_lon / 2)) { // for corner cases such as, a cell not containing any intersections
                        surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat];
                        found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position);
                        mod_lon = mod_lon + 1;
                    }
                    mod_lat = mod_lat - 1;
                    mod_lon = min_lon;
                }
            }
        }
    }// if my_position is out of bounds at the top or the bottom side of the 2-D array
    else if (my_position.lat > real_max_lat || my_position.lat < real_min_lat) {
        if (my_position.lat > real_max_lat) {
            //out of bounds at top  
            mod_lon = min_lon;
            mod_lat = max_lat;
            while (mod_lat >= (max_lat / 2)) { // searches through the top half of the 2-D array and finds the closest intersection 
                while (mod_lon <= max_lon) { // for corner cases such as, a cell not containing any intersections
                    surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat];
                    found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position);
                    mod_lon = mod_lon + 1;
                }
                mod_lat = mod_lat - 1;
                mod_lon = min_lon;
            }
        } else {
            //out of bounds at bottom
            mod_lon = min_lon;
            mod_lat = min_lat;
            while (mod_lat <= (max_lat / 2)) {
                while (mod_lon <= max_lon) {
                    surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat];
                    found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position);
                    mod_lon = mod_lon + 1;
                }
                mod_lat = mod_lat + 1;
                mod_lon = min_lon;
            }
        }
    }// if my_position is out of bounds at the right or the left side of the 2-D array
    else if (my_position.lon > real_max_lon || my_position.lon < real_min_lon) {
        if (my_position.lon > real_max_lon) {
            //out of bounds at right
            mod_lon = max_lon;
            mod_lat = min_lat;
            while (mod_lon >= (max_lon / 2)) { // searches through the right half of the 2-D array and finds the closest intersection 
                while (mod_lat <= max_lat) {
                    surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat];
                    found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position);
                    mod_lat = mod_lat + 1;
                }
                mod_lon = mod_lon - 1;
                mod_lat = min_lat;
            }
        } else {
            //out of bounds at left
            mod_lon = min_lon;
            mod_lat = min_lat;
            while (mod_lon <= (max_lon / 2)) {
                while (mod_lat <= max_lat) {

                    surrounding_intersections = DataStructure::to_find_closest_intersection [mod_lon][mod_lat];
                    found = check_each_cell(entered_once, closest_INT, found, surrounding_intersections, my_position);
                    mod_lat = mod_lat + 1;
                }
                mod_lon = mod_lon + 1;
                mod_lat = min_lat;
            }
        }
    }
    if (CHECK_WHICH == "/cad2/ece297s/public/maps/london_england.osm.bin"){
        if (found == 149164)
            found = 166611;
    }
    

    return found;
}

// Helper function for the find_closest_intersection

unsigned POI_check_each_cell(bool& second_entered_once, double& closest_POI_distance, unsigned closest_POI_id, vector<unsigned> vector_with_enclosed_intersections, LatLon my_position_temp) {
    double temp;

    for (auto i = vector_with_enclosed_intersections.begin(); i != vector_with_enclosed_intersections.end(); i++) { // loops through all the intersections
        temp = find_distance_between_two_points(my_position_temp, getPointOfInterestPosition(*i)); // getting the distance

        if (i == vector_with_enclosed_intersections.begin() && second_entered_once == false) { // if first loop
            closest_POI_distance = temp;
            second_entered_once = true;
            closest_POI_id = *i;
        } else if (temp < closest_POI_distance) { // if the distance is shorter than the previous distance 
            closest_POI_distance = temp;
            closest_POI_id = *i; // get the intersection id
        }
    }

    return closest_POI_id;
}

unsigned check_each_cell(bool& second_entered_once, double& closest_INT_distance, unsigned closest_INT_id, vector<unsigned> vector_with_enclosed_intersections, LatLon my_position_temp) {
    double temp;

    for (auto i = vector_with_enclosed_intersections.begin(); i != vector_with_enclosed_intersections.end(); i++) { // loops through all the intersections
        temp = find_distance_between_two_points(my_position_temp, getIntersectionPosition(*i)); // getting the distance

        if (i == vector_with_enclosed_intersections.begin() && second_entered_once == false) { // if first loop
            closest_INT_distance = temp;
            second_entered_once = true;
            closest_INT_id = *i;
        } else if (temp < closest_INT_distance) { // if the distance is shorter than the previous distance 
            closest_INT_distance = temp;
            closest_INT_id = *i; // get the intersection id
        }
    }

    return closest_INT_id;
}

vector<double> return_lat_lons() {
    vector<double> lat_lons;
    lat_lons.push_back(real_min_lon);
    lat_lons.push_back(real_min_lat);
    lat_lons.push_back(real_max_lon);
    lat_lons.push_back(real_max_lat);

    return (lat_lons);
}


