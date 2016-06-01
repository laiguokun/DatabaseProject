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
int path_cnt = -1;
int edge_cnt = 0;

vector<int> findsimpath(Path* path)
{
	vector<int> res;
	res.clear();
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
			if (id != now)
			{
				path_cnt ++;
				now = id;
				paths[path_cnt] = new Path(points[cnt], path_cnt);
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
			points[cnt]->lat = lat;
			points[cnt]->lng = lng;
			cnt ++;
		}
		cout << cnt << " " << path_cnt << endl;
		break;
	} 
	findallsimpath();
}