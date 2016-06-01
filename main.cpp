#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "common.h"
#include <vector>
using namespace std;

const int point_num = 20000000;
FILE* file;
string filename[3];
Point** points;
Edge** edges;
Path** paths;
int cnt = 0;
int path_cnt = 0;
int edge_cnt = 0;

Edge** candidate_set;
int candidate_size;
vector<int> findsimpath(Path* path)
{
	vector<int> res;
	res.clear();
	map<int, bool> candidate[2];
	candidate[0].clear();
	int cindex = 0;
	int path_index = 0;
	for (int i = 0; i < path_cnt; i++)
	{
		if (i != path->path_index) candidate[cindex][i] = true;
	}
	Point* p = path->start;
	while (p->next_point != NULL)
	{
		candidate[cindex ^ 1].clear();
		get_candidate(&candidate_set, &candidate_size);
		for (int i = 0; i < candidate_size; i ++)
		{
			path_index = 
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
		resultset = findsimpath(paths[i]);
	}
}
int main()
{
	candidate_set = new Edge*[pointsum];
	filename[0] = "data/data1.txt";
	filename[1] = "data/data2.txt";
	filename[2] = "data/data3.txt";
	int id;
	double lng, lat;
	int now = -1;
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
				now = id;
				paths[path_cnt] = new Path(points[cnt], path_cnt);
				path_cnt ++;
				if (path_cnt > 100)
					break;
			}
			else
			{
				points[cnt - 1]->next_point = points[cnt];
				edges[edge_cnt] = new Edge(points[cnt - 1], points[cnt], path_cnt);
				points[cnt - 1]->edge = edges[edge_cnt];
				edge_cnt ++;
			}
			points[cnt]->path_index = path_cnt;
			cnt ++;
		}
		cout << cnt << " " << path_cnt << endl;
		break;
	} 
	findallsimpath();
}