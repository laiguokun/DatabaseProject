#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "common.h"
#include <vector>
#include <map>
#include <ctime>
using namespace std;

const int point_num = 20000000;
const double threshold = 10000.0;
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
	map<int, Point*> candidate[2];
	candidate[0].clear();
	int cindex = 0;
	for (int i = 0; i < path_cnt; i++)
	{
		candidate[cindex][i] = paths[i]->start;
	}
	Point* p = path->start;
	while (p != NULL)
	{
		candidate[cindex ^ 1].clear();
		int path_index;
		for (map<int, Point*>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
		{
			path_index = it->first;
			Point* next = it->second;
			Point* prev = it->second->prev_point;
			Point* now = NULL;
			double dis_next = point2seg(p, next->edge);
			double dis_prev = point2seg(p, prev->edge);
			double tmp = 0;
			bool flag = false;
			cout << "yes" <<endl;
			while (next != NULL or prev != NULL)
			{
				if (dis_next < dis_prev)
				{
					tmp = dis_next;
					now = next;
					next = next->next_point;
					dis_next = point2seg(p, next->edge);
				}
				else
				{
					tmp = dis_prev;
					now = prev;
					prev = prev->prev_point;
					dis_next = point2seg(p, prev->edge);

				}
				if (tmp <= threshold)
				{
					flag = true;
					break;
				}
			}
			cout << tmp <<endl;
			if (point2point(p, prev) <= threshold) flag = true;
			if (flag) candidate[cindex ^ 1][path_index] = now;
		}
		cindex ^= 1;
		p = p->next_point;
	}
	for (map<int, Point*>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
		res.push_back(it->first);
	return res;
}

void findallsimpath()
{
	vector<int> resultset;
	FILE* output = fopen("allsim.txt", "w");
	for (int i = 0; i < path_cnt; i++)
	{
		clock_t start, finish;
		start = clock();
		resultset = findsimpath(paths[i]);
		finish = clock();
/*		for (int j = 0; j < resultset.size(); j++)
		{
			fprintf(output, "%d %d\n", i, resultset[j]);
			cout << j << endl;
		}*/
		cout << i << " " << resultset.size() << " " << double(finish - start)/((clock_t)1000) << endl;
		break;
	}
}

void shuffleallpath()
{
	int point_index = 0, x, y, t;
	int* path_set = new int[100000];
	int set_cnt = 0;
	for (int i = 0; i < path_cnt; i++)
	{
		while (points[point_index]->path_index == i)
		{
			path_set[set_cnt] = point_index;
			set_cnt ++;
			point_index ++;
		}
		for (int k = 0; k < set_cnt; k ++)
		{
			x = rand()%set_cnt;
			y = rand()%set_cnt;
			t = path_set[x];path_set[x] = path_set[y];path_set[y] = t;
		}
		paths[i]->start = points[path_set[0]];
//		cout << path[i]->path_index << " " << points[path_set[0]]->path_index << endl;
		Point* now = points[path_set[0]];
		for (int k = 1; k < set_cnt; k ++)
		{
			now->next_point = points[path_set[k]];
			now = now->next_point;
		} 
		now->next_point = NULL;
		set_cnt = 0;
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
	path_cnt = -1;
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
				if (path_cnt > 1000)
					break;
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
		cout << cnt << " " << path_cnt << endl;
		break;
	} 
//	shuffleallpath();
	findallsimpath();
}