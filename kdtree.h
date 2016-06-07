#ifndef KDTREE_H
#define KDTREE_H

#include "common.h"

class Kdtree
{
public:
    struct Node
    {
        Node* lch;
        Node* rch;
        int k;//分割坐标
        double min_lat, min_lng, max_lat, max_lng;
        int n;
        int l,r;
    };
    Edge** edges;
    Kdtree(Edge**, int);
    void find(Node* node, double xmin, double xmax, double ymin, double ymax, int** kd_set, int* cnt);
    Node* root;
    int* index;
    void buildtree(Node*);
    void sort(int **index,int l,int r,int k);
};

#endif // KDTREE_H
