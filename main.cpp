#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "common.h"
#include "kdtree.h"
#include <vector>
#include <map>
#include <ctime>
using namespace std;

const int point_num = 12000000;
const double threshold = 1.0;
FILE* file;
string filename[3];
Point** points;
Edge** edges;
Path** paths;
int cnt = 0;
int path_cnt = 0;
int edge_cnt = 0;
Kdtree* kdtree;
int* cand_set = new int[point_num];
int cand_cnt;

Edge** candidate_set;
int candidate_size;

void get_box(double* dx_, double* dy_, Point* p)
{
	double h = 0, t = threshold * 1000, mid;
	Point* tmp = new Point;
	tmp->lat = p->lat;tmp->lng = p->lng;
	double tt = threshold * 1.1;
	while(abs(t-h) > 1e-5)
	{
		mid = (h+t)/2;
		tmp->lat = p->lat + mid;
		if (point2point(tmp, p) > (tt)) t = mid;
		else h = mid;
	}
	*dx_ = h;
	h = 0; t = threshold * 1000;
	tmp->lat = p->lat;tmp->lng = p->lng;
	while(abs(t-h) > 1e-5)
	{
		mid = (h+t)/2;
		tmp->lng = p->lng + mid;
		if (point2point(tmp, p) > (tt)) t = mid;
		else h = mid;
	}
	*dy_ = h;
}

vector<int> findsimpath(Path* path)
{
	vector<int> res;
	res.clear();
	int cindex = 0;
	int path_index = 0;
	double dx,dy;
	Point* p = path->start;

	cand_cnt = 0;
	get_box(&dx,&dy,p);
	kdtree->find(kdtree->root, p->lat - dx, p->lat + dx, p->lng - dy, p->lng + dy, &cand_set, &cand_cnt);

	map<int, bool> candidate[2];
	candidate[0].clear();
	int tmp;
	for (int i = 0; i < cand_cnt; i++)
	{
		tmp = edges[cand_set[i]]->path_index;
		if (tmp != path->path_index)
			candidate[cindex][tmp] = true;
	}

	
	cout << candidate[cindex].size() <<endl;
	while (p != NULL)
	{
		candidate[cindex ^ 1].clear();
		int path_index;
		for (map<int, bool>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
		{
			path_index = it->first;
			Point* pp = paths[path_index]->start;
			bool flag = false;
//			cout << path_index << " " << pp->edge <<endl;
			while (pp->next_point != NULL)
			{
//				cout << path_index << " " << point2seg(p, pp->edge) << endl;
				if (point2seg(p, pp->edge) <= threshold)
				{
					flag = true;
					break;
				}
				pp = pp->next_point;
			}
			if (flag) candidate[cindex ^ 1][path_index] = true;
		}
		cindex ^= 1;
		p = p->next_point;
	}
	for (map<int, bool>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
		res.push_back(it->first);
	return res;
}

void findallsimpath()
{
	vector<int> resultset;
	for (int i = 0; i < path_cnt; i++)
	{
		clock_t start, finish;
		start = clock();
		resultset = findsimpath(paths[i]);
		finish = clock();
/*		for (int j = 0; j < resultset.size(); j++)
		{
//			fprintf(output, "%d %d\n", i, resultset[j]);
			cout << i << " " << resultset[j] << endl;
		}*/
		cout << i << " " << resultset.size() << " " << double(finish - start)/((clock_t)1000) << endl;
	}
}
int main()
{
	filename[0] = "data/data1.txt";
//	filename[0] = "data/test.txt";
	filename[1] = "data/data2.txt";
	filename[2] = "data/data3.txt";
	int id;
	double lng, lat;
	int now = -1;
	path_cnt = -1;
	char t1[255],t2[255];
	points = new Point*[point_num];
	edges = new Edge*[point_num];
	paths = new Path*[point_num];
	for (int i = 0;i < 3; i ++)
	{
		file = fopen(filename[i].c_str(), "r");
		while (fscanf(file, "%d,%80[^,],%lf,%lf,%s", &id, &t1, &lat, &lng, &t2)!=EOF)
		{
			points[cnt] = new Point();
			points[cnt]->lat = lat;
			points[cnt]->lng = lng;
			if (id != now)
			{
				if (cnt != 0) 
				{
					points[cnt - 1]->edge = NULL;
					points[cnt - 1]->next_point = NULL;
					paths[path_cnt]->end = points[cnt - 1];
				}
				points[cnt]->prev_point = NULL;
				now = id;
				path_cnt ++;
				paths[path_cnt] = new Path(points[cnt], path_cnt);
//				if (path_cnt > 1000)
//					break;
			}
			else
			{
				points[cnt - 1]->next_point = points[cnt];
				edges[edge_cnt] = new Edge(points[cnt - 1], points[cnt], path_cnt);
				points[cnt - 1]->edge = edges[edge_cnt];
				points[cnt]->prev_point = points[cnt-1];
				edge_cnt ++;
			}
			points[cnt]->path_index = path_cnt;
			cnt ++;
		}
		cout << cnt << " " << path_cnt << " " << edge_cnt << endl;
		break;
	} 
	/*
	double avg = 0;
	double dmin = 1000000;
	double dmax = -dmin;
	double dis = 0;
	for (int i = 0; i < edge_cnt; i++)
	{
		dis = point2point(edges[i]->start, edges[i]->end);
		avg += dis;
		dmin = min(dmin,dis);
		dmax = max(dmax,dis);
	}
	avg = avg/edge_cnt;
	cout << avg << " " << dmin << " " << dmax << endl;
	*/
	kdtree = new Kdtree(edges, edge_cnt);
	findallsimpath();
}