#include <RCF/RCF.hpp>
#include <opencv2/opencv.hpp>

#include <exception>
#include <string>

#include "grabcut_test.pb.h"
#include "macros.h"

using namespace std;
using namespace cv;
using namespace GrabCut;

#include "grabcut_test_service.rcf"

DEFINE_string(mask_name, "10.jpg_newmask_opencv.ppm", "");
DEFINE_int32(port, 9876, "");

void ensemble(const string& filename, ImageMaskList* mock_list) {
  Mat img = imread(filename, 0);
  uint8_t* data_ptr = img.data;
  mock_list->clear_masks();
  ImageMask* local_mask = mock_list->add_masks();
  local_mask->set_image_mask(string((char*)data_ptr,
                              img.rows * img.cols));
  local_mask->set_image_id(1);
  mock_list->set_image_set_id("mock set");
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  ImageMaskList mask_list;
  ensemble(FLAGS_mask_name, &mask_list);

  LOG(INFO) << "Successfully read masks";

  RCF::RcfInitDeinit rcf_init;
  RcfClient<I_GrabCutTest> client(
      RCF::TcpEndpoint(static_cast<int>(FLAGS_port)));

  try {
    const TestResponse& response = client.JudgeList(mask_list, mask_list);
    if (response.code() != TestResponse_ResponseCode_OK) {
      LOG(INFO) << "Error: " << response.error_msg();
    }
    LOG(INFO) << "Precision = " << response.precision() << ", "
        << "Recall = " << response.recall();
  } catch (exception& e) {
    LOG(FATAL) << e.what();
  }  
}
