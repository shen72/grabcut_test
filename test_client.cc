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

DEFINE_int32(port, 9876, "");

void read_list(string filename, 

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
