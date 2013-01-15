#ifndef MASKFLOODFILL_H
#define MASKFLOODFILL_H 

#include <opencv2/opencv.hpp>
#include "macros.h"

using cv::Mat;
using cv::Point;

class FloodFillNormalizer {
 public:
  explicit FloodFillNormalizer(int area_threshold = 0) 
      : area_threshold_(area_threshold) {}
  Mat Normalize(const Mat& mask);
  
 private:
  void fill_(int y, int x, int id, Mat* work_, const Mat& mask_);
  bool inrange_(const Point& p, const cv::Size& size);
  size_t area_threshold_;
  DISALLOW_COPY_AND_ASSIGN(FloodFillNormalizer);
};

#endif /* MASKFLOODFILL_H */
