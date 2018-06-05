#ifndef __geom_h
#define __geom_h
#include <vector>

using namespace std;

typedef struct point2D{
  int x,y;
  double distance;
  char type;
  struct point2D *prev;
  vector<point2D> *visible;
  bool operator==(const point2D &o) const {
        return x == o.x && y == o.y;
    }
   bool operator<(const point2D &o) const {
        return x > o.x || (x == o.x && y < o.y);
    }
}point2D;

typedef struct _Segment{
  point2D start;
  point2D end;
  int index;
}Segment;


typedef struct _vertex{
  int index;
  point2D point;
  vector<Segment> adjacent;
  vector<point2D> prev;
  double distance;
  bool operator==(const _vertex &o) const {
        return point.x == o.point.x && point.y == o.point.y;
    }
   bool operator<(const _vertex &o) const {
        return point.x > o.point.x || (point.x == o.point.x && point.y < o.point.y);
    }
}vertex;

typedef struct _point2DD {
  double x;
  double y;
  double angle;
  int distance;
  char type;
} point2DD; 

void findall(vector<vector<point2D> > shapes);
vector<point2D> djikstra(vector<vertex> vertices);
vector<vertex> find(vector<vector<point2D> > shapes);
#endif