#include "merge_opencv.h"

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

MergeOpencv::MergeOpencv() {}

MergeOpencv::~MergeOpencv() {}

void MergeOpencv::CalculateRotationSize(int nSrcWidth, int nSrcHeight, int degree,
                                        int& nRotatedWidth, int& nRotatedHeight) {
    Point2f ptCenter(nSrcWidth / 2., nSrcHeight / 2.);
    Mat r = getRotationMatrix2D(ptCenter, degree, 1.0);

    double nCos = abs(r.at<double>(0, 0));
    double nSin = abs(r.at<double>(0, 1));
    nRotatedWidth = (int)(nSrcHeight * nSin + nSrcWidth * nCos);
    if (nRotatedWidth < nSrcWidth) nRotatedWidth = nSrcWidth;
    nRotatedHeight = (int)(nSrcHeight * nCos + nSrcWidth * nSin);
    if (nRotatedHeight < nSrcHeight) nRotatedHeight = nSrcHeight;
}

void MergeOpencv::Merge(unsigned char* imgT, unsigned char* imgv, int width, int height,
                        int iMmatch_score, int nRot, int nDx, int nDy) {
    const int img_size = height * width;
    Mat matT(height, width, CV_8UC1, Scalar(0));
    memcpy((unsigned char*)matT.ptr(), imgT, img_size * sizeof(unsigned char));
    Mat matV(height, width, CV_8UC1, Scalar(0));
    memcpy((unsigned char*)matV.ptr(), imgv, img_size * sizeof(unsigned char));

    double degree = -nRot;  // * 180.0 / 128
    int szBigHeight = height * 2;
    int szBigWidth = width * 2;
    int szRotationWidth = 0, szRotationHeight = 0;
    CalculateRotationSize(szBigWidth, szBigHeight, degree, szRotationWidth, szRotationHeight);
    int offsetX = szRotationWidth / 2;
    int offsetY = szRotationHeight / 2;
    Mat imgRot(szRotationHeight, szRotationWidth, CV_8UC1, Scalar(0));
    matT.copyTo(imgRot(Rect(offsetX, offsetY, matT.cols, matT.rows)));

    // rotate
    Point2f ptCenter(offsetX, offsetY);
    Mat r = getRotationMatrix2D(ptCenter, degree, 1.0);
    Mat imgRotDst;
    warpAffine(imgRot, imgRotDst, r, Size(szRotationWidth, szRotationHeight));

    // convert to color image
    Mat imgColorT;  // green
    cvtColor(imgRotDst, imgColorT, cv::COLOR_GRAY2BGR);
    for (int y = 0; y < szRotationHeight; y++) {
        Vec3b* data = imgColorT.ptr<Vec3b>(y);
        for (int x = 0; x < szRotationWidth; x++) {
            data[x][0] = 0;
            data[x][2] = 0;
        }
    }
    Mat imgColorV;  // red
    cvtColor(matV, imgColorV, cv::COLOR_GRAY2BGR);
    for (int y = 0; y < matV.rows; y++) {
        Vec3b* data = imgColorV.ptr<Vec3b>(y);
        for (int x = 0; x < matV.cols; x++) {
            data[x][0] = 0;
            data[x][1] = 0;
        }
    }

    // merge images
    int nMergedHeight = szRotationHeight + abs(nDy);
    int nMergedWidth = szRotationWidth + abs(nDx);
    Mat imgTmp1(nMergedHeight, nMergedWidth, CV_8UC3, Scalar(0));
    imgColorT.copyTo(
        imgTmp1(Rect(nDx > 0 ? nDx : 0, nDy > 0 ? nDy : 0, imgColorT.cols, imgColorT.rows)));
    Mat imgTmp2(nMergedHeight, nMergedWidth, CV_8UC3, Scalar(0));
    imgColorV.copyTo(
        imgTmp2(Rect(nDx < 0 ? -nDx + offsetX : offsetX, nDy < 0 ? -nDy + offsetY : offsetY,
                     imgColorV.cols, imgColorV.rows)));
    int nPos = 5;
    double alpha = (nPos <= 5) ? 1.0 : 1 - (nPos - 5) / 5.0;
    double beta = (nPos >= 5) ? 1.0 : nPos / 5.0;
    Mat matOut;
    addWeighted(imgTmp1, alpha, imgTmp2, beta, 0.0, matOut);
	
	// crop
	int minX = nMergedWidth;
	int minY = nMergedHeight;
	int maxX = 0;
	int maxY = 0;
	for (int y = 0; y < nMergedHeight; y++) {
		Vec3b* data = matOut.ptr<Vec3b>(y);
		for (int x = 0; x < nMergedWidth; x++) {
			if (data[x][1] > 0 || data[x][2] > 0 /*|| data[x][0] > 0*/)  // G or R not 0
			{
				if (x < minX) minX = x;
				if (x > maxX) maxX = x;
				if (y < minY) minY = y;
				if (y > maxY) maxY = y;
			}
		}
	}
	matOut = matOut(Rect(minX, minY, maxX + 1 - minX, maxY + 1 - minY));
	// add letterbox
	Mat matRes(matOut.rows + 4, matOut.cols + 4, CV_8UC3, Scalar(0, 0, 0));
	matOut.copyTo(matRes(Rect(2, 2, matOut.cols, matOut.rows)));
	matOut = matRes;

    cv::String cvStrRange = cv::format("%d,%d,%d,%d", iMmatch_score, nRot, nDx, nDy);
    putText(matOut, cvStrRange, Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));
	    imwrite("merge.png", matOut);
}

bool MergeOpencv::ReadPng(string sImgPath, unsigned char* img, int& width, int& height) {
    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", sImgPath.c_str());
    Mat matImg = imread(sImgPath.c_str(), IMREAD_GRAYSCALE);
    if (matImg.empty()) {  // check whether the image is loaded or not
        matImg = imread(fn_long, IMREAD_GRAYSCALE);
        if (matImg.empty()) {
            return false;
        }
    }
    width = matImg.cols;
    height = matImg.rows;
    memcpy(img, (uchar*)matImg.ptr(), width * height * sizeof(uchar));

    return true;
}
