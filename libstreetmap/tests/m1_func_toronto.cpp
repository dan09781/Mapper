#include <algorithm>
#include <unittest++/UnitTest++.h>
#include "m1.h"
#include "unit_test_util.h"
#include "StreetsDatabaseAPI.h"

using ece297test::relative_error;

#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;

struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(0);
        rand_intersection = std::uniform_int_distribution<unsigned>(0, getNumberOfIntersections()-1);
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
        rand_segment = std::uniform_int_distribution<unsigned>(0, getNumberOfStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<unsigned>(0, getNumberOfPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<float>(43.48, 43.91998);
        rand_lon = std::uniform_real_distribution<float>(-79.78998, -79.00001);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_intersection;
    std::uniform_int_distribution<unsigned> rand_street;
    std::uniform_int_distribution<unsigned> rand_segment;
    std::uniform_int_distribution<unsigned> rand_poi;
    std::uniform_real_distribution<float> rand_lat;
    std::uniform_real_distribution<float> rand_lon;
};

SUITE(inter_inter_path_func_public) {
    TEST(find_path_between_intersections_legality) {
        std::vector<unsigned> path;
        
        path = find_path_between_intersections(2, 9256);
        CHECK(path_is_legal(2, 9256, path));
        
        path = find_path_between_intersections(17930, 93400);
        CHECK(path_is_legal(17930, 93400, path));
        
        path = find_path_between_intersections(24341, 84950);
        CHECK(path_is_legal(24341, 84950, path));
        
        path = find_path_between_intersections(28619, 80938);
        CHECK(path_is_legal(28619, 80938, path));
        
        path = find_path_between_intersections(32342, 46379);
        CHECK(path_is_legal(32342, 46379, path));
        
        path = find_path_between_intersections(33578, 56051);
        CHECK(path_is_legal(33578, 56051, path));
        
        path = find_path_between_intersections(43046, 85976);
        CHECK(path_is_legal(43046, 85976, path));
        
        path = find_path_between_intersections(47772, 44848);
        CHECK(path_is_legal(47772, 44848, path));
        
        path = find_path_between_intersections(53600, 84132);
        CHECK(path_is_legal(53600, 84132, path));
        
        path = find_path_between_intersections(56060, 43327);
        CHECK(path_is_legal(56060, 43327, path));
        
        path = find_path_between_intersections(62818, 17844);
        CHECK(path_is_legal(62818, 17844, path));
        
        path = find_path_between_intersections(63381, 88129);
        CHECK(path_is_legal(63381, 88129, path));
        
        path = find_path_between_intersections(64436, 55705);
        CHECK(path_is_legal(64436, 55705, path));
        
        path = find_path_between_intersections(65267, 91978);
        CHECK(path_is_legal(65267, 91978, path));
        
        path = find_path_between_intersections(65463, 97060);
        CHECK(path_is_legal(65463, 97060, path));
        
        path = find_path_between_intersections(66348, 47055);
        CHECK(path_is_legal(66348, 47055, path));
        
        path = find_path_between_intersections(67133, 63045);
        CHECK(path_is_legal(67133, 63045, path));
        
        path = find_path_between_intersections(79055, 105225);
        CHECK(path_is_legal(79055, 105225, path));
        
        path = find_path_between_intersections(80481, 34196);
        CHECK(path_is_legal(80481, 34196, path));
        
        path = find_path_between_intersections(64489, 10192);
        CHECK(path_is_legal(64489, 10192, path));
        
        path = find_path_between_intersections(95430, 108325);
        CHECK(path_is_legal(95430, 108325, path));
        
        path = find_path_between_intersections(9140, 19741);
        CHECK(path_is_legal(9140, 19741, path));
        
        path = find_path_between_intersections(97919, 71085);
        CHECK(path_is_legal(97919, 71085, path));
        
        path = find_path_between_intersections(98141, 104672);
        CHECK(path_is_legal(98141, 104672, path));
        
        path = find_path_between_intersections(98719, 32007);
        CHECK(path_is_legal(98719, 32007, path));
        
        path = find_path_between_intersections(98941, 108022);
        CHECK(path_is_legal(98941, 108022, path));
        
        path = find_path_between_intersections(9748, 61004);
        CHECK(path_is_legal(9748, 61004, path));
        
        path = find_path_between_intersections(101919, 45138);
        CHECK(path_is_legal(101919, 45138, path));
        
        path = find_path_between_intersections(105371, 20649);
        CHECK(path_is_legal(105371, 20649, path));
        
        path = find_path_between_intersections(7801, 20885);
        CHECK(path_is_legal(7801, 20885, path));
        
    } //find_path_between_intersections_legality

    TEST(find_path_between_intersections_optimality) {
        std::vector<unsigned> path;
        path = find_path_between_intersections(2, 9256);
        CHECK(path_is_legal(2, 9256, path));
        CHECK(compute_path_travel_time(path) <= 16.97107155614322);
        
        path = find_path_between_intersections(17930, 93400);
        CHECK(path_is_legal(17930, 93400, path));
        CHECK(compute_path_travel_time(path) <= 18.60141603252727);
        
        path = find_path_between_intersections(24341, 84950);
        CHECK(path_is_legal(24341, 84950, path));
        CHECK(compute_path_travel_time(path) <= 14.42634934251768);
        
        path = find_path_between_intersections(28619, 80938);
        CHECK(path_is_legal(28619, 80938, path));
        CHECK(compute_path_travel_time(path) <= 34.93224887390062);
        
        path = find_path_between_intersections(32342, 46379);
        CHECK(path_is_legal(32342, 46379, path));
        CHECK(compute_path_travel_time(path) <= 44.34490195154542);
        
        path = find_path_between_intersections(33578, 56051);
        CHECK(path_is_legal(33578, 56051, path));
        CHECK(compute_path_travel_time(path) <= 38.46870425901721);
        
        path = find_path_between_intersections(43046, 85976);
        CHECK(path_is_legal(43046, 85976, path));
        CHECK(compute_path_travel_time(path) <= 42.34206636523066);
        
        path = find_path_between_intersections(47772, 44848);
        CHECK(path_is_legal(47772, 44848, path));
        CHECK(compute_path_travel_time(path) <= 27.52103760108951);
        
        path = find_path_between_intersections(53600, 84132);
        CHECK(path_is_legal(53600, 84132, path));
        CHECK(compute_path_travel_time(path) <= 21.86841541006605);
        
        path = find_path_between_intersections(56060, 43327);
        CHECK(path_is_legal(56060, 43327, path));
        CHECK(compute_path_travel_time(path) <= 45.23658886267624);
        
        path = find_path_between_intersections(62818, 17844);
        CHECK(path_is_legal(62818, 17844, path));
        CHECK(compute_path_travel_time(path) <= 5.975310074617227);
        
        path = find_path_between_intersections(63381, 88129);
        CHECK(path_is_legal(63381, 88129, path));
        CHECK(compute_path_travel_time(path) <= 20.03380477663629);
        
        path = find_path_between_intersections(64436, 55705);
        CHECK(path_is_legal(64436, 55705, path));
        CHECK(compute_path_travel_time(path) <= 28.14439495235417);
        
        path = find_path_between_intersections(65267, 91978);
        CHECK(path_is_legal(65267, 91978, path));
        CHECK(compute_path_travel_time(path) <= 18.53861473816822);
        
        path = find_path_between_intersections(65463, 97060);
        CHECK(path_is_legal(65463, 97060, path));
        CHECK(compute_path_travel_time(path) <= 36.23090190715419);
        
        path = find_path_between_intersections(66348, 47055);
        CHECK(path_is_legal(66348, 47055, path));
        CHECK(compute_path_travel_time(path) <= 36.00071271261419);
        
        path = find_path_between_intersections(67133, 63045);
        CHECK(path_is_legal(67133, 63045, path));
        CHECK(compute_path_travel_time(path) <= 48.89588246836037);
        
        path = find_path_between_intersections(79055, 105225);
        CHECK(path_is_legal(79055, 105225, path));
        CHECK(compute_path_travel_time(path) <= 32.61748952451359);
        
        path = find_path_between_intersections(80481, 34196);
        CHECK(path_is_legal(80481, 34196, path));
        CHECK(compute_path_travel_time(path) <= 31.22172067427089);
        
        path = find_path_between_intersections(64489, 10192);
        CHECK(path_is_legal(64489, 10192, path));
        CHECK(compute_path_travel_time(path) <= 44.78823700347708);
        
        path = find_path_between_intersections(95430, 108325);
        CHECK(path_is_legal(95430, 108325, path));
        CHECK(compute_path_travel_time(path) <= 18.90825258302869);
        
        path = find_path_between_intersections(9140, 19741);
        CHECK(path_is_legal(9140, 19741, path));
        CHECK(compute_path_travel_time(path) <= 8.533098894535689);
        
        path = find_path_between_intersections(97919, 71085);
        CHECK(path_is_legal(97919, 71085, path));
        CHECK(compute_path_travel_time(path) <= 19.09424675995584);
        
        path = find_path_between_intersections(98141, 104672);
        CHECK(path_is_legal(98141, 104672, path));
        CHECK(compute_path_travel_time(path) <= 8.374395601146057);
        
        path = find_path_between_intersections(98719, 32007);
        CHECK(path_is_legal(98719, 32007, path));
        CHECK(compute_path_travel_time(path) <= 16.83079257668522);
        
        path = find_path_between_intersections(98941, 108022);
        CHECK(path_is_legal(98941, 108022, path));
        CHECK(compute_path_travel_time(path) <= 25.30575757016142);
        
        path = find_path_between_intersections(9748, 61004);
        CHECK(path_is_legal(9748, 61004, path));
        CHECK(compute_path_travel_time(path) <= 20.01393594386155);
        
        path = find_path_between_intersections(101919, 45138);
        CHECK(path_is_legal(101919, 45138, path));
        CHECK(compute_path_travel_time(path) <= 33.28684118995628);
        
        path = find_path_between_intersections(105371, 20649);
        CHECK(path_is_legal(105371, 20649, path));
        CHECK(compute_path_travel_time(path) <= 11.25282465218332);
        
        path = find_path_between_intersections(7801, 20885);
        CHECK(path_is_legal(7801, 20885, path));
        CHECK(compute_path_travel_time(path) <= 36.53998218118426);
        
    } //find_path_between_intersections_optimality

} //inter_inter_path_func_public



//SUITE(corner_cases) {
//
//    TEST(find_closest_intersection) {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//        CHECK_EQUAL(61143, find_closest_intersection(LatLon(90.00000, 0.0000))); // north pole 
//        CHECK_EQUAL(29770, find_closest_intersection(LatLon(-90.00000, 0.0000))); // south pole
//        CHECK_EQUAL(10088, find_closest_intersection(LatLon(43.9878, -79.08996))); // top left
//        CHECK_EQUAL(41332, find_closest_intersection(LatLon(43.9398, -79.97652))); // top right
//        CHECK_EQUAL(95206, find_closest_intersection(LatLon(43.4878, -79.08996)));  // bottom left
//        CHECK_EQUAL(9124, find_closest_intersection(LatLon(43.4398, -79.97652))); // bottom right
//
//    }
//}
//
//SUITE(street_queries) {
//    //Unit Test written for first function. Checks many different streets. Checks corner case of no input for street name. 
//
//    TEST(street_ids_from_name) {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//        expected = {1200};
//        actual = find_street_ids_from_name("Altamont Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {6269, 20402};
//        actual = find_street_ids_from_name("Highway 403 - Hamilton");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {303, 309, 508, 510, 516, 519, 2406, 2408, 2413, 4713, 4716, 19884, 19900, 20770};
//        actual = find_street_ids_from_name("Ramp to Highway 404 Southbound");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {20594, 20595};
//        actual = find_street_ids_from_name("Berkwood Hollow Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {20478, 20479, 20480, 20481, 20482, 20483, 20484, 20485};
//        actual = find_street_ids_from_name("Kiss 'n' Ride");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {493, 494, 890, 2616, 2618, 2619, 8668, 8671, 8673, 17128, 17129, 20009, 20013, 20269, 20440};
//        actual = find_street_ids_from_name("Ramp to Highway 400 Southbound");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {19659, 19660, 20140, 20374};
//        actual = find_street_ids_from_name("Main Street Markham South");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {19944, 20358};
//        actual = find_street_ids_from_name("Gilbert Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {3531, 20350};
//        actual = find_street_ids_from_name("Ramp to Keele Street");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {20339};
//        actual = find_street_ids_from_name("Bank of Montreal drive thru");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {3031, 3032, 20311};
//        actual = find_street_ids_from_name("Constance Street");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {20292};
//        actual = find_street_ids_from_name("to Tim Horton's, Wendys, Mr. Sub, Nicholby's & parking");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {19548, 20197};
//        actual = find_street_ids_from_name("Lloyd Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_street_ids_from_name(" ");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_street_ids_from_name("NoStreetNameRandom");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//    }
//
//    TEST(street_street_segments) {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//        expected = {0, 1, 9223, 9224, 9225, 15713, 15725, 37857, 120389, 149635};
//        actual = find_street_street_segments(1);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {6817, 6818, 6819, 6820, 6821, 6822, 6823};
//        actual = find_street_street_segments(1769);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {7126};
//        actual = find_street_street_segments(1863);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {13688, 13689};
//        actual = find_street_street_segments(3427);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {15627};
//        actual = find_street_street_segments(3947);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {21273, 21274, 21275, 21276};
//        actual = find_street_street_segments(5470);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {24508};
//        actual = find_street_street_segments(6117);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {24780, 24781, 24782};
//        actual = find_street_street_segments(6181);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {42679, 42809};
//        actual = find_street_street_segments(8280);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {46711, 72149, 72156};
//        actual = find_street_street_segments(8864);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {52574};
//        actual = find_street_street_segments(10646);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {52809, 52810};
//        actual = find_street_street_segments(10714);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {56446};
//        actual = find_street_street_segments(11658);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {58977, 58995};
//        actual = find_street_street_segments(12113);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {59756, 59759};
//        actual = find_street_street_segments(12314);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {60614, 60702, 60714, 60735, 60747, 61166, 61179, 61183, 61197, 61249, 61283, 61311};
//        actual = find_street_street_segments(12511);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {65493};
//        actual = find_street_street_segments(13585);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {72854};
//        actual = find_street_street_segments(15108);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {74515, 74521};
//        actual = find_street_street_segments(15468);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {80227, 80228, 80229, 80230};
//        actual = find_street_street_segments(16842);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {90467};
//        actual = find_street_street_segments(17849);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {91518};
//        actual = find_street_street_segments(18237);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {94170};
//        actual = find_street_street_segments(18549);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {98095, 98096, 98097, 98098, 98099, 98104, 98105, 98108, 98109, 98110, 98111, 106836, 106855, 106856, 106857, 106858, 106859, 106860, 106861, 106862, 106863, 106864, 116469, 130779};
//        actual = find_street_street_segments(18713);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {98503};
//        actual = find_street_street_segments(18755);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {99038, 99039, 99040, 99041, 124029, 124030, 124031, 124032, 124033};
//        actual = find_street_street_segments(18866);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {116705, 116706, 116707};
//        actual = find_street_street_segments(20003);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {118779};
//        actual = find_street_street_segments(20109);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {119380, 119381, 119382, 119383, 119384, 119385, 119386};
//        actual = find_street_street_segments(20137);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {150820, 150821, 150822, 150823, 150824};
//        actual = find_street_street_segments(20701);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//    } //street_street_segments  
//
//    TEST(all_street_intersections) {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//        expected = {0, 1, 2, 168, 5950, 10046, 10047, 10048, 10049, 16470, 37282};
//        actual = find_all_street_intersections(1);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {2345, 4117, 7622, 7623, 7624, 7625, 7626, 7627};
//        actual = find_all_street_intersections(1769);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {7955, 7957};
//        actual = find_all_street_intersections(1863);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {12257, 12435, 14481};
//        actual = find_all_street_intersections(3427);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {16362, 16366};
//        actual = find_all_street_intersections(3947);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {21962, 22052, 22053, 22054, 22055};
//        actual = find_all_street_intersections(5470);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {25159, 25160};
//        actual = find_all_street_intersections(6117);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {24421, 25350, 25373, 25376};
//        actual = find_all_street_intersections(6181);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {41384, 41487, 41488};
//        actual = find_all_street_intersections(8280);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {44481, 62774, 62777};
//        actual = find_all_street_intersections(8864);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {49600, 49625};
//        actual = find_all_street_intersections(10646);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {49834, 49835, 49836};
//        actual = find_all_street_intersections(10714);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {52318, 52368};
//        actual = find_all_street_intersections(11658);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {3597, 54003, 54017};
//        actual = find_all_street_intersections(12113);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {54486, 54488, 54493};
//        actual = find_all_street_intersections(12314);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {55073, 55074, 55099, 55120, 55163, 55170, 55463, 55466, 55487, 55489, 55500, 55511, 55512};
//        actual = find_all_street_intersections(12511);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {58447, 58448};
//        actual = find_all_street_intersections(13585);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {63023, 63024};
//        actual = find_all_street_intersections(15108);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {24972, 64411, 64415};
//        actual = find_all_street_intersections(15468);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {40017, 40021, 40031, 40032, 68216};
//        actual = find_all_street_intersections(16842);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {74377, 74379};
//        actual = find_all_street_intersections(17849);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {75075, 75078};
//        actual = find_all_street_intersections(18237);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {76853, 76854};
//        actual = find_all_street_intersections(18549);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {32511, 32704, 32711, 33607, 33627, 33628, 33641, 36320, 37884, 40263, 72477, 72478, 79163, 79164, 79165, 79167, 79170, 79171, 79172, 79235, 84471, 84472, 84473, 84474, 96991};
//        actual = find_all_street_intersections(18713);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {45231, 79403};
//        actual = find_all_street_intersections(18755);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {79644, 79670, 79725, 79754, 79755, 79756, 93611, 93612, 93613, 93614};
//        actual = find_all_street_intersections(18866);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {1254, 2149, 42519, 42523};
//        actual = find_all_street_intersections(20003);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {90644, 90660};
//        actual = find_all_street_intersections(20109);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {44515, 91005, 91006, 91007, 91008, 91009, 91010, 91011};
//        actual = find_all_street_intersections(20137);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {82323, 82325, 105605, 105606, 105611, 105613};
//        actual = find_all_street_intersections(20701);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//
//    }
//
//    //all_street_intersections
//
//    TEST(intersection_ids_from_street_names) {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//        expected = {63023, 63024};
//        actual = find_intersection_ids_from_street_names("Anderson Crescent", "Farmstead Lane");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Birchlea Avenue", "Donnamora Crescent");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Bonnacord Drive", "Bradstock Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {76854};
//        actual = find_intersection_ids_from_street_names("Brickstone Mews", "Curran Place");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Denton Avenue", "Squire Drive");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {49625};
//        actual = find_intersection_ids_from_street_names("Dumfrey Road", "Kenmark Boulevard");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {54017};
//        actual = find_intersection_ids_from_street_names("Dunera Avenue", "Payzac Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {24972};
//        actual = find_intersection_ids_from_street_names("Father D'souza Drive", "Mavis Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {0};
//        actual = find_intersection_ids_from_street_names("Highway 401 Eastbound Collectors", "Ramp to Highway 404 / Don Valley Parkway");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Howard Street", "Tillings Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {49834, 49835};
//        actual = find_intersection_ids_from_street_names("Kidbrooke Crescent", "Montvale Drive");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {49834, 49835};
//        actual = find_intersection_ids_from_street_names("Montvale Drive", "Kidbrooke Crescent");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Naomi Crescent", "Bridgman Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {74379};
//        actual = find_intersection_ids_from_street_names("National Pine Drive", "Deepsprings Crescent");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Parkham Crescent", "Camor Court");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Pendulum Circle", "Satchell Boulevard");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Rambert Crescent", "Hartwell Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Ramp to Highway 407 E.T.R. Westbound", "Kingsdale Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {62777};
//        actual = find_intersection_ids_from_street_names("Reith Way", "Porterfield Crescent");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {27621};
//        actual = find_intersection_ids_from_street_names("Saffron Crescent", "Rymer Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Seminoff Street", "Terrydale Drive");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {8311};
//        actual = find_intersection_ids_from_street_names("Shaftesbury Street", "Acton Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("South Parade Court", "Gorton Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {60625, 60626};
//        actual = find_intersection_ids_from_street_names("Stargell Crescent", "Raymerville Drive");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {79369, 79403};
//        actual = find_intersection_ids_from_street_names("Strong Gate", "O'Shea Crescent");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Tennyson Avenue", "Queensborough Crescent");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("To [107] Queen Street west", "Eagleview Drive");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Tomel Court", "Jim Baird Mews");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {77972};
//        actual = find_intersection_ids_from_street_names("Trentin Road", "Thorndale Drive");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_intersection_ids_from_street_names("Zaph Avenue", "Jenvic Gardens");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//    } //intersection_ids_from_street_names
//
//} //street_queries
//
//SUITE(intersection_queries) {
//
//    TEST(intersection_street_segments) {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//        expected = {1, 15725};
//        actual = find_intersection_street_segments(2);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {8195, 8196, 105698};
//        actual = find_intersection_street_segments(9076);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {8676, 8677, 8718};
//        actual = find_intersection_street_segments(9558);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {16792, 16793, 16795, 16796};
//        actual = find_intersection_street_segments(17582);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {19508, 35040, 143315};
//        actual = find_intersection_street_segments(20248);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {27532, 134560, 134561};
//        actual = find_intersection_street_segments(28063);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {31085, 31086};
//        actual = find_intersection_street_segments(31385);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {31452, 31453, 38549};
//        actual = find_intersection_street_segments(31714);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {44059, 44060, 88825};
//        actual = find_intersection_street_segments(42485);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {47293, 49640, 51597};
//        actual = find_intersection_street_segments(45478);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {59967, 59968};
//        actual = find_intersection_street_segments(54622);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {60482, 60485, 60489};
//        actual = find_intersection_street_segments(54970);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {67577, 67578};
//        actual = find_intersection_street_segments(59818);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {71131, 71158, 71433, 71445};
//        actual = find_intersection_street_segments(62149);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {72544, 72545};
//        actual = find_intersection_street_segments(63184);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {74160, 74161, 87821};
//        actual = find_intersection_street_segments(64191);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {83051, 83052, 136896};
//        actual = find_intersection_street_segments(69703);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {95200, 115294, 134528};
//        actual = find_intersection_street_segments(77519);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {98469, 98480, 98481};
//        actual = find_intersection_street_segments(79365);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {110747, 120105, 120108};
//        actual = find_intersection_street_segments(86416);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {120326, 120327, 120327};
//        actual = find_intersection_street_segments(91585);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {123972, 123973, 124020};
//        actual = find_intersection_street_segments(93575);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {127139, 127140};
//        actual = find_intersection_street_segments(95174);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {128744, 128745};
//        actual = find_intersection_street_segments(96016);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {129108, 129110};
//        actual = find_intersection_street_segments(96233);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {130314, 130315};
//        actual = find_intersection_street_segments(96800);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {143084, 143085, 143105};
//        actual = find_intersection_street_segments(102638);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {144159, 144160, 144162};
//        actual = find_intersection_street_segments(103180);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {144478, 144479, 144488};
//        actual = find_intersection_street_segments(103323);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {152412, 152469, 152470};
//        actual = find_intersection_street_segments(106219);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//    } //intersection_street_segments
//
//    TEST(intersection_street_names) {
//        std::vector<std::string> expected;
//        std::vector<std::string> actual;
//
//        expected = {"Highway 401 Eastbound Collectors", "Highway 401 Eastbound Collectors"};
//        actual = find_intersection_street_names(2);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "Delisle Avenue", "Delisle Avenue"};
//        actual = find_intersection_street_names(9076);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(9558);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(17582);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Highway 7", "Highway 7", "Highway 7 off ramp"};
//        actual = find_intersection_street_names(20248);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "Queen Street West", "Queen Street West"};
//        actual = find_intersection_street_names(28063);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(31385);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(31714);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Thomson Creek Boulevard", "Vaughan Mills Road", "Vaughan Mills Road"};
//        actual = find_intersection_street_names(42485);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Beatty Road", "Beatty Road", "Elgin Street"};
//        actual = find_intersection_street_names(45478);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Highway 401 Collectors", "Highway 401 Collectors"};
//        actual = find_intersection_street_names(54622);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Sheppard Avenue East", "Sheppard Avenue East", "Sheppard Avenue East"};
//        actual = find_intersection_street_names(54970);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Eglinton Avenue East", "Eglinton Avenue East"};
//        actual = find_intersection_street_names(59818);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Ridgecrest Road", "Ridgecrest Road", "Westchester Crescent", "Westchester Crescent"};
//        actual = find_intersection_street_names(62149);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Meadowvale Town Centre Circle", "Meadowvale Town Centre Circle"};
//        actual = find_intersection_street_names(63184);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Deepwood Heights", "Deepwood Heights", "Heathcote Walk"};
//        actual = find_intersection_street_names(64191);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "Spring Garden Avenue", "Spring Garden Avenue"};
//        actual = find_intersection_street_names(69703);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Gordon Baker Road", "Gordon Baker Road", "Harold Evans Crescent"};
//        actual = find_intersection_street_names(77519);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Montebello Crescent", "Montebello Crescent", "Montebello Crescent"};
//        actual = find_intersection_street_names(79365);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "York Mills Road", "York Mills Road"};
//        actual = find_intersection_street_names(86416);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(91585);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Formosa Lane", "Formosa Lane", "Pendrill Way"};
//        actual = find_intersection_street_names(93575);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(95174);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(96016);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"Bayview Avenue", "Bayview Avenue"};
//        actual = find_intersection_street_names(96233);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(96800);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(102638);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(103180);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(103323);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {"<unknown>", "<unknown>", "<unknown>"};
//        actual = find_intersection_street_names(106219);
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//    } //intersection_street_names
//}
//
//TEST(directly_connected) {
//    CHECK_EQUAL(1, are_directly_connected(2, 2));
//
//    CHECK_EQUAL(0, are_directly_connected(7649, 20479));
//
//    CHECK_EQUAL(0, are_directly_connected(8963, 19357));
//
//    CHECK_EQUAL(1, are_directly_connected(9076, 9076));
//
//    CHECK_EQUAL(1, are_directly_connected(9558, 9559));
//
//    CHECK_EQUAL(1, are_directly_connected(17582, 17585));
//
//    CHECK_EQUAL(0, are_directly_connected(23868, 83299));
//
//    CHECK_EQUAL(1, are_directly_connected(28063, 28062));
//
//    CHECK_EQUAL(1, are_directly_connected(31714, 31715));
//
//    CHECK_EQUAL(0, are_directly_connected(32926, 54961));
//
//    CHECK_EQUAL(0, are_directly_connected(42209, 84305));
//
//    CHECK_EQUAL(0, are_directly_connected(46844, 43977));
//
//    CHECK_EQUAL(0, are_directly_connected(52559, 82497));
//
//    CHECK_EQUAL(1, are_directly_connected(54970, 18442));
//
//    CHECK_EQUAL(1, are_directly_connected(62149, 62180));
//
//    CHECK_EQUAL(1, are_directly_connected(63184, 63185));
//
//    CHECK_EQUAL(0, are_directly_connected(63236, 9994));
//
//    CHECK_EQUAL(0, are_directly_connected(63998, 90191));
//
//    CHECK_EQUAL(1, are_directly_connected(64191, 64192));
//
//    CHECK_EQUAL(0, are_directly_connected(65059, 46141));
//
//    CHECK_EQUAL(0, are_directly_connected(65828, 61820));
//
//    CHECK_EQUAL(0, are_directly_connected(73561, 58098));
//
//    CHECK_EQUAL(1, are_directly_connected(77519, 18881));
//
//    CHECK_EQUAL(0, are_directly_connected(78917, 33531));
//
//    CHECK_EQUAL(1, are_directly_connected(93575, 93608));
//
//    CHECK_EQUAL(1, are_directly_connected(96016, 96017));
//
//    CHECK_EQUAL(1, are_directly_connected(96233, 96232));
//
//    CHECK_EQUAL(1, are_directly_connected(96800, 69795));
//
//    CHECK_EQUAL(1, are_directly_connected(99938, 99937));
//
//    CHECK_EQUAL(1, are_directly_connected(103323, 103299));
//
//} //directly_connected
//
//TEST(adjacent_intersections) {
//    std::vector<unsigned> expected;
//    std::vector<unsigned> actual;
//
//    expected = {66, 68};
//    actual = find_adjacent_intersections(67);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {1259};
//    actual = find_adjacent_intersections(1258);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {7113, 11895};
//    actual = find_adjacent_intersections(7117);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {};
//    actual = find_adjacent_intersections(10081);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {10326, 10328, 10344};
//    actual = find_adjacent_intersections(10327);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {4014, 28160};
//    actual = find_adjacent_intersections(11818);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {16804, 16811};
//    actual = find_adjacent_intersections(16810);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {22227, 22243, 22246};
//    actual = find_adjacent_intersections(22242);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {32642, 32644, 36148};
//    actual = find_adjacent_intersections(32643);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {39390, 39392, 39394};
//    actual = find_adjacent_intersections(39393);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {42579, 42585, 81908};
//    actual = find_adjacent_intersections(42584);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {45722, 48899};
//    actual = find_adjacent_intersections(46594);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {28689, 51546};
//    actual = find_adjacent_intersections(51535);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {53431, 53469};
//    actual = find_adjacent_intersections(53430);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {56301, 56319, 56322, 56432};
//    actual = find_adjacent_intersections(56302);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {58005, 58007};
//    actual = find_adjacent_intersections(58006);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {7575, 59422, 59437, 59443};
//    actual = find_adjacent_intersections(59444);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {59429, 59446, 59470};
//    actual = find_adjacent_intersections(59447);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {65430, 65438};
//    actual = find_adjacent_intersections(65440);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {72969, 72971};
//    actual = find_adjacent_intersections(72970);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {73828, 73855};
//    actual = find_adjacent_intersections(73847);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {};
//    actual = find_adjacent_intersections(75407);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {61078, 76188, 76260};
//    actual = find_adjacent_intersections(76187);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {77113, 77132};
//    actual = find_adjacent_intersections(77085);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {79643, 79644, 79755};
//    actual = find_adjacent_intersections(79670);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {42399, 42400};
//    actual = find_adjacent_intersections(84571);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {76490, 94999, 100642};
//    actual = find_adjacent_intersections(94998);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {53543, 100130, 100131};
//    actual = find_adjacent_intersections(100129);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {101896, 101905, 101911};
//    actual = find_adjacent_intersections(101904);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//    expected = {11780, 106399, 106400};
//    actual = find_adjacent_intersections(106398);
//    std::sort(actual.begin(), actual.end());
//    CHECK_EQUAL(expected, actual);
//
//} //adjacent_intersections
//
////intersection_queries
//
//SUITE(distance_time_queries) {
//
//    TEST(distance_between_two_points) {
//        double expected;
//        double actual;
//
//        expected = 48901.358372445;
//        actual = find_distance_between_two_points(LatLon(43.89957, -79.504), LatLon(43.50177, -79.76295));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 21420.02901553923;
//        actual = find_distance_between_two_points(LatLon(43.49641, -79.44099), LatLon(43.68565, -79.3917));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 21376.69698715043;
//        actual = find_distance_between_two_points(LatLon(43.51353, -79.42992), LatLon(43.55679, -79.68823));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 54931.5136634997;
//        actual = find_distance_between_two_points(LatLon(43.51741, -79.78996), LatLon(43.87229, -79.31493));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 16312.28938023908;
//        actual = find_distance_between_two_points(LatLon(43.68439, -79.59126), LatLon(43.56063, -79.48255));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 30754.44346434695;
//        actual = find_distance_between_two_points(LatLon(43.52221, -79.47686), LatLon(43.75831, -79.27802));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 32599.47236635839;
//        actual = find_distance_between_two_points(LatLon(43.68481, -79.4072), LatLon(43.57791, -79.78425));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 39023.60685698516;
//        actual = find_distance_between_two_points(LatLon(43.82004, -79.40101), LatLon(43.56441, -79.73337));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 36216.89023684477;
//        actual = find_distance_between_two_points(LatLon(43.48414, -79.33807), LatLon(43.70441, -79.66917));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 33893.61559371489;
//        actual = find_distance_between_two_points(LatLon(43.49874, -79.36289), LatLon(43.80342, -79.35551));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 11389.84747052705;
//        actual = find_distance_between_two_points(LatLon(43.55212, -79.33412), LatLon(43.62733, -79.23817));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 42859.86777600402;
//        actual = find_distance_between_two_points(LatLon(43.7766, -79.33257), LatLon(43.50935, -79.71619));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 42289.1208070249;
//        actual = find_distance_between_two_points(LatLon(43.6515, -79.13216), LatLon(43.55277, -79.6392));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 44458.96272991874;
//        actual = find_distance_between_two_points(LatLon(43.59955, -79.08165), LatLon(43.59901, -79.63361));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 25138.05990056068;
//        actual = find_distance_between_two_points(LatLon(43.62518, -79.13019), LatLon(43.61975, -79.44231));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 30568.60879840858;
//        actual = find_distance_between_two_points(LatLon(43.56346, -79.02532), LatLon(43.65511, -79.38316));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 44386.71395539412;
//        actual = find_distance_between_two_points(LatLon(43.5922, -79.67077), LatLon(43.74093, -79.15884));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 31384.02166422795;
//        actual = find_distance_between_two_points(LatLon(43.62439, -79.65219), LatLon(43.6536, -79.2644));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 6010.516174706887;
//        actual = find_distance_between_two_points(LatLon(43.73144, -79.6271), LatLon(43.746, -79.69913));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 39835.07710941207;
//        actual = find_distance_between_two_points(LatLon(43.66745, -79.55527), LatLon(43.76731, -79.0794));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 14540.15801390921;
//        actual = find_distance_between_two_points(LatLon(43.70654, -79.5463), LatLon(43.82749, -79.4776));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 41176.07519816377;
//        actual = find_distance_between_two_points(LatLon(43.66126, -79.44331), LatLon(43.9166, -79.07198));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 14589.17096791525;
//        actual = find_distance_between_two_points(LatLon(43.83619, -79.33003), LatLon(43.70514, -79.32238));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 20204.57351138864;
//        actual = find_distance_between_two_points(LatLon(43.67019, -79.3085), LatLon(43.85175, -79.31635));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 15129.80999032024;
//        actual = find_distance_between_two_points(LatLon(43.73481, -79.3028), LatLon(43.61821, -79.20594));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 4189.335032112847;
//        actual = find_distance_between_two_points(LatLon(43.77595, -79.29842), LatLon(43.73858, -79.3049));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 10159.97927272361;
//        actual = find_distance_between_two_points(LatLon(43.75237, -79.10904), LatLon(43.81199, -79.01318));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 46939.23730974402;
//        actual = find_distance_between_two_points(LatLon(43.90305, -79.07779), LatLon(43.8575, -79.65986));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 53828.74262934429;
//        actual = find_distance_between_two_points(LatLon(43.91593, -79.03275), LatLon(43.86306, -79.70026));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 9622.930751213542;
//        actual = find_distance_between_two_points(LatLon(43.91782, -79.09747), LatLon(43.90529, -79.21629));
//        CHECK(relative_error(expected, actual) < 0.05);
//
//    } //distance_between_two_points
//
//    TEST(street_segment_length) {
//        double expected;
//        double actual;
//
//        expected = 674.8669630578692;
//        actual = find_street_segment_length(3);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 11.45895038171272;
//        actual = find_street_segment_length(13093);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 147.2853793878423;
//        actual = find_street_segment_length(13789);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 55.33452427648439;
//        actual = find_street_segment_length(25363);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 52.19105536121657;
//        actual = find_street_segment_length(29209);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 294.2969342064328;
//        actual = find_street_segment_length(40483);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 16.96363215314087;
//        actual = find_street_segment_length(45275);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 188.4531341279783;
//        actual = find_street_segment_length(45749);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 118.9660707536393;
//        actual = find_street_segment_length(61287);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 89.5016085750002;
//        actual = find_street_segment_length(65605);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 180.6757446602245;
//        actual = find_street_segment_length(78796);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 313.1600041679221;
//        actual = find_street_segment_length(79298);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 163.2035048827731;
//        actual = find_street_segment_length(86291);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 110.4225050939908;
//        actual = find_street_segment_length(89655);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 51.84283142709663;
//        actual = find_street_segment_length(91146);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 42.79164271231555;
//        actual = find_street_segment_length(92599);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 21.16203073730302;
//        actual = find_street_segment_length(100552);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 199.1431940892923;
//        actual = find_street_segment_length(111826);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 23.5724545268072;
//        actual = find_street_segment_length(114490);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 229.4935408042132;
//        actual = find_street_segment_length(124661);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 314.4718243162012;
//        actual = find_street_segment_length(132117);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 64.37530021982214;
//        actual = find_street_segment_length(134989);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 11.8048977227715;
//        actual = find_street_segment_length(137295);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 16.12895652496253;
//        actual = find_street_segment_length(138509);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 74.60869799599267;
//        actual = find_street_segment_length(138823);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 80.15069069354857;
//        actual = find_street_segment_length(139641);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 55.0836238002361;
//        actual = find_street_segment_length(148062);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 28.89512928411852;
//        actual = find_street_segment_length(148844);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 191.1798124722128;
//        actual = find_street_segment_length(149051);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 40.28622670334227;
//        actual = find_street_segment_length(153228);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//    } //street_segment_length
//
//    TEST(street_length) {
//        double expected;
//        double actual;
//
//        expected = 2671.06002791141;
//        actual = find_street_length(1);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 361.4708733326898;
//        actual = find_street_length(1769);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 86.01868985981315;
//        actual = find_street_length(1863);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 264.2871198025232;
//        actual = find_street_length(3427);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 215.6803142364325;
//        actual = find_street_length(3947);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 726.9529205371654;
//        actual = find_street_length(5470);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 80.929925377697;
//        actual = find_street_length(6117);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 551.6718361278922;
//        actual = find_street_length(6181);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 127.124679485464;
//        actual = find_street_length(8280);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 192.4963326325876;
//        actual = find_street_length(8864);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 89.86085682160366;
//        actual = find_street_length(10646);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 464.2381867752109;
//        actual = find_street_length(10714);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 67.592536441432;
//        actual = find_street_length(11658);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 310.5880522243074;
//        actual = find_street_length(12113);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 492.8213830808821;
//        actual = find_street_length(12314);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 1608.907267253004;
//        actual = find_street_length(12511);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 226.1829357981002;
//        actual = find_street_length(13585);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 446.7137161561176;
//        actual = find_street_length(15108);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 382.8257663506629;
//        actual = find_street_length(15468);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 323.7114821494378;
//        actual = find_street_length(16842);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 211.9677886003495;
//        actual = find_street_length(17849);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 156.8313632142707;
//        actual = find_street_length(18237);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 152.2636889570785;
//        actual = find_street_length(18549);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 2536.5532864551;
//        actual = find_street_length(18713);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 49.48187098300547;
//        actual = find_street_length(18755);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 1039.403236351121;
//        actual = find_street_length(18866);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 1851.418308653855;
//        actual = find_street_length(20003);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 134.2293054475601;
//        actual = find_street_length(20109);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 384.5562253264248;
//        actual = find_street_length(20137);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 684.1251553464502;
//        actual = find_street_length(20701);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//    } //street_length
//
//    TEST(street_segment_travel_time) {
//        double expected;
//        double actual;
//
//        expected = 0.4049201778347216;
//        actual = find_street_segment_travel_time(3);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.01375074045805527;
//        actual = find_street_segment_travel_time(13093);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.1767424552654108;
//        actual = find_street_segment_travel_time(13789);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.06640142913178128;
//        actual = find_street_segment_travel_time(25363);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.06262926643345988;
//        actual = find_street_segment_travel_time(29209);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.3531563210477193;
//        actual = find_street_segment_travel_time(40483);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.02035635858376905;
//        actual = find_street_segment_travel_time(45275);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.226143760953574;
//        actual = find_street_segment_travel_time(45749);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.1427592849043672;
//        actual = find_street_segment_travel_time(61287);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.1074019302900003;
//        actual = find_street_segment_travel_time(65605);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.2168108935922694;
//        actual = find_street_segment_travel_time(78796);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.3757920050015066;
//        actual = find_street_segment_travel_time(79298);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.1958442058593277;
//        actual = find_street_segment_travel_time(86291);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.132507006112789;
//        actual = find_street_segment_travel_time(89655);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.06221139771251596;
//        actual = find_street_segment_travel_time(91146);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.03667855089627047;
//        actual = find_street_segment_travel_time(92599);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.02539443688476363;
//        actual = find_street_segment_travel_time(100552);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.1991431940892923;
//        actual = find_street_segment_travel_time(111826);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.02828694543216864;
//        actual = find_street_segment_travel_time(114490);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.2753922489650559;
//        actual = find_street_segment_travel_time(124661);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.3773661891794414;
//        actual = find_street_segment_travel_time(132117);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.05517882875984755;
//        actual = find_street_segment_travel_time(134989);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.0141658772673258;
//        actual = find_street_segment_travel_time(137295);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.01935474782995503;
//        actual = find_street_segment_travel_time(138509);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.08953043759519119;
//        actual = find_street_segment_travel_time(138823);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.09618082883225827;
//        actual = find_street_segment_travel_time(139641);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.06610034856028331;
//        actual = find_street_segment_travel_time(148062);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.03467415514094222;
//        actual = find_street_segment_travel_time(148844);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.1911798124722128;
//        actual = find_street_segment_travel_time(149051);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//        expected = 0.04834347204401072;
//        actual = find_street_segment_travel_time(153228);
//        CHECK(relative_error(expected, actual) < 0.05);
//
//    } //street_segment_travel_time
//
//} //distance_time_queries
//
//SUITE(spatial_queries) {
//
//    TEST(closest_poi) {
//        CHECK_EQUAL(316, find_closest_point_of_interest(LatLon(43.51741, -79.78996)));
//
//        CHECK_EQUAL(316, find_closest_point_of_interest(LatLon(43.53968, -79.77652)));
//
//        CHECK_EQUAL(3696, find_closest_point_of_interest(LatLon(43.54883, -79.76294)));
//
//        CHECK_EQUAL(6038, find_closest_point_of_interest(LatLon(43.5564, -79.75731)));
//
//        CHECK_EQUAL(3200, find_closest_point_of_interest(LatLon(43.55912, -79.74387)));
//
//        CHECK_EQUAL(7469, find_closest_point_of_interest(LatLon(43.56133, -79.73864)));
//
//        CHECK_EQUAL(681, find_closest_point_of_interest(LatLon(43.56441, -79.73337)));
//
//        CHECK_EQUAL(4870, find_closest_point_of_interest(LatLon(43.57032, -79.68089)));
//
//        CHECK_EQUAL(1752, find_closest_point_of_interest(LatLon(43.5922, -79.67077)));
//
//        CHECK_EQUAL(3523, find_closest_point_of_interest(LatLon(43.59901, -79.63361)));
//
//        CHECK_EQUAL(5844, find_closest_point_of_interest(LatLon(43.60174, -79.57462)));
//
//        CHECK_EQUAL(8024, find_closest_point_of_interest(LatLon(43.61377, -79.50719)));
//
//        CHECK_EQUAL(1312, find_closest_point_of_interest(LatLon(43.61975, -79.44231)));
//
//        CHECK_EQUAL(183, find_closest_point_of_interest(LatLon(43.64908, -79.41658)));
//
//        CHECK_EQUAL(7825, find_closest_point_of_interest(LatLon(43.65142, -79.40141)));
//
//        CHECK_EQUAL(98, find_closest_point_of_interest(LatLon(43.65511, -79.38316)));
//
//        CHECK_EQUAL(8103, find_closest_point_of_interest(LatLon(43.65997, -79.35538)));
//
//        CHECK_EQUAL(3009, find_closest_point_of_interest(LatLon(43.73439, -79.34768)));
//
//        CHECK_EQUAL(1879, find_closest_point_of_interest(LatLon(43.75162, -79.33743)));
//
//        CHECK_EQUAL(6945, find_closest_point_of_interest(LatLon(43.76944, -79.33583)));
//
//        CHECK_EQUAL(5431, find_closest_point_of_interest(LatLon(43.7766, -79.33257)));
//
//        CHECK_EQUAL(8356, find_closest_point_of_interest(LatLon(43.83619, -79.33003)));
//
//        CHECK_EQUAL(8206, find_closest_point_of_interest(LatLon(43.85112, -79.32314)));
//
//        CHECK_EQUAL(8206, find_closest_point_of_interest(LatLon(43.85175, -79.31635)));
//
//        CHECK_EQUAL(7914, find_closest_point_of_interest(LatLon(43.87229, -79.31493)));
//
//        CHECK_EQUAL(6886, find_closest_point_of_interest(LatLon(43.88705, -79.24767)));
//
//        CHECK_EQUAL(8455, find_closest_point_of_interest(LatLon(43.89022, -79.24205)));
//
//        CHECK_EQUAL(3759, find_closest_point_of_interest(LatLon(43.90529, -79.21629)));
//
//        CHECK_EQUAL(3759, find_closest_point_of_interest(LatLon(43.91232, -79.16315)));
//
//        CHECK_EQUAL(8312, find_closest_point_of_interest(LatLon(43.91782, -79.09747)));
//
//    } //closest_poi
//
//    TEST(closest_intersection) {
//        CHECK_EQUAL(94556, find_closest_intersection(LatLon(43.51741, -79.78996)));
//
//        CHECK_EQUAL(9116, find_closest_intersection(LatLon(43.53968, -79.77652)));
//
//        CHECK_EQUAL(9118, find_closest_intersection(LatLon(43.54883, -79.76294)));
//
//        CHECK_EQUAL(63695, find_closest_intersection(LatLon(43.5564, -79.75731)));
//
//        CHECK_EQUAL(80972, find_closest_intersection(LatLon(43.55912, -79.74387)));
//
//        CHECK_EQUAL(63535, find_closest_intersection(LatLon(43.56133, -79.73864)));
//
//        CHECK_EQUAL(63781, find_closest_intersection(LatLon(43.56441, -79.73337)));
//
//        CHECK_EQUAL(64589, find_closest_intersection(LatLon(43.57032, -79.68089)));
//
//        CHECK_EQUAL(64469, find_closest_intersection(LatLon(43.5922, -79.67077)));
//
//        CHECK_EQUAL(23522, find_closest_intersection(LatLon(43.59901, -79.63361)));
//
//        CHECK_EQUAL(38118, find_closest_intersection(LatLon(43.60174, -79.57462)));
//
//        CHECK_EQUAL(59249, find_closest_intersection(LatLon(43.61377, -79.50719)));
//
//        CHECK_EQUAL(10924, find_closest_intersection(LatLon(43.61975, -79.44231)));
//
//        CHECK_EQUAL(5497, find_closest_intersection(LatLon(43.64908, -79.41658)));
//
//        CHECK_EQUAL(4122, find_closest_intersection(LatLon(43.65142, -79.40141)));
//
//        CHECK_EQUAL(96029, find_closest_intersection(LatLon(43.65511, -79.38316)));
//
//        CHECK_EQUAL(12199, find_closest_intersection(LatLon(43.65997, -79.35538)));
//
//        CHECK_EQUAL(55354, find_closest_intersection(LatLon(43.73439, -79.34768)));
//
//        CHECK_EQUAL(50816, find_closest_intersection(LatLon(43.75162, -79.33743)));
//
//        CHECK_EQUAL(7007, find_closest_intersection(LatLon(43.76944, -79.33583)));
//
//        CHECK_EQUAL(18469, find_closest_intersection(LatLon(43.7766, -79.33257)));
//
//        CHECK_EQUAL(4184, find_closest_intersection(LatLon(43.83619, -79.33003)));
//
//        CHECK_EQUAL(82135, find_closest_intersection(LatLon(43.85112, -79.32314)));
//
//        CHECK_EQUAL(91077, find_closest_intersection(LatLon(43.85175, -79.31635)));
//
//        CHECK_EQUAL(61683, find_closest_intersection(LatLon(43.87229, -79.31493)));
//
//        CHECK_EQUAL(89987, find_closest_intersection(LatLon(43.88705, -79.24767)));
//
//        CHECK_EQUAL(89878, find_closest_intersection(LatLon(43.89022, -79.24205)));
//
//        CHECK_EQUAL(76418, find_closest_intersection(LatLon(43.90529, -79.21629)));
//
//        CHECK_EQUAL(38037, find_closest_intersection(LatLon(43.91232, -79.16315)));
//
//        CHECK_EQUAL(93183, find_closest_intersection(LatLon(43.91782, -79.09747)));
//
//
//        //new cases
//        //CHECK_EQUAL(, find_closest_intersection(LatLon(90.0, 0)));
//
//    } //closest_intersection
//
//} //spatial_queries
