#include "kdtree.h"
#include "common.h"
const double eps=1e-7;
kdtree::kdtree(Edge** edges, int size)
{
    this->edges = edges;
    root=new Node;
    root->box = new Edge();
    root->box->max_lat = maxn;
    root->box->min_lat = -maxn;
    root->box->max_lng = maxn;
    root->box->min_lng = -maxn;
    root->l=0;root->r=size;
    root->k=0;
    obj=new int[size];
    for (int i=0;i<scene->num;i++)
        obj[i]=i;
    double start=clock();
    buildtree(root);
    double end=clock();
    cout<<"build:"<<end-start<<endl;
}
void kdtree::sort(int** obj,int l,int r,int k)
{
    int i=l,j=r;
    Vec3f x=scene->obj[(*obj)[rand()%(r-l+1)+l]]->boxmin;
    while (i<=j)
    {
        while (scene->obj[(*obj)[i]]->boxmin[k]<x[k]) i++;
        while (x[k]<scene->obj[(*obj)[j]]->boxmin[k]) j--;
        if (i<=j)
        {
            int t=(*obj)[i];
            (*obj)[i]=(*obj)[j];
            (*obj)[j]=t;
            i++;j--;
        }
    }
    if (i<r) sort(obj,i,r,k);
    if (l<j) sort(obj,l,j,k);
}
Vec3f maxvec=Vec3f(maxn,maxn,maxn);
Vec3f minvec=Vec3f(-maxn,-maxn,-maxn);
void kdtree::buildtree(Node* node)
{
    int k=node->k;
    if (node->l==node->r-1)
    {
        return ;
    }
    sort(&obj,node->l,node->r-1,k);
    int mid=(node->l+node->r)/2;
    node->lch=new Node;node->rch=new Node;
    node->lch->l=node->l;node->lch->r=mid;
    node->rch->l=mid;node->rch->r=node->r;
    node->lch->k=(node->k+1)%3;
    node->rch->k=(node->k+1)%3;
    node->lch->boxmin=maxvec;
    node->lch->boxmax=minvec;
    node->rch->boxmin=maxvec;
    node->rch->boxmax=minvec;
    for (int i=node->l;i<mid;i++)
    {
        node->lch->boxmax=Vgetmax(node->lch->boxmax,scene->obj[obj[i]]->boxmax);
        node->lch->boxmin=Vgetmin(node->lch->boxmin,scene->obj[obj[i]]->boxmin);
    }
    for (int i=mid;i<node->r;i++)
    {
        node->rch->boxmax=Vgetmax(node->rch->boxmax,scene->obj[obj[i]]->boxmax);
        node->rch->boxmin=Vgetmin(node->rch->boxmin,scene->obj[obj[i]]->boxmin);
    }
    buildtree(node->lch);
    buildtree(node->rch);
}
void kdtree::travel(Ray ray,Node* node,int& crash,double& dist)
{
/*    if (fabs(ray.dir.x-0.449166)<1e-4 && fabs(ray.dir.y+0.616722)<1e-4)
        {
            print(node->boxmin);
            print(node->boxmax);
            system("pause");
        }*/
    if (node->l==node->r-1)
    {
        Vec3f p,p1,p2;
        double d_;
        if (scene->obj[obj[node->l]]->intersect(ray,d_,p,p1,p2))
        {
            dist=d_;
            crash=obj[node->l];
        }
        return;
    }
    double d1=maxn,d2=maxn;
    double t1=-1,t2=-1;
    intersect(ray,node->lch,t1);
    intersect(ray,node->rch,t2);
    int c1=-1,c2=-1;
    int t=0;
    if (inside(ray.o,node->lch)) {t=1;t1=0;}
    if (inside(ray.o,node->rch)) {t=2;t2=0;}
    if (t==0)
    {
        if (t1==-1 && t2==-1) return;
        if (t1<t2 || t2==-1) t=1;
        if (t1>t2 || t1==-1) t=2;
    }
    if (t==1)
    {
        travel(ray,node->lch,c1,d1);
        if ((d1<t2-eps)||(t2==-1))
        {
            dist=d1;crash=c1;return;
        }
        travel(ray,node->rch,c2,d2);
        if (d1<d2)
        {
            dist=d1;crash=c1;
        }
        else
        {
            dist=d2;crash=c2;
        }
    }
    else
    {
        travel(ray,node->rch,c1,d1);
        if ((d1<t1-eps)||(t1==-1))
        {
            dist=d1;crash=c1;return;
        }
        travel(ray,node->lch,c2,d2);
        if (d1<d2)
        {
            dist=d1;crash=c1;
        }
        else
        {
            dist=d2;crash=c2;
        }
    }
    return;
}

int kdtree::gethit(Ray ray,double& d)
{
    int crash=-1;
    travel(ray,root,crash,d);
    double end=clock();
    return crash;
}

bool kdtree::inside(Vec3f p, Node *node)
{
    bool o=true;
    for (int i=0;i<3;i++)
        if (p[i]<node->boxmin[i]-eps || p[i]>node->boxmax[i]+eps)
        {
            o=false;
            break;
        }
    return o;
}
bool kdtree::intersect(Ray ray, Node *node,double& dist)
{
    double x1,x2,y1,y2,z1,z2;
    double x,y,z;
    x1=(node->boxmin.x-ray.o.x)/ray.dir.x;
    x2=(node->boxmax.x-ray.o.x)/ray.dir.x;
    y1=(node->boxmin.y-ray.o.y)/ray.dir.y;
    y2=(node->boxmax.y-ray.o.y)/ray.dir.y;
    z1=(node->boxmin.z-ray.o.z)/ray.dir.z;
    z2=(node->boxmax.z-ray.o.z)/ray.dir.z;
    x=fgetmin(x1,x2);y=fgetmin(y1,y2);z=fgetmin(z1,z2);
    double max=f3getmax(x,y,z);
    if (max<0) return false;
    Vec3f p;
    p=ray.o+ray.dir*(max);
    if (inside(p,node))
    {
        dist=max;
        return true;
    }
    else return false;
}
