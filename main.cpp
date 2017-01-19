#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "iostream"
#include <string>
#include <vector>
#include <algorithm>
#include "OSMDatabaseAPI.h"
#include "m2.h"
#include "data_structure.h"
#include "m3.h"


#include "milestone_3_graph.h"
#include "m3.h"


using namespace std;

int main() {

    string temp;
    cout << "Select which map to load: ";
    getline(cin, temp);

    if (temp == "Toronto") {
        load_map("/cad2/ece297s/public/maps/toronto.streets.bin");
    } else if (temp == "NewYork") {
        load_map("/cad2/ece297s/public/maps/newyork.streets.bin");
    } else if (temp == "Moscow") {
        load_map("/cad2/ece297s/public/maps/moscow.streets.bin");
    } else if (temp == "Cairo") {
        load_map("/cad2/ece297s/public/maps/cairo_egypt.streets.bin");
    } else if (temp == "Saint") {
        load_map("/cad2/ece297s/public/maps/saint_helena.streets.bin");
    }
    else if (temp == "London") {
        load_map("/cad2/ece297s/public/maps/london_england.streets.bin");
    }

    Graph graph;
    //draw_map();
    //find_path_between_intersections(17930, 93400);
    

    
//    
//          vector <unsigned> moo = find_path_between_intersections(24341, 84950);
////        
//////         
//       for (auto iter = moo.begin(); iter != moo.end(); iter++){
//           StreetSegmentInfo tempInfo = getStreetSegmentInfo (*iter);
//           cout << *iter << endl;
//           //cout << "*From: " << tempInfo.from << " To:  " << tempInfo.to << "  " << *iter << " " << endl;
//       }
//    
    
    
    
//    vector <unsigned> adj_intersections = find_adjacent_intersections(2427);
//    StreetSegmentInfo tempInfo;
//    for (auto i = adj_intersections.begin(); i != adj_intersections.end(); i++){
//        unsigned street_seg_id = DataStructure::intersection_to_street_segment[2427][*i];
//        tempInfo = getStreetSegmentInfo(street_seg_id);
//        cout << tempInfo.oneWay << endl;
//        cout << tempInfo.to << endl;
//        cout << tempInfo.from << endl;
//    }
    
    
    
    
    
//vector <unsigned> moo = find_path_between_intersections(2427,1847);
        draw_map();


    //    find_path_between_intersections(97837, 105444);
    //    for (auto i = DataStructure::shortest_path.begin(); i != DataStructure::shortest_path.end(); i++) {
    //        StreetSegmentInfo info = getStreetSegmentInfo(*i);
    //        cout << "*From: " << info.from << " To:  " << info.to << "  " << *i << " " << endl;
    //    }




//    for (auto i = foo.begin(); i != foo.end(); i++){
//        cout << " Data structure : " << (*i).to << endl;
//    }
//    cout << "\n" ;
//    
//    Graph graph;
//    vector <unsigned> moo = find_adjacent_intersections(47924);
//    for (auto i = moo.begin(); i!= moo.end(); i++) {
//        cout << " m1 : " << *i << endl;
//        
////        cout << " Data : " << DataStructure::intersection_to_street_segment[47924][(*i)] << endl;
//    }
//    
//    cout << "\n";
//    vector <Edge> boo = graph.hash_of_vertices[47924].adjacent_edges;
//    for (auto i = boo.begin(); i != boo.end(); i++){
//        bool one_way = (*i).oneway;
//        if (one_way == true){
//            unsigned mo = (*i).to;
//        
//            cout << " graph : " << mo << endl;
//        }
//        else {
//            
//            unsigned mo = (*i).from;
//            
//            cout << " graph : " << mo << endl;
//        }
//
//    }
    
    
    //    
    //    unordered_map <unsigned, Vertex> hash_vertex = graph.get_hash_of_vertices();
    //    Vertex temp_vertex = hash_vertex[1000];
    //    vector<Edge> name = temp_vertex.get_adj_edges();

    //    for (auto iter = name.begin(); iter != name.end(); iter++){
    //        unsigned id_of_adj = (*iter).get_to();
    ////        StreetSegmentInfo temp_info = getStreetSegmentInfo(id_of_edge);
    ////        unsigned street_id = temp_info.streetID;
    ////        
    ////        string name_of = getStreetName(street_id);

    //    unordered_map <unsigned, Vertex> hash_vertex = graph.get_hash_of_vertices();
    //    vector <Edge> adj = hash_vertex[1048].get_adj_edges();
    //    
    //    for (auto i = adj.begin(); i != adj.end(); i++){
    //        Edge mooooo = *i;
    //        unsigned mo = mooooo.get_street_seg_id();

    //        cout << " " << id_of_adj << " " << endl;
    //        
    //    } 12566, 41447           
    //    
    //    
    //    cout << "\n" << endl;
    //    



    //        cout << " "  << mo << " " << endl;
    //    }


    //    


    //    for(unsigned i = 0; i < getNumberOfStreetSegments(); i++){
    //        cout << i << endl;
    //    }

    //    Graph graph;
    //    
    //    unordered_map <unsigned, Vertex> hash_vertex = graph.get_hash_of_vertices();
    //    Vertex temp_vertex = hash_vertex[1000];
    //    vector<Edge> name = temp_vertex.get_adj_edges();
    //    
    //    for (auto iter = name.begin(); iter != name.end(); iter++){
    //      
    //        unsigned mo = (*iter).get_to();
    //
    ////        StreetSegmentInfo temp_info = getStreetSegmentInfo(id_of_edge);
    ////        unsigned street_id = temp_info.streetID;
    ////        
    ////        string name_of = getStreetName(street_id);
    //        
    //        cout << " " << mo << " " << endl;
    //        
    //    }
    //    
    //    
    //    cout << "\n" << endl;
    //    
    //    vector <unsigned> mool = find_adjacent_intersections(1000);
    //    
    //    for (auto i = mool.begin(); i != mool.end(); i++){
    //        
    //        cout << "* " << *i << " " << endl;
    //    }


    // cout << tempInfo.streetID;
    //    




    closeOSMDatabase();

    close_map();

    return 0;
}
