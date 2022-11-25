#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

#define CSV_PATH "../mediumdataset.txt"

struct Point {
    double x;
    double y;
    double dist_from_cluster;
    int cluster;
    void reset() {
        dist_from_cluster = numeric_limits<double>::max();
    }
    Point(double a, double b) : x(a), y(b) {
        cluster = -1;
        reset();
    }
    double get_dist_to(const Point &p) {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }
};

void loadFile(vector<Point> &points, size_t n_points) {
    // 6th row = Start_Lon
    // 7th row = Start_Lat
    ifstream data(CSV_PATH);
    string line;
    getline(data, line); // ignore 1st line
    getline(data, line); // ignore 2nd line
    while (getline(data, line)) {
        size_t pos = -1;
        for (int i = 0; i < 5; ++i) {
            pos = line.find(',', pos+1);
        }
        size_t flag1 = line.find(',', pos+1);
        size_t flag2 = line.find(',', flag1+1);
        stringstream x_value(line.substr(pos+1, flag1-pos-1));
        stringstream y_value(line.substr(flag1+1, flag2-flag1-1));
        double x, y;
        x_value >> x;
        y_value >> y;
        points.emplace_back(x, y);
        if(points.size() >= n_points) return;
    }
}

void getBounds(vector<Point> &points, int &max_x, int &max_y, int &min_x, int &min_y) {
    max_x = max_y = numeric_limits<int>::min();
    min_x = min_y = numeric_limits<int>::max();
    for (int i = 0; i < points.size(); ++i) {
        max_x = max(max_x, int(points[i].x));
        min_x = min(min_x, int(points[i].x));
        max_y = max(max_y, int(points[i].y));
        min_y = min(min_y, int(points[i].y));
    }
}

vector<Point> kMeans(vector<Point> &points, size_t k, bool ask) {
    srand(time(NULL));
    vector<Point> centroids;
    if (!ask) {
        // Random centroids
        int max_x, max_y, min_x, min_y;
        getBounds(points, max_x, max_y, min_x, min_y);
        int limit_x = max(abs(max_x), abs(min_x));
        int limit_y = max(abs(max_y), abs(min_y));
        for (int i = 0; i < k; ++i) {
            float rand_x = -1 * (rand() % 2 ? -1 : 1) * (rand() % limit_x);
            float rand_y = -1 * (rand() % 2 ? -1 : 1) * (rand() % limit_y);
            centroids.emplace_back(rand_x, rand_y);
        }
    }
    else {
        // Ask for centroids
        for (int i = 0; i < k; ++i) {
            cout<<"["<<i+1<<"] Enter x and y coordinates: ";
            double x, y;
            cin>>x>>y;
            centroids.emplace_back(x, y);
        }
    }
    bool go = true;
    while (go) {
        for (int i = 0; i < k; ++i) {
            for (Point &p : points) {
                double dist = centroids[i].get_dist_to(p);
                if (dist < p.dist_from_cluster) {
                    p.dist_from_cluster = dist;
                    p.cluster = i;
                }
            }}
        vector<int> pointCount(k, 0);
        vector<double> sumX(k, 0.0), sumY(k, 0.0);
        for (Point &p : points) {
            sumX[p.cluster] += p.x;
            sumY[p.cluster] += p.y;
            pointCount[p.cluster]++;
            p.reset();
        }
        go = false;
        for (int i = 0; i < k; ++i) {
            if (pointCount[i] == 0) continue;
            double new_x = sumX[i] / pointCount[i];
            double new_y = sumY[i] / pointCount[i];
            if (centroids[i].x != new_x || centroids[i].y != new_y) go = true;
            centroids[i].x = new_x;
            centroids[i].y = new_y;
        }
    }
    return centroids;
}

int main(int argc, char* args[]) {
    vector<Point> points;
    size_t k = (argc >= 2 ? stoi(args[1]) : 10);
    size_t n = (argc >= 3 ? stoi(args[2]) : 7000000);
    loadFile(points, n);

    cout<<"------------------------\n";
    cout<<" Point Count: "<<points.size()<<endl;
    cout<<" k value: "<<k<<endl;
    cout<<"------------------------\n";
    cout<<"Do you want to predefine the "<<k<<" initial centroids? (y/n) ";
    bool ask; char ans;
    cin>>ans;
    ask = (ans == 'y' ? true : false);
    vector<Point> centroids = kMeans(points, k, ask);
    vector<int> pointCount(k, 0);
    for (Point &p : points)
        pointCount[p.cluster]++;
    cout<<"------------------------\n";
    cout<<" Centroids:"<<endl;
    for (int i = 0; i < k; ++i) {
        cout<<fixed<<setprecision(3)<<" > ("<<centroids[i].x<<" , "<<centroids[i].y<<")";
        cout<<" : "<<pointCount[i]<<" points\n";
    }
    cout<<"------------------------\n";
}