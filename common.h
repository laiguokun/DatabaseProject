#include <cmath> 
#include <cstdlib>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

struct Edge;

struct Point
{
  double lat;
  double lng;
  int path_index;
  Point* next_point = NULL;
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
  int path_index;
  Path(Point* s, int index)
  {
    start = s;
    path_index = index;
  }
};

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
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
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
  double x0 = point->lat, y0 = point->lng;
  double A = edge->A, B = edge->B, C = edge->C;
  Point* p = new Point;
  p->lat = (B*B*x0 - A*B*y0 - A*C); p->lng = (-A*B*x0 + A*A*y0 - B*C); //the coordinate of the intersection point
//      cout << A << " " << B << " " << C << " " << x0 << " " << y0 << " " << p->lat << " " << p->lng << " " << point2point(point,p)<<endl;
//      cout << point2point(point, edge->start) << " " << point2point(point,edge->end) << endl;
  if (p->lat >= edge->min_lat && p->lat <= edge->max_lat && p->lng >= edge->min_lng && p->lng <= edge->max_lng)
    return point2point(point, p);
  return min(point2point(point, edge->start), point2point(point, edge->end));
}








