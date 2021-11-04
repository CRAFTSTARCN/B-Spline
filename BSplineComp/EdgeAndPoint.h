#ifndef EDGEANDPOINT_H
#define EDGEANDPOINT_H

class Point {

    public:
    float x,y;
    Point();
    Point(float xCord, float yCord);
    ~Point();

    Point& operator=(const Point& p);
};

class Edge {
    
    public:
    int ymax;
    float x;
    float dx;
    Edge* next;

    Edge();
    Edge(int ym, float xpos, float xstep, Edge* nxt);
    ~Edge();
};

#endif