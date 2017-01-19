#pragma once
#include <string>
#include "StreetsDatabaseAPI.h"
#include <iomanip>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include "graphics.h"
#include "m3.h"
#include "easygl_constants.h"

// Draws the map. You can assume your load_map (string map_name)
// function is called before this function in the unit tests.
// Your main () program should do the same.

void load_struct();  
void draw_map();
void draw_screen ();
void draw_streets ();
void draw_street_seg_with_curve_point(LatLon position_1, LatLon position_2, StreetSegmentInfo info);
void act_on_new_button_func (void (*drawscreen_ptr) (void));
void draw_intersection_highlight ();
void draw_features();
void print_out_street_names();
void act_on_find_button_func(void (*drawscreen_ptr) (void));
void draw_found_highlighted();
void print_out_POI_names();
void act_on_button_press (float x, float y, t_event_buttonPressed event);
void draw_train_stns();
void draw_subway_tracks();
void act_on_clear_button_func (void (*drawscreen_ptr) (void)); 
void act_on_display_POI_button_func(void (*drawscreen_ptr) (void));
void act_on_display_public_transit_func(void (*drawscreen_ptr) (void));
void act_on_help_button_func (void (*drawscreen_ptr) (void));
void draw_POI_highlight();
void draw_bus_routes();
void draw_help_screen();
void draw_search_bar();
void draw_search_text();
void draw_searched_text();
void draw_search_bar_POI();
void draw_search_text_POI();
void draw_searched_text_POI();
void oneway();
int find_pixels_between_two_points(LatLon point1, LatLon point2);
void draw_shortest_edges_with_curve_point(LatLon position_1, LatLon position_2, StreetSegmentInfo info);
void draw_shortests ();
void act_on_key_press (char c, int keysym);
void draw_shortest_edges_with_curve_point2(LatLon position_1, LatLon position_2, StreetSegmentInfo info);
void act_on_find_path_button_func(void (*drawscreen_ptr) (void));
void act_on_find_inter_func(void (*drawscreen_ptr) (void));
void act_on_find_POI_func(void (*drawscreen_ptr) (void));
void display_directions ();
void act_on_find_path_POI_button_func(void (*drawscreen_ptr) (void));