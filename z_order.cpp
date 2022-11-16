//z order implementation for 2D points in 3D space (x,y,z) 

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

struct Point
{
    int x;
    int y;
    int z;
    int z_order;
};

int main()
{
    int n;
    cin >> n;
    vector<Point> points(n);
    for (int i = 0; i < n; i++)
    {
        cin >> points[i].x >> points[i].y >> points[i].z;
    }
    for (int i = 0; i < n; i++)
    {
        int z_order = 0;
        for (int j = 0; j < n; j++)
        {
            if (points[i].x < points[j].x && points[i].y < points[j].y && points[i].z < points[j].z)
            {
                z_order++;
            }
        }
        points[i].z_order = z_order;
    }
    for (int i = 0; i < n; i++)
    {
        cout << points[i].z_order << " ";
    }
    return 0;
}

/*
0.5 1.10 0 
0.5 1.5 0 
0.5 0.125 -1 
0.25 0.125 -2 
1.0 2.0 1 
2.0 4.0 2
 
 
*/