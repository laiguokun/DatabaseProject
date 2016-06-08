
#include "common.h"

#define pi 3.14159265358979323846
#define earthRadiusm 6371.0

using namespace std;

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) 
{
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

inline double sqr(double x)
{
  return x*x;
}

inline double point2point(Point* p1, Point* p2) //the distance between point and point
{
  return distanceEarth(p1->lat, p1->lng, p2->lat, p2->lng);
//  return sqrt(sqr(p1->lat - p2->lat) + sqr(p1->lng - p2->lng));
}

double point2seg(Point* point, Edge* edge) //the distance between point and segment
{
  if (edge == NULL) return 10000000000;
  double x0 = point->lat, y0 = point->lng;
  double A = edge->A, B = edge->B, C = edge->C;
  double res;
  Point* p = new Point;
  p->lat = (B*B*x0 - A*B*y0 - A*C); p->lng = (-A*B*x0 + A*A*y0 - B*C); //the coordinate of the intersection point
//      cout << A << " " << B << " " << C << " " << x0 << " " << y0 << " " << p->lat << " " << p->lng << " " << point2point(point,p)<<endl;
//      cout << point2point(point, edge->start) << " " << point2point(point,edge->end) << endl;
  if (p->lat >= edge->min_lat && p->lat <= edge->max_lat && p->lng >= edge->min_lng && p->lng <= edge->max_lng)
    res = point2point(point, p);
  else
    res = min(point2point(point, edge->start), point2point(point, edge->end));
  delete p;
  return res;
}







