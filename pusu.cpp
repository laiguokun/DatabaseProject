#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "common.h"
#include <vector>
#include <map>
using namespace std;

const int point_num = 20000000;
const double threshold = 10;
FILE* file;
string filename[3];
Point** points;
Edge** edges;
Path** paths;
int cnt = 0;
int path_cnt = 0;
int edge_cnt = 0;


vector<int> findsimpath(Path* path)
{
	vector<int> res;
	res.clear();
	map<int, bool> candidate[2];
	candidate[0].clear();
	int cindex = 0;
	for (int i = 0; i < path_cnt; i++)
	{
		candidate[cindex][i] = true;
	}
	Point* p = path->start;
	while (p->next_point != NULL)
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
	FILE* output = fopen("allsim.txt", "w");
	for (int i = 0; i < path_cnt; i++)
	{
		resultset = findsimpath(paths[i]);
		for (int j = 0; j < resultset.size(); j++)
		{
			fprintf(output, "%d %d\n", i, resultset[j]);
			cout << i << " " << resultset[j] <<endl;
		}
		break;
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