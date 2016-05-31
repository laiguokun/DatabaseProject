#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

FILE* file;
string filename[3];
int main()
{
	filename[0] = "data/data1.txt";
	filename[1] = "data/data2.txt";
	filename[2] = "data/data3.txt";
	int id;
	double lng, lat;
	char t1[255],t2[255];
	for (int i = 0;i < 3; i ++)
	{
		file = fopen(filename[i].c_str(), "r");
		fscanf(file, "%d,%80[^,],%lf,%lf,%s", &id, &t1, &lng, &lat, &t2);
		cout << id << " " << lng << " " << lat <<endl;
		break;
	} 
}