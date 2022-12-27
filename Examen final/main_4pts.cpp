#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

#define CSV_PATH "../iris_data.txt"

struct Point {
	double x;
	double y;
	double w;
	double z;
	double dist_from_cluster;
	int cluster;
	void reset() {
		dist_from_cluster = numeric_limits<double>::max();
	}
	Point(double a, double b, double c, double d) : x(a), y(b), w(c), z(d) {
		cluster = -1;
		reset();
	}
	double get_dist_to(const Point &p) {
		return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)*(w-p.w)*(w-p.w)*(z-p.z)*(z-p.z);
	}
};

void loadFile(vector<Point> &points, size_t n_points) {
	ifstream data(CSV_PATH);
	string line;
	getline(data, line); 
	getline(data, line);
	while (getline(data, line)) {
		size_t pos = -1;
		for (int i = 0; i < 5; ++i) {
			pos = line.find(',', pos+1);
		}
		size_t flag1 = line.find(',', pos+1);
		size_t flag2 = line.find(',', flag1+1);
		size_t flag3 = line.find(',', flag2+1);
		size_t flag4 = line.find(',', flag3+1);  
		stringstream x_value(line.substr(pos+1, flag1-pos-1));
		stringstream y_value(line.substr(flag1+1, flag2-flag1-1));
		stringstream w_value(line.substr(flag2+1, flag3-flag2-1));
		stringstream z_value(line.substr(flag3+1, flag4-flag3-1));
		double x, y,w,z;
		x_value >> x;
		y_value >> y;
		w_value >> w;
		z_value >> z;
		points.emplace_back(x, y,w,z);
		if(points.size() >= n_points) return;
	}
}

void getBounds(vector<Point> &points, int &max_x, int &max_y, int &min_x, int &min_y,int &max_z, int &min_z,int &max_w, int &min_w) {
	max_x = max_y = numeric_limits<int>::min();
	min_x = min_y = numeric_limits<int>::max();
	for (int i = 0; i < points.size(); ++i) {
		max_x = max(max_x, int(points[i].x));
		min_x = min(min_x, int(points[i].x));
		max_y = max(max_y, int(points[i].y));
		min_y = min(min_y, int(points[i].y));
		max_z = max(max_z, int(points[i].z));
		min_z = min(min_z, int(points[i].z));
		max_w = max(max_w, int(points[i].w));
		min_w = min(min_w, int(points[i].w));
	}
}

vector<Point> kMeans(vector<Point> &points, size_t k) {
	srand(time(NULL));
	vector<Point> centroids;
	int max_x, max_y, min_x, min_y, max_w, min_w, max_z, min_z;
	getBounds(points, max_x, max_y, min_x, min_y, max_z, min_z, max_w, min_w);
	int limit_x = max(abs(max_x), abs(min_x));
	int limit_y = max(abs(max_y), abs(min_y));
	int limit_w = max(abs(max_w), abs(min_w));
	int limit_z = max(abs(max_z), abs(min_z));
	for (int i = 0; i < k; ++i) {
		float rand_x = -1 * (rand() % 2 ? -1 : 1) * (rand() % limit_x);
		float rand_y = -1 * (rand() % 2 ? -1 : 1) * (rand() % limit_y);
		float rand_w = -1 * (rand() % 2 ? -1 : 1) * (rand() % limit_w);
		float rand_z = -1 * (rand() % 2 ? -1 : 1) * (rand() % limit_z);
		centroids.emplace_back(rand_x, rand_y, rand_w, rand_z);
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
		vector<double> sumX(k, 0.0), sumY(k, 0.0), sumW(k, 0.0), sumZ(k, 0.0);
		for (Point &p : points) {
			sumX[p.cluster] += p.x;
			sumY[p.cluster] += p.y;
			sumW[p.cluster] += p.w;
			sumZ[p.cluster] += p.z;
			pointCount[p.cluster]++;
			p.reset();
		}
		go = false;
		for (int i = 0; i < k; ++i) {
			if (pointCount[i] == 0) continue;
			double new_x = sumX[i] / pointCount[i];
			double new_y = sumY[i] / pointCount[i];
			double new_w = sumW[i] / pointCount[i];
			double new_z = sumZ[i] / pointCount[i];
			if (centroids[i].x != new_x || centroids[i].y != new_y) go = true;
			centroids[i].x = new_x;
			centroids[i].y = new_y;
			centroids[i].z = new_z;
			centroids[i].w = new_w;
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
	vector<Point> centroids = kMeans(points, k);
	vector<int> pointCount(k, 0);
	for (Point &p : points)
		pointCount[p.cluster]++;
	cout<<"------------------------\n";
	cout<<" Centroids:"<<endl;
	for (int i = 0; i < k; ++i) {
		cout<<fixed<<setprecision(3)<<" > ("<<centroids[i].x<<" , "<<centroids[i].y<<" , "<<centroids[i].w<<" , "<<centroids[i].z<<")";
		cout<<" : "<<pointCount[i]<<" points\n";
	}
	cout<<"------------------------\n";
}
