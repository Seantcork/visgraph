/* Author Sean Cork */
#include "geom.h"


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<iostream>
#include <vector> 

using namespace std; 

GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};

GLint fillmode = 0;

vector<vector<point2D> > shapes;
vector<point2D> path;
vector<vertex> adjacency;



const int WINDOWSIZE = 750; 

//the current polygon 
vector<point2D>  poly;
vector<point2D> newShape;
point2D start;
point2D ending;
int started = 0;
int ended = 0;

//coordinates of last mouse click
double mouse_x=-10, mouse_y=-10; 
//initialized to a point outside the window

//when this is 1, then clicking the mouse results in those points being stored in poly
int poly_init_mode = 0; 

point2D origin = {0, 0};


void draw_circle(double x, double y){
  glColor3fv(blue);   
  int precision = 100;
  double r = 4; 
  double theta = 0;
  glBegin(GL_POLYGON);
  for(int i = 0; i < precision; i++){
    theta = i * 2 * M_PI/precision;
    glVertex2f(x + r*cos(theta), y + r*sin(theta));
  }
  glEnd();
}



void draw_start(){
   if(shapes.size() <=1){
    return;
  }
  start.x = shapes[0][0].x;
  start.y = shapes[0][0].y;
  glColor3fv(yellow);   
  int precision = 100;
  double r = 4; 
  double theta = 0;
  glBegin(GL_POLYGON);
  for(int i = 0; i < precision; i++){
    theta = i * 2 * M_PI/precision;
    glVertex2f(start.x + r*cos(theta), start.y + r*sin(theta));
  }
  glEnd();
}

void draw_shape(){
  if(newShape.size() <= 1){
    return;
  }
  glColor3fv(blue);   
  int i;
  int size = newShape.size();
  for (i=0; i < newShape.size(); i++) {
    glBegin(GL_LINES);
    glVertex2f(newShape[i].x, newShape[i].y); 
    glVertex2f(newShape[(i+1)%size].x, newShape[(i+1)%size].y);
    glEnd();
  }
}

void draw_end(){
  if(shapes.size() <=1){
    return;
  }
  ending.x = shapes[shapes.size()-1][0].x;
  ending.y = shapes[shapes.size()-1][0].y;
  cout << "seg faulting in draw_end" << endl;
  if(ending.x == NULL){
    return;
  }
  glColor3fv(red);   
  int precision = 100;
  double r = 4; 
  double theta = 0;
  glBegin(GL_POLYGON);
  for(int i = 0; i < precision; i++){
    theta = i * 2 * M_PI/precision;
    glVertex2f(ending.x + r*cos(theta), ending.y + r*sin(theta));
  }
  glEnd();
}
void mousepress(int button, int state, int x, int y) {
  if(state == GLUT_DOWN) {

    mouse_x = x;
    mouse_y = y;
    //(x,y) are in wndow coordinates, where the origin is in the upper
    //left corner; our reference system has the origin in lower left
    //corner, this means we have to reflect y
    mouse_y = WINDOWSIZE - mouse_y; 

    printf("mouse click at (x=%d, y=%d)\n", (int)mouse_x, (int)mouse_y);

    if (poly_init_mode ==1) {
      point2D p = {mouse_x, mouse_y}; 
      cout << p.x << p.y << endl;
      newShape.push_back(p);
    }
  }
  
  glutPostRedisplay();
}

void initialize_shapes(){
	shapes.clear();
	int n = 10; //size of polygon 
  	double rad = 100; 
  	double step = 2 * M_PI / n;
  	point2D *p = new point2D; 
  	vector<point2D> hex;
  	
    for (int i=0; i<n; i++) {

   	 	p->x = WINDOWSIZE/2 + rad * cos (i * step);
   	 	p->y = WINDOWSIZE/2 + rad * sin (i * step);
      p->visible = new vector<point2D>;
      p->prev = new point2D;
    //insert the segment in the array of segments 
   	 hex.push_back(*p); 
  	} //for i
  	shapes.push_back(hex);

  	 vector<point2D> newShape;
  	p->x = 75;
  	p->y = 200;
    p->visible = new vector<point2D>;
    //p->prev = new point2D;
  	newShape.push_back(*p);
  	p->x = 180;
  	p->y = 250;
    p->visible = new vector<point2D>;
    //p->prev = new point2D;
  	newShape.push_back(*p);
  	p->x = 50;
  	p->y = 300;
    p->visible = new vector<point2D>;
    //p->prev = new point2D;
  	newShape.push_back(*p);
  	shapes.push_back(newShape);


    newShape.clear();
    p->x = 50;
    p->y = 50;
    p->visible = new vector<point2D>;
    //p->prev = new point2D;
    newShape.push_back(*p);
    shapes.push_back(newShape);


    newShape.clear();
    p->x = 600;
    p->y = 600;
    p->visible = new vector<point2D>;
    //p->prev = new point2D;
    newShape.push_back(*p);
    shapes.insert(shapes.begin(), newShape);

}

void keypress(unsigned char key, int x, int y) {
  vector<point2D> shape;
  switch(key) {
  case 'q':
    exit(0);
    break;

  case 's':
    mouse_x = mouse_y=0; 
    poly_init_mode = 1; 
    glutPostRedisplay();
    break; 
  case 'c':
    shapes.clear();
    path.clear();
    adjacency.clear();
    glutPostRedisplay();
    break; 
  case 'e': 
    poly_init_mode=0;
    cout << newShape.size() << endl;
    if(newShape.size() > 2){
       shapes.push_back(newShape);
       cout << "  fsshapes size " << newShape.size() << endl;
       cout << newShape[0].x << endl;
    

    newShape.clear();
    glutPostRedisplay();
    break; 
  case 'v':
    //findall(shapes);
    adjacency = find(shapes);
    path = djikstra(adjacency);
    glutPostRedisplay();
    break;
  case 'a':
    start.x = mouse_x;
    start.y = mouse_y;
    shape.clear();
    shape.push_back(start);
    shapes.insert(shapes.begin(), shape);
    glutPostRedisplay();
    break;
  case 'b':
    ending.x = mouse_x;
    ending.y = mouse_y;
    point2D *p = new point2D;
    p->x = mouse_x;
    p->y = mouse_y;
    p->visible = new vector<point2D>;
    shape.clear();
    shape.push_back(*p);
    shapes.push_back(shape);
    glutPostRedisplay();
    break;
  }

}

void draw_initial(){
  // if(poly_init_mode == 1){
  //   return;
  // }
  if(shapes.size() == 0){
    return;
  }
  cout << "made it here" << endl;
  glColor3fv(blue);   
	int j;
	for(int i = 0; i < shapes.size(); i ++){
    cout << "in first loop" << endl;
    cout << "this is shapes size" << shapes.size() << endl;
    cout << shapes[i][0].x << endl;
		for(j = 0; j < shapes[i].size(); j ++){
      cout << "made it second loop" << endl;
			int size = shapes[i].size();
			glBegin(GL_LINES);
		 	glVertex2f(shapes[i][j].x, shapes[i][j].y); 
		 	glVertex2f(shapes[i][(j+1)%size].x, shapes[i][(j+1)%size].y);
			glEnd();
		}

	}
}

void draw_everything(){
  if(started == 0 && ended == 0){
    return;
  }

  glColor3fv(red);
  if(shapes.size() == 0){
    return;
  }
  cout << "seg fault in draw_everything" << endl;
  for(int i = 0; i < shapes.size(); i++){
    cout << "here" << endl;
    for(int j = 0; j < shapes[i].size(); j++){
      int k = 0;
      while(k < shapes[i][j].visible->size()){
        glBegin(GL_LINES);
        cout << "here" << endl;
        glVertex2f(shapes[i][j].x, shapes[i][j].y);\
        cout << "here" << endl;
        glVertex2f(shapes[i][j].visible->at(k).x, shapes[i][j].visible->at(k).y);
        glEnd();
        k++;
      }
    }
  }
  cout << "not seg fault in draw_everything" << endl;

}

void draw_find(){
  glColor3fv(red);
  if(adjacency.size() == 0){
    return;
  }
  for(int i = 0; i < adjacency.size(); i++){
    for(int j = 0; j < adjacency[i].adjacent.size(); j++){
      //cout << "we are going to seg fault here" << endl;
      glBegin(GL_LINES);
      glVertex2f(adjacency[i].adjacent[j].start.x, adjacency[i].adjacent[j].start.y);
      glVertex2f(adjacency[i].adjacent[j].end.x, adjacency[i].adjacent[j].end.y);
      glEnd();
    }
  }

}

void draw_path(){
 

  glColor3fv(yellow);
  //glLineWidth(3);
  if(path.size() == 0){
    return;
  }
  int size = path.size();
  for(int i = 0; i < path.size() -1; i++){
    glBegin(GL_LINES);
    glVertex2f(path[i].x, path[i].y);
    glVertex2f(path[i+1].x, path[i+1].y);
    glEnd();
  }
}


void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); //clear the matrix


  /* The default GL window is [-1,1]x[-1,1] with the origin in the
     center.  
     Our system of coordinates (in which we generate our points) is
     (0,0) to (WINSIZE,WINSIZE), with the origin in the lower left
     corner.
     
     We need to map the points to [-1,1] x [-1,1]  
     
     Assume we are the local coordinate system. 
     
     First we scale down to [0,2] x [0,2] */ 
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);  
   /* Then we translate so the local origin goes in the middle of teh
     window to (-WINDOWSIZE/2, -WINDOWSIZE/2) */
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0); 

  //now we draw in our local coordinate system (0,0) to
  //(WINSIZE,WINSIZE), with the origin in the lower left corner.
  //draw_initial(); 
  //draw a circle where the mouse was last clicked. Note that this
  //point is stored as a global variable and is modified by the mouse
  //handler function
  draw_circle(mouse_x, mouse_y); 
  //initialize_shapes();
  //draw_initial();
  draw_start();
  draw_end();
  //draw_shape(shape);
  //cout << "seg fault in draw_everything" << endl;
  draw_everything();
  draw_find();
  draw_initial();
  draw_shape();
  draw_path();
  //draw_lines();

  /* execute the drawing commands */
  glFlush();
}


int main(int argc, char** argv) {

  //initialize_shapes();
 

  /* initialize GLUT  */
  glutInit(&argc, argv);
  initialize_shapes();
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);
  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);
  glutMouseFunc(mousepress); 
  //  glutIdleFunc(timerfunc); 

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  /* here we can enable depth testing and double buffering and so
     on */
  
  /* give control to event handler */
  glutMainLoop();
  return 0;
}
