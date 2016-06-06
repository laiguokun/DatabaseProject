#ifndef KDTREE_H
#define KDTREE_H
#include "common.h"
#include "object.h"
class kdtree
{
public:
    struct Node
    {
        Node* lch;
        Node* rch;
        int k;//分割坐标
        Edge* box;
        int n;
        int l,r;
    };
    Edge** edges;
    kdtree(Edge**, int);
    void find(Node* node, double xmin, double xmax, double ymin, double ymax, int** kd_set, int* cnt);
    Node* root;
    int* index;
    void buildtree(Node*);
    void sort(int **index,int l,int r,int k);
};

#endif // KDTREE_H
