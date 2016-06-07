#include "kdtree.h"
#include <iostream>
#include <cstdlib>

const double eps=1e-7;
const double maxn = 1000000000;

using namespace std;

Kdtree::Kdtree(Edge** edges, int size)
{
    this->edges = edges;
    root=new Node;
    root->max_lat = maxn;
    root->min_lat = -maxn;
    root->max_lng = maxn;
    root->min_lng = -maxn;
    root->l=0;root->r=size;
    root->k=0;
    index=new int[size];
    for (int i=0; i<size; i++)
        index[i]=i;
    double start=clock();
    buildtree(root);
    double end=clock();
    cout<<"build:"<<(end-start)/1000<<endl;
}
void Kdtree::sort(int** index,int l,int r,int k)
{
    int i=l,j=r;
    if (k == 0)
    {
        double x=edges[(*index)[rand()%(r-l+1)+l]]->min_lat;
        while (i<=j)
        {
            while (edges[(*index)[i]]->min_lat<x) i++;
            while (x<edges[(*index)[j]]->min_lat) j--;
            if (i<=j)
            {
                int t=(*index)[i];
                (*index)[i]=(*index)[j];
                (*index)[j]=t;
                i++;j--;
            }
        }
    }
    else
    {
        double x=edges[(*index)[rand()%(r-l+1)+l]]->min_lng;
        while (i<=j)
        {
            while (edges[(*index)[i]]->min_lng<x) i++;
            while (x<edges[(*index)[j]]->min_lng) j--;
            if (i<=j)
            {
                int t=(*index)[i];
                (*index)[i]=(*index)[j];
                (*index)[j]=t;
                i++;j--;
            }
        }
    }
    if (i<r) sort(index,i,r,k);
    if (l<j) sort(index,l,j,k);
}

void Kdtree::buildtree(Node* node)
{
    int k=node->k;

    if (node->l==node->r-1)
    {
        return ;
    }

    sort(&index,node->l,node->r-1,k);

    int mid=(node->l+node->r)/2;
    node->lch=new Node;node->rch=new Node;
    node->lch->l=node->l;node->lch->r=mid;
    node->rch->l=mid;node->rch->r=node->r;

    node->lch->k=(node->k+1)%2;
    node->rch->k=(node->k+1)%2;

    node->lch->min_lat = maxn;
    node->lch->min_lng = maxn;
    node->lch->max_lat = -maxn;
    node->lch->max_lng = -maxn;

    node->rch->min_lat = maxn;
    node->rch->min_lng = maxn;
    node->rch->max_lat = -maxn;
    node->rch->max_lng = -maxn;

    for (int i=node->l;i<mid;i++)
    {
        node->lch->max_lat=fmax(node->lch->max_lat,edges[index[i]]->max_lat);
        node->lch->max_lng=fmax(node->lch->max_lng,edges[index[i]]->max_lng);
        node->lch->min_lat=fmin(node->lch->min_lat,edges[index[i]]->min_lat);
        node->lch->min_lng=fmin(node->lch->min_lng,edges[index[i]]->min_lng);
    }

    for (int i=mid;i<node->r;i++)
    {
        node->rch->max_lat=fmax(node->rch->max_lat,edges[index[i]]->max_lat);
        node->rch->max_lng=fmax(node->rch->max_lng,edges[index[i]]->max_lng);
        node->rch->min_lat=fmin(node->rch->min_lat,edges[index[i]]->min_lat);
        node->rch->min_lng=fmin(node->rch->min_lng,edges[index[i]]->min_lng);
    }

    buildtree(node->lch);
    buildtree(node->rch);
}

void Kdtree::find(Node* node, double xmin, double xmax, double ymin, double ymax, int** kd_set_, int* cnt_)
{
    if (node->min_lat > xmin && node->min_lng > ymin && node->max_lat < xmax && node->max_lng < ymax)
    {
        for (int i = node->l; i < node->r; i++)
        {
            (*kd_set_)[(*cnt_)] = index[i];
            (*cnt_) ++;
        }
        return;
    }

    if (node->min_lat > xmax or node->min_lng > ymax or node->max_lat < xmin or node->max_lng < ymin)
        return;

    if (node->r - node->l == 1)
    {
        (*kd_set_)[(*cnt_)] = index[node->l];
        (*cnt_) ++;
        return;
    }
    find(node->lch, xmin, xmax, ymin, ymax, kd_set_, cnt_);
    find(node->rch, xmin, xmax, ymin, ymax, kd_set_, cnt_);
}

