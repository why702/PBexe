#ifndef DRY_DETECTOR_OPENCV_H
#define DRY_DETECTOR_OPENCV_H

#include <string>

class MergeOpencv {
   public:
    MergeOpencv();
    ~MergeOpencv();

	bool ReadPng(std::string sImgPath, unsigned char* img, int& width, int& height);

    void Merge(unsigned char* imgT, unsigned char* imgv, int width, int height, int iMmatch_score,
               int nRot, int nDx, int nDy);

   private:
    void CalculateRotationSize(int nSrcWidth, int nSrcHeight, int degree, int& nRotatedWidth,
                               int& nRotatedHeight);
};

#endif
