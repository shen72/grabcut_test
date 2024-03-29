#include "MaskFloodfill.h"
#include "macros.h"

DEFINE_string(mask_name, "", "");
DEFINE_string(new_name, "result.jpg", "");

using namespace cv;

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  Mat mask = imread(FLAGS_mask_name, 0);
  LOG(INFO) << "Started";

  FloodFillNormalizer foo(200);
  Mat newmask = foo.Normalize(mask);
  imwrite(FLAGS_new_name, newmask);
}
