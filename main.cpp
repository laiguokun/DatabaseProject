#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "common.h"
using namespace std;

const int point_num = 20000000;
FILE* file;
string filename[3];
Point** points;
Path** paths;
int main()
{
	filename[0] = "data/data1.txt";
	filename[1] = "data/data2.txt";
	filename[2] = "data/data3.txt";
	int id;
	double lng, lat;
	int index = -1;
	int cnt = 0;
	int path_cnt = 0;
	int now = -1;
	char t1[255],t2[255];
	points = new Point*[point_num];
	paths = new Path*[point_num];
	for (int i = 0;i < 3; i ++)
	{
		file = fopen(filename[i].c_str(), "r");
		while (fscanf(file, "%d,%80[^,],%lf,%lf,%s", &id, &t1, &lat, &lng, &t2)!=EOF)
		{
			points[cnt] = new Point();
			points[cnt]->path_index = index;
			points[cnt]->lat = lat;
			points[cnt]->lng = lng;
			if (id != now)
			{
				index ++;
				now = id;
			}
			else
			{
				points[cnt - 1]->next_point = points[cnt];
				paths[path_cnt] = new Path(points[cnt - 1], points[cnt]);
				path_cnt ++;
			}
			cnt ++;
		}
		cout << cnt << " " << path_cnt << endl;
	} 
}