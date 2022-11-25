//
// Created by HarryLex on 10/11/2022.
//

#ifndef DBSCAN_V1_DBSCAN_H
#define DBSCAN_V1_DBSCAN_H

#include "opencv2/opencv.hpp"
#include <map>
#include <sstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace cv;

class DbScan{
public:
    std::map<int, int> labels; //labels for each point
    std::vector<Rect>& data; //data points
    int C; //number of clusters
    double eps; //radius of neighborhood
    int mnpts; //minimum number of points in a cluster
    double* dp{}; //distance matrix
    //memoization table in case of complex dist functions
#define DP(i,j) dp[(data.size()*i)+j] // distance between i and j
    DbScan(std::vector<Rect>& _data, double _eps, int _mnpts) :data(_data){ //constructor
        C = -1; //no clusters yet
        for (int i = 0; i < data.size(); ++i)  // initialize labels to NOISE
            labels[i] = -99; //unclassified
        eps = _eps; //neighborhood radius
        mnpts = _mnpts; //minimum number of points in a cluster
    }
    void run(){
        dp = new double[data.size()*data.size()]; //distance matrix
        for (int i = 0; i < data.size(); ++i) { //initialize distance matrix
            for (int j = 0; j < data.size(); ++j) { //initialize distance matrix
                if (i == j)
                    DP(i, j) = 0; //distance to self is 0
                else
                    DP(i, j) = -1; //uninitialized
            }
        }
        for (int i = 0; i < data.size(); ++i){ //for each point
            if (!isVisited(i)){ //if not visited
                std::vector<int> neighbours = regionQuery(i); //find neighbours
                if (neighbours.size()<mnpts) //if not enough neighbours
                    labels[i] = -1;//noise
                else{ //enough neighbours
                    C++; //new cluster
                    expandCluster(i, neighbours); //add neighbours to cluster
                }
            }
        }
        delete[] dp; //free memory
    }
    void expandCluster(int p, std::vector<int> neighbours){ //add p to cluster C
        labels[p] = C; //add p to cluster C
        for (int i = 0; i < neighbours.size(); ++i){ //for each neighbour
            if (!isVisited(neighbours[i])){ //if not visited
                labels[neighbours[i]] = C; //add neighbour to cluster C
                std::vector<int> neighbours_p = regionQuery(neighbours[i]); //find neighbours of neighbour
                if (neighbours_p.size() >= mnpts) //if enough neighbours
                    expandCluster(neighbours[i], neighbours_p); //add neighbours to cluster
            }
        }
    }

    bool isVisited(int i){ //check if point i is visited
        return labels[i] != -99; //if not NOISE
    }

    std::vector<int> regionQuery(int p){ //find neighbours of p
        std::vector<int> res; //neighbours
        for (int i = 0; i < data.size(); ++i){ //for each point
            if (distanceFunc(p, i) <= eps) //if close enough
                res.push_back(i); //add to neighbours
        }
        return res; //return neighbours
    }

    double dist2d(Point2d a, Point2d b){ //euclidean distance
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); //euclidean distance
    }

    double distanceFunc(int ai, int bi){
        if (DP(ai, bi) != -1)  //if distance is already computed
            return DP(ai, bi); //return it
        Rect a = data[ai]; //get point a
        Rect b = data[bi]; //get point b
        /*
        Point2d cena= Point2d(a.x+a.width/2,
        a.y+a.height/2);
        Point2d cenb = Point2d(b.x+b.width/2,
        b.y+b.height/2);
        double dist = sqrt(pow(cena.x-cenb.x,2) + pow(cena.y-cenb.y,2));
        DP(ai,bi)=dist;
        DP(bi,ai)=dist;*/
        Point2d tla = Point2d(a.x, a.y); //top left corner of a
        Point2d tra = Point2d(a.x + a.width, a.y); //top right corner of a
        Point2d bla = Point2d(a.x, a.y + a.height); //bottom left corner of a
        Point2d bra = Point2d(a.x + a.width, a.y + a.height); //bottom right corner of a

        Point2d tlb = Point2d(b.x, b.y); //top left corner of b
        Point2d trb = Point2d(b.x + b.width, b.y); //top right corner of b
        Point2d blb = Point2d(b.x, b.y + b.height); //bottom left corner of b
        Point2d brb = Point2d(b.x + b.width, b.y + b.height); //bottom right corner of b

        double minDist = 9999999; //minimum distance

        minDist = min(minDist, dist2d(tla, tlb)); //distance between top left corners
        minDist = min(minDist, dist2d(tla, trb)); //distance between top left and top right corners
        minDist = min(minDist, dist2d(tla, blb)); //distance between top left and bottom left corners
        minDist = min(minDist, dist2d(tla, brb)); //distance between top left and bottom right corners

        minDist = min(minDist, dist2d(tra, tlb)); //distance between top right and top left corners
        minDist = min(minDist, dist2d(tra, trb)); //distance between top right corners
        minDist = min(minDist, dist2d(tra, blb)); //distance between top right and bottom left corners
        minDist = min(minDist, dist2d(tra, brb)); //distance between top right and bottom right corners

        minDist = min(minDist, dist2d(bla, tlb)); //distance between bottom left and top left corners
        minDist = min(minDist, dist2d(bla, trb)); //distance between bottom left and top right corners
        minDist = min(minDist, dist2d(bla, blb)); //distance between bottom left corners
        minDist = min(minDist, dist2d(bla, brb)); //distance between bottom left and bottom right corners

        minDist = min(minDist, dist2d(bra, tlb)); //distance between bottom right and top left corners
        minDist = min(minDist, dist2d(bra, trb)); //distance between bottom right and top right corners
        minDist = min(minDist, dist2d(bra, blb)); //distance between bottom right and bottom left corners
        minDist = min(minDist, dist2d(bra, brb)); //distance between bottom right corners
        DP(ai, bi) = minDist; //save distance
        DP(bi, ai) = minDist; //save distance
        return DP(ai, bi); //return distance
    }

    std::vector<std::vector<Rect> > getGroups(){ //get clusters
        std::vector<std::vector<Rect> > ret; //clusters
        for (int i = 0; i <= C; ++i){ //for each cluster
            ret.push_back(std::vector<Rect>()); //add new cluster
            for (int j = 0; j < data.size(); ++j){ //for each point
                if (labels[j] == i) //if point belongs to cluster
                    ret[ret.size() - 1].push_back(data[j]); //add point to cluster
            }
        }
        return ret; //return clusters
    }
};

void generateRedCyan(Mat &retImage, Mat &image, Mat &depthMap) {
    Mat channel[3];
    //imshow("Original Image", image);

    // The actual splitting.
    split(image, channel);

    // Now I can access each channel separately
    int n, m;
    for (int i = 0; i < image.rows; ++i){ //for each row
        for (int j = 0; j < image.cols; ++j){ //for each column
            n = depthMap.at<uchar>(i,j); //get depth value
            m = (15 * n)/255; //scale depth value
            if (j < image.cols - m - 1){ //if pixel is not too close to the right edge
                //channel[0].at<uchar>(i, j) = channel[0].at<uchar>(i, j + m);
                image.at<cv::Vec3b>(i, j)[0] = image.at<cv::Vec3b>(i, j + m)[0]; //set blue channel to blue channel of pixel m to the right
                //channel[1].at<uchar>(i, j) = channel[1].at<uchar>(i, j + m);
                image.at<cv::Vec3b>(i, j)[1] = image.at<cv::Vec3b>(i, j + m)[1]; //set green channel to green channel of pixel m to the right
            }
            if (j > m){
                //channel[2].at<uchar>(i, j) = channel[2].at<uchar>(i, j - m);
                //image.at<cv::Vec3b>(i, j)[2] = image.at<cv::Vec3b>(i, j - m)[2];
            }
            //grouped.at<uchar>(Point(bottomPoint.x, bottomPoint.y))
            //std::cout << three_channels[0].at<uchar>(i, j) << " " << three_channels[1].at<uchar>(i, j) << " " << three_channels[2].at<uchar>(i, j) << std::endl;
        }
    }
    //merge(channel, 3, retImage);

    retImage = image; //set return image to image
}

Point getMaxPoint(std::vector<cv::Point> &points, int vertical, int horizontal) { //get point with maximum x or y value
    Point max(points.at(0)); //set max point to first point
    for (auto &point : points){ // access by reference to avoid copying
        if ((horizontal * point.x > horizontal * max.x) || (vertical * point.y > vertical * max.y)) //if point is greater than max point
            max = point; //set max point to point
    }
    return max; //return max point
}

Point getBottomPoint(std::vector<cv::Point> &points){ //get point with maximum y value
    return getMaxPoint(points, 1, 0); //return point with maximum y value
}

void getDistinctContures(Mat &insertImage, std::vector<std::vector<cv::Point> > &contours, int thresh = 100, int thresh2 = 200, int blurSize = 3) {
    Mat src_gray; //grayscale image
    Mat canny_output; //canny output
    std::vector<cv::Vec4i> hierarchy; //hierarchy of contours

    /// Convert image to gray and blur it
    cvtColor(insertImage, src_gray, ColorConversionCodes::COLOR_BGR2GRAY); //convert image to grayscale
    blur(src_gray, src_gray, Size(blurSize, blurSize)); //blur image

    //imshow("before dilate", src_gray);

    dilate(src_gray, src_gray, std::vector<Vec6i>()); //dilate image

    //imshow("after dilate",src_gray);

    /// Detect edges using canny
    Canny(src_gray, canny_output, thresh, thresh2, 3);

    /// Find contours
    findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, ContourApproximationModes::CHAIN_APPROX_TC89_KCOS, Point(0, 0)); //find contours
}


#endif //DBSCAN_V1_DBSCAN_H
