#include <algorithm>
#include <queue>
#include <boost/scoped_array.hpp>
#include "MaskFloodfill.h"

using boost::scoped_array;

Mat FloodFillNormalizer::Normalize(const Mat& mask) {
  CHECK(mask.type() == CV_8UC1)
      << "Invalid Input Mask Matrix for FloodFill Normalizing";

  Mat work_(mask.rows, mask.cols, CV_32SC1, cv::Scalar(0));
  
  LOG(INFO) << "Mask size : " << work_.rows << " x " << work_.cols;
  LOG(INFO) << "Initialized";

  for (int y = 0; y < work_.rows; y++) {
    fill_(y, 0, 1, &work_, mask);
    fill_(y, work_.cols - 1, 1, &work_, mask);
  }  

  for (int x = 0; x < work_.cols; x++) {
    fill_(0, x, 1, &work_, mask);
    fill_(work_.rows - 1, 0, 1, &work_, mask);
  }

  int id = 2;
  for (int y = 0; y < work_.rows; y++) {
    for (int x = 0; x < work_.cols; x++) {
      if (work_.at<uint8_t>(y, x) == 0 && mask.at<uint8_t>(y, x) == 0) {
        fill_(y, x, id, &work_, mask);
        id++;
      }
    }
  }
 
  LOG(INFO) << "Filled";
  scoped_array<int> count_(new int[id]);
  std::fill(count_.get(), count_.get() + id, 0);
  for (int y = 0; y < work_.rows; y++) {
    for (int x = 0; x < work_.cols; x++) {
      const int id_ = work_.at<uint8_t>(y, x);
      if (id_ > 1 && id_ < id) { count_[id_]++; }
    }
  }

  LOG(INFO) << "ID count = " << id;

  for (int i = 2; i < id; i++) {
    LOG(INFO) << "ID " << i << ":" << count_[i];
  }

  Mat retval = mask.clone();
  for (int y = 0; y < mask.rows; y++) {
    for (int x = 0; x < mask.cols; x++) {
      const int id_ = work_.at<uint8_t>(y, x);
      if (id_ > 1 && count_[id_] < area_threshold_) {
        retval.at<uint8_t>(y, x) = 255;
      }
    }
  }

  return retval;
}

void FloodFillNormalizer::fill_(int y, int x, int id, Mat* work_,
                                const Mat& mask_) {
  const Point dp[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

  using namespace std;
  if (mask_.at<uint8_t>(y, x) != 0 ||
      work_->at<uint8_t>(y, x) != 0) { return; }

  queue<Point> q_;
  q_.push(Point(x, y));
  work_->at<uint8_t>(y, x) = id;
  while (!q_.empty()) {
    Point p_ = q_.front();
    for (int d = 0; d < 4; d++) {
      Point pp_ = p_ + dp[d];
      if (inrange_(pp_, mask_.size()) && 
          work_->at<uint8_t>(pp_) == 0 &&
          mask_.at<uint8_t>(pp_) == 0) {
        work_->at<uint8_t>(pp_) = id;
        q_.push(pp_);
      }
    }
    q_.pop();
  }
}

bool FloodFillNormalizer::inrange_(const Point& p, const cv::Size& size) {
  return p.x > 0 && p.y > 0 && p.x < size.width && p.y < size.height;
}
