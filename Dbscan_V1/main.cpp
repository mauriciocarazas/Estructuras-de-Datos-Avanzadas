#include <iostream>
#include "DbScan.h"

void test(){
    /// Initialize vars
    Mat im; //image
    RNG rng(12345); //random number generator
    std::vector<std::vector<cv::Point> > contours; //contours

    /// Load source image and convert it to gray

    //im = imread(R"(C:\Users\win 10\Documents\CLION\Dbscan_V1\cat.jpg)", IMREAD_COLOR);
    im = imread(R"(C:\Users\win 10\Documents\CLION\Dbscan_V1\flower.jpg)", IMREAD_COLOR); //read image

    /// Check for invalid input
    if (!im.data){ //if image is empty
        std::cout << "Could not open or find the image" << std::endl; //if image is invalid, print error message
        //return -1; //return error code
    }

    getDistinctContures(im, contours); //get contours

    /// Convert every contour into its bounding box
    std::vector<Rect> boxes;
    for (size_t i = 0; i < contours.size(); ++i){ //for each contour
        Rect r = boundingRect(contours[i]); //get bounding box of contour
        boxes.push_back(r); //add bounding box to vector
    }

    /// Merge bounding boxes that come from same object
    int dbScanDistance = ((im.size().height + im.size().width) /2) * 0.02;//10;//
    DbScan dbscan(boxes, dbScanDistance, 2); //create dbscan object with boxes, distance, and min points
    dbscan.run(); //run dbscan

    /// Set linear gradient (255 gray levels)
    Mat lines(im.size(), CV_8U, Scalar(0)); //create image with same size as original image
    int col = 0; // goes from 32 to 223
    int rowLen = lines.rows; //number of rows
    for (int r = 0; r < rowLen; ++r){ //for each row
        col = (191 * r) / rowLen; //set column value
        lines.row(r).setTo(col + 32); //set row to column value
    }
    //namedWindow("Linear Gradient", CV_WINDOW_NORMAL);
    //imshow("Linear Gradient", lines);

    Mat grouped = lines;//Mat::zeros(im.size(), CV_8UC3);//

    std::vector<std::vector<cv::Point> > contours2(dbscan.C + 1, std::vector<cv::Point>()); //create vector of contours
    std::vector<Scalar> colors; //create vector of colors
    RNG rng2(3); //random number generator
    int colr = 0; //color value
    for (int i = 0; i <= dbscan.C; ++i){ //for each cluster
        //colors.push_back(HSVtoRGBcvScalar(rng(255), 255, 255));
        colr = 32 + (191*i) / dbscan.C; //set color value
        //colors.push_back(Scalar(256 - colr, 256 - colr, 256 - colr));
    }
    for (int i = 0; i < dbscan.data.size(); ++i){ //for each data point
        Scalar color;
        if (dbscan.labels[i] == -1) //if data point is noise
            color = Scalar(128, 128, 128); //set color to gray
        else{
            int label = dbscan.labels[i]; //get label of data point
            //color = colors[label];
            contours2[label].insert(contours2[label].end(), contours[i].begin(), contours[i].end()); //add contour to vector of contours
        }
        //drawContours(grouped, contours, i, color, CV_FILLED);
    }

    // nakon grupisanja kontura iz contours2 probati da svaku konturu posebno nacrtas na slici pa od takve slike izvadis konture i to external?
    Mat slicica = Mat::zeros(grouped.size(), CV_8UC3); //create image with same size as original image

    /// Draw merged contours on new image
    Mat drawingContours2 = Mat::zeros(grouped.size(), CV_8UC3); //create image with same size as original image
    for (int i = 0; i < contours2.size(); ++i){ //for each contour
        // find bottom pixel on contours2
        Point bottomPoint = getBottomPoint(contours2[i]); //get bottom point of contour
        // get color from pixel beneeth
        colors.push_back(grouped.at<uchar>(Point(bottomPoint.x, bottomPoint.y))); //get color from pixel beneeth

        Scalar color = Scalar(rng2.uniform(0, 255), rng2.uniform(0, 255), rng2.uniform(0, 255)); //set color
        drawContours(drawingContours2, contours2, i, color, LineTypes::FILLED, 8, std::vector<Vec4i>(), 0, Point()); //draw contour on image
        //drawContours(drawingContours2, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        drawContours(slicica, contours2, i, Scalar(255, 255, 255), LineTypes::FILLED, 8, std::vector<Vec4i>(), 0, Point()); //draw contour on image
    }
    /// Fill single object with color
    std::vector<std::vector<Point>>hull(contours2.size()); //create vector of contours
    for (int i = 0; i < contours2.size(); ++i) //for each contour
        convexHull(Mat(contours2[i]), hull[i], false); //get convex hull of contour

    /// Draw contours + hull results
    Mat drawing = Mat::zeros(grouped.size(), CV_8UC3); //create image with same size as original image
    for (int i = 0; i < contours2.size(); ++i){ //for each contour
        Scalar color = colors[i];//Scalar(rng2.uniform(0, 255), rng2.uniform(0, 255), rng2.uniform(0, 255));//

        drawContours(grouped, hull, i, color, -100, 8, std::vector<Vec4i>(), 0, Point()); // -- ovo ne izgleda fino zato sto je konveksan hull umjesto konkavan
    }

    /// Show starting image
    imshow("im", im);
    /// Show generated depthmap of an image
    imshow("grouped", grouped);
    /// Write depthmap to a file
    //imwrite(R"(C:\Users\win 10\Documents\CLION\Dbscan_V1\cat_dbscan.jpg)", grouped);
    imwrite(R"(C:\Users\win 10\Documents\CLION\Dbscan_V1\flower.jpg)", grouped);

    /// Generate red-cyan image
    Mat redCyan = Mat::zeros(grouped.size(), CV_8UC3);
    generateRedCyan(redCyan, im, grouped);

    /// Show red-cyan 3d image
    imshow("redCyan", redCyan);
    /// Write red-cyan 3d image to a file
    //imwrite(R"(C:\Users\win 10\Documents\CLION\Dbscan_V1\cat_redCyan.jpg)", redCyan);
    imwrite(R"(C:\Users\win 10\Documents\CLION\Dbscan_V1\flower_redCyan.jpg)", redCyan);
    waitKey(0);
}

int main(){
    test(); //run test
}