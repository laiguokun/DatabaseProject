#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "common.h"
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
const double max_int = 1000000000000;
//map<int, Point*> candidate[2];
Point*** candidate = new Point**[2];
int** pusu_c = new int*[2];
int c_cnt[2];

vector<int> findsimpath_pusu(Path* path)
{
	vector<int> res;
	res.clear();
	c_cnt[0] = 0;c_cnt[1] = 0;
	int cindex = 0;
	for (int i = 0; i < path_cnt; i++)
	{
		if (i != path->path_index)
		{
			pusu_c[cindex][c_cnt[cindex]] = i;
			c_cnt[cindex] ++;
		}
	}
	Point* p = path->start;
//	cout << "ok" <<endl;
	while (p != NULL)
	{
		c_cnt[cindex ^ 1] = 0;
		int path_index;
//		for (map<int, bool>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
		for (int i = 0; i < c_cnt[cindex]; i++)
		{
			path_index = pusu_c[cindex][i];
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
			if (flag) 
			{
				pusu_c[cindex ^ 1][c_cnt[cindex ^ 1]] = path_index;
				c_cnt[cindex ^ 1] ++;
			}
		}
		cindex ^= 1;
		p = p->next_point;
	}
	for (int i = 0; i < c_cnt[cindex]; i++)
		res.push_back(pusu_c[cindex][i]);
	return res;
}

vector<int> findsimpath(Path* path)
{
	vector<int> res;
	res.clear();
	int cindex = 0;
	c_cnt[0] = 0;c_cnt[1] = 0;
	for (int i = 0; i < path_cnt; i++)
	{
		if (i != path->path_index)
		{
			candidate[cindex][c_cnt[cindex]] = paths[i]->start;
			c_cnt[cindex] ++;
		}
	}
	Point* p = path->start;
	while (p != NULL)
	{
		c_cnt[cindex ^ 1] = 0;
		int path_index;
//		for (map<int, Point*>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
		for (int i = 0; i < c_cnt[cindex]; i++)
		{
			path_index = candidate[cindex][i]->path_index;
			Point* next = candidate[cindex][i];
			Point* prev = candidate[cindex][i]->prev_point;
			double dis_next, dis_prev;
			if (prev == NULL) dis_prev = max_int;
			else dis_prev = point2seg(p, prev->edge);
			dis_next = point2seg(p, next->edge);
			Point* now = NULL;
			double tmp = 0;
			bool flag = false;
			while (next != NULL or prev != NULL)
			{
				if (dis_next < dis_prev)
				{
					tmp = dis_next;
					now = next;
					next = next->next_point;
					if (next == NULL) dis_next = max_int;
					else dis_next = point2seg(p, next->edge);
				}
				else
				{
					tmp = dis_prev;
					now = prev;
					prev = prev->prev_point;
					if (prev == NULL) dis_prev = max_int;
					else dis_prev = point2seg(p, prev->edge);
				}
				if (tmp <= threshold)
				{
					flag = true;
					break;
				}
			}
			if (flag) 
			{
				candidate[cindex ^ 1][c_cnt[cindex ^ 1]] = now;
				c_cnt[cindex ^ 1] ++;
			}
		}
		cindex ^= 1;
		p = p->next_point;
	}
//	for (map<int, Point*>::iterator it = candidate[cindex].begin(); it != candidate[cindex].end(); it ++)
	for (int i = 0; i < c_cnt[cindex]; i++)
		res.push_back(candidate[cindex][i]->path_index);
	return res;
}

void findallsimpath()
{
	vector<int> resultset;
	FILE* output = fopen("allsim-1km-pusu.txt", "w");
	clock_t all_start, all_finish;
	all_start = clock();
	for (int i = 0; i < 200; i++)
	{
		clock_t start, finish;
		start = clock();
		resultset = findsimpath_pusu(paths[i]);
		finish = clock();
		for (int j = 0; j < resultset.size(); j++)
		{
			fprintf(output, "%d %d\n", i, resultset[j]);
		}
		cout << i << " " << resultset.size() << " " << double(finish - start)/((clock_t)1000) << endl;
//		break;
	}
	all_finish = clock();
	fprintf(output, "%f\n", double((all_finish - all_start))/((clock_t)1000));
	cout << double(all_finish - all_start)/((clock_t)1000) << endl;
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
	candidate[0] = new Point*[100000];
	candidate[1] = new Point*[100000];
	pusu_c[0] = new int[100000];
	pusu_c[1] = new int[100000];
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
	} 
//	shuffleallpath();
	findallsimpath();
}