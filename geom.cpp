#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
using namespace std;

int signed_area2D(point2D a, point2D b, point2D c) {
  int result =  0;
  result = ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
  return result;
}
/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
  int result = 0;
  result = p.x * (q.y - r.y) + q.x * (r.y - p.y) + r.x * (p.y - q.y);
  if (result != 0){
    return 0;
  }
  else {
    return 1;
  }
}

double distance(point2D a, point2D b){
  double result = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
  return result;
}

/* **************************************** */
/*checks the between ness between three points taken from the book
 return value is whether points are between each other */
/* **************************************** */
bool Between(point2D a, point2D b, point2D c){
  if(!collinear(a, b, c)){
    return false;
  }

  if(a.x != b.x){
    return((a.x <= c.x) && (c.x <= b.x)) ||
    ((a.x >= c.x) && (c.x >= b.x));
  }
  else{
    return ((a.y <= c.y) && (c.y <= b.y)) || ((a.y >= c.y) && (c.y >= b.y));
  }
}

/* **************************************** */
/* Taken from the book dont really know what it does */
/* **************************************** */
void Assigndi(point2DD *p, point2D a){
  p->x = a.x;
  p->y = a.y;
}

/* **************************************** */
/* This functions returns whether the point of intersection is collinear
with any ot the segments in the polygon
*/
char ParallelInt(point2D a, point2D b, point2D c, point2D d, point2DD *p){
  if (!collinear(a, b, c)){
    return '0';
  }
  if(Between(a, b, c)){
    Assigndi(p, c);
    return 'e';
  }
  if(Between(a, b, d)){
    Assigndi(p, d);
    return 'e';
  }
  if(Between(c, d, a)){
    Assigndi(p, a);
    return 'e';
  }
  if(Between(c, d, b)){
    Assigndi(p, b);
    return 'e';
  }
  return '0';
}

/* **************************************** */
/*This function checks to see if there are any intersections
between segments and returns the type of intersection and the points
of intersection */
/* **************************************** */
char seg_Intersect(point2D a, point2D b, point2D c, point2D d, point2DD *p){
  double s, t;
  double num, denom;
  char code = '?';

  denom = (a.x * (double)(d.y - c.y)) + (b.x * (double)(c.y - d.y)) +
  (d.x * (double)(b.y - a.y)) + (c.x * (double)(a.y - b.y));


  if(denom == 0.0){
    return ParallelInt(a, b, c, d,p);
  }

  num = (a.x * (double)(d.y - c.y)) + (c.x * (double)(a.y - d.y)) + (d.x * (double)(c.y-a.y));
  if((num == 0.0) || (num == denom)){
    code = 'v';
  }
  s = num/denom;

  num = -((a.x * (double)(c.y - b.y)) + (b.x * (double)(a.y - c.y)) + (c.x * (double)(b.y-a.y)));
  if((num == 0.0) || (num == denom)){
    code = 'v';
  }
  t = num/denom;

  
  if((0.0 < s) && (s <1.0) && (0.0 < t) && (t <1.0)){
    code = 'l';
  }
  else if((0.0 > s) || (s > 1.0) || (0.0 > t) || (t>1.0)){
    code = '0';
  }
  //p->x = a.x + (s * (b.x - a.x));
  //p->y = a.y + (s * (b.y - a.y));
  //cout << code << endl;
  return code;

}
bool isVisible(point2D start, point2D b, vector<vector<point2D> > shapes){
  point2DD *ipoint = new point2DD;
  point2D *intersect = new point2D;
  for(int i = 0; i < shapes.size(); i ++){
    int size = shapes[i].size();
    for(int j = 0; j < shapes[i].size(); j++){
      
      char type;
      type = seg_Intersect(start, b, shapes[i][j], shapes[i][(j+1)%size], ipoint);
      if(type == 'l'){
        return false;
      }
    } 
  }
  return true;
}
//make point outside polygon
// make midpoint of line and check number of intersections with edges of shape
//and outside edge

bool inPoly(point2D a, point2D b, point2D orgin, int shape, vector<vector<point2D > >shapes){
  point2DD *point;
  point2D midpoint;
  int intersects = 0;
  double xmid = (a.x + b.x)/2;
  double ymid = (a.y + b.y)/2;
  midpoint.x = xmid;
  midpoint.y = ymid;
  char type;
  for(int i = 0; i < shapes[shape].size(); i ++){
    int size = shapes[shape].size();
    if(size == 1){
      return false;
    }
    type = seg_Intersect(midpoint, orgin ,shapes[shape][i], shapes[shape][(i+1)%size], point);
    if(type == 'l' || type == 'v' || type == 'e'){
      intersects ++;
    }
  }
  if((intersects % 2) == 1){
    return true;
  }
  if((intersects % 2) ==0){
    return false;
  }

}

void findall(vector<vector<point2D> > shapes){
  point2D orgin;
  orgin.x = 300;
  orgin.y = 0;
  for(int i = 0; i < shapes.size(); i ++){
    for(int j = 0; j < shapes[i].size(); j++){
      vertex *v = new vertex;
      v->point.x = shapes[i][j].x;
      v->point.y = shapes[i][j].y;
      for(int k = 0; k < shapes.size(); k++){
        for(int l = 0; l < shapes[k].size(); l++){
          if(isVisible(shapes[i][j], shapes[k][l], shapes) == true){
            if((j+1 == l) || (j-1 == l)){
              shapes[i][j].visible->push_back(shapes[k][l]);
              continue;
            }
            if(!(inPoly(shapes[i][j], shapes[k][l], orgin, i, shapes))){
              shapes[i][j].visible->push_back(shapes[k][l]);
            }
          }
        }
      }
    } 
  }
}

vector<vertex> find(vector<vector<point2D> > shapes){
  point2D orgin;
  orgin.x = 300;
  orgin.y = 0;
  vector<vertex> alist;
  Segment visible;
  for(int i = 0; i < shapes.size(); i ++){
    for(int j = 0; j < shapes[i].size(); j++){
      vertex *v = new vertex;
      v->point.x = shapes[i][j].x;
      v->point.y = shapes[i][j].y;
      for(int k = 0; k < shapes.size(); k++){
        for(int l = 0; l < shapes[k].size(); l++){
          if(isVisible(shapes[i][j], shapes[k][l], shapes) == true){
            if((j+1 == l) || (j-1 == l)){
              visible.start = v->point;
              point2D ending;
              ending.x = shapes[k][l].x;
              ending.y = shapes[k][l].y;
              visible.end = ending;
              v->adjacent.push_back(visible);
              continue;
            }
            if(!(inPoly(shapes[i][j], shapes[k][l], orgin, i, shapes))){
              visible.start = v->point;
              point2D ending;
              ending.x = shapes[k][l].x;
              ending.y = shapes[k][l].y;
              visible.end = ending;
              v->adjacent.push_back(visible);
            }
          }
        }
      }
      alist.push_back(*v);
    } 
  }
  for(int i = 0; i < alist.size(); i++){
    for(int j = 0; j < alist[i].adjacent.size(); j++){
      if(alist[i].adjacent[i].end.x == alist[i].point.x && 
        alist[i].adjacent[i].end.y == alist[i].point.y){
        alist[i].adjacent.erase(alist[i].adjacent.begin() + j);
      }
    }
  }
  cout << alist.size() << endl;
  return alist;
}
class compare{

public:
 bool operator() (vertex a, vertex b){
    return a.distance > b.distance;
  }
};


vector<point2D> djikstra(vector<vertex> vertices){
  cout << "in djikstra" << endl;
  point2D null;
  null.x = -1;
  null.y = -1;
  
  point2D start;
  start.x = vertices[0].point.x;
  start.y = vertices[0].point.y;
  point2D end = vertices[vertices.size()-1].point;


  set<vertex> v;
  vector<point2D> path;
  map<point2D, point2D> prev;
  map<point2D, double> dist;
  for(int i = 0; i < vertices.size(); i++){
    dist[vertices[i].point] = 100000;
    prev[vertices[i].point] = null;
    //cout << vertices[i].point.x << ", " << vertices[i].point.y << endl;
    v.insert(vertices[i]);
  }
  cout << dist.size() << endl;
  //need to isnert the start into the set
 
  for(map<point2D, double>::iterator it = dist.begin(); it != dist.end(); it++){
    cout << it->first.x << " , " << it->second << endl;
  }
  cout << dist[start] << endl;
  dist[start] = 0;
  cout << dist[start] << endl;
  while(!v.empty()){ 
    vertex curr;
    int min = 100000;
    set<vertex>::iterator minp;
    for(set<vertex>::iterator itr = v.begin(); itr != v.end(); itr++){
      if(dist[itr->point] < min){
        min = dist[itr->point];
        minp = itr;
      }
    }
    cout << "hereee" << endl;
    curr = *minp;
    v.erase(minp);
    

    for(int j = 0; j < curr.adjacent.size(); j++){
      cout << "here???" << endl;
      double newdist = dist[curr.point] + distance(curr.point, curr.adjacent[j].end);
      if(newdist < dist[curr.adjacent[j].end]){
        cout << "less" << endl;
        dist[curr.adjacent[j].end] = newdist;
        cout << dist[curr.adjacent[j].end] << endl;
        prev[curr.adjacent[j].end] = curr.point;
      }
    }
    cout << "aboud to erase" << endl;
    cout << "here" << endl;
  }
 

  cout << "finished this" << endl;
  point2D finish;
  finish = vertices[vertices.size()-1].point;
  path.push_back(finish);
  //cout << "reached here" << endl;
  while(prev[finish].x != -1){
    cout << "int while loop" << endl;
    path.push_back(prev[finish]);
    finish = prev[finish];
  }
  return path;
}

