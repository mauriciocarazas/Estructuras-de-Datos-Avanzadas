#include <cstdio>
#include <iostream>
#include "dbscan.h"

#define MINIMUM_POINTS 4     // minimum number of cluster
#define EPSILON (0.75*0.75)  // distance for clustering, metre^2

//reference; https://en.wikipedia.org/wiki/DBSCAN

void readBenchmarkData(vector<Point>& points){ // read benchmark data
    // load point cloud
    FILE *stream; // file stream
    stream = fopen (R"(C:\Users\win 10\Documents\CLION\DbscanAlgorithm\benchmark_hepta.dat)","ra");

    unsigned int minpts, num_points, cluster, i = 0;    // minimum number of cluster, number of points, cluster ID, index
    double epsilon; // distance for clustering
    fscanf(stream, "%u\n", &num_points); // read number of points

    Point *p = (Point *)calloc(num_points, sizeof(Point)); // allocate memory for points

    while (i < num_points){ // read points
        fscanf(stream, "%f,%f,%f,%d\n", &(p[i].x), &(p[i].y), &(p[i].z), &cluster); // read points
        p[i].clusterID = UNCLASSIFIED; // initialize cluster ID
        points.push_back(p[i]); // push points
        ++i; // increment index
    }

    free(p); // free memory
    fclose(stream); // close file stream
}

void printResults(vector<Point>& points, int num_points) { // print results
    int i = 0; // index
    printf("Number of points: %u\n"
           " x     y     z     cluster_id\n"
           "-----------------------------\n"
            , num_points);
    while (i < num_points){ // print points
        printf("%5.2lf %5.2lf %5.2lf: %d\n",
               points[i].x,
               points[i].y, points[i].z,
               points[i].clusterID);
        ++i;
    }
}

int main(){
    vector<Point> points; // points

    // read point data
    readBenchmarkData(points); // read benchmark data

    // constructor
    DBSCAN ds(MINIMUM_POINTS, EPSILON, points); // this is a constructor

    // main loop
    ds.run(); // run DBSCAN

    // result of DBSCAN algorithm
    printResults(ds.m_points, ds.getTotalPointSize()); // print results

    return 0;
}