#ifndef COMMON_H
#define COMMON_H

#include <cmath> 
#include <cstdlib>
#include <algorithm>

using namespace std;

struct Edge;

struct Point
{
  double lat;
  double lng;
  int path_index;
  Point* next_point = NULL;
  Point* prev_point = NULL;
  Edge* edge = NULL;
  Point()
  {
    next_point = NULL;
    edge = NULL;
  }
};

struct Edge
{
  Point* start;
  Point* end;
  double max_lat;
  double max_lng;
  double min_lat;
  double min_lng;
  double A,B,C;
  int path_index;

  Edge(Point* start, Point* end, int index)
  {
    this->start = start;
    this->end = end;
    max_lat = max(start->lat, end->lat);
    min_lat = min(start->lat, end->lat);
    max_lng = max(start->lng, end->lng);
    min_lng = min(start->lng, end->lng);
    if (start->lat != end->lat)
    {
      A = -(start->lng - end->lng)/(start->lat - end->lat);
      B = 1;
    }
    else
    {
      A = 1;
      B = -(start->lat - end ->lat)/(start->lng - end->lng);      
    }
    C = -(A * start->lat + start->lng);
    double tmp = sqrt(A*A + B*B);
    A = A/tmp; B = B/tmp; C = C/tmp;
//    cout << A << " " << B << " " << C << " " <<start->lat<<endl;
    path_index = index;
  }
};

struct Path
{
  Point* start;
  Point* end;
  int path_index;
  Path(Point* s, int index)
  {
    start = s;
    path_index = index;
  }
};

// This function converts decimal degrees to radians
double deg2rad(double deg);

//  This function converts radians to decimal degrees
double rad2deg(double rad);

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);

inline double sqr(double x);

inline double point2point(Point* p1, Point* p2); //the distance between point and point

double point2seg(Point* point, Edge* edge); //the distance between point and segment

#endif






