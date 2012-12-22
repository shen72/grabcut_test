#include <algorithm>
#include <exception>
#include <string>

#include <RCF/RCF.hpp>

#include "grabcut_test.pb.h"
#include "macros.h"

using GrabCut::ImageMask;
using GrabCut::ImageMaskList;
using GrabCut::TestResponse;
using google::protobuf::RepeatedPtrField;
using namespace std;

#include "grabcut_test_service.rcf"

DEFINE_int32(port, 9876, "");

static bool ImageMaskComparator(const ImageMask& img1, const ImageMask& img2);

class GrabcutMaskJudge {
 public:
  static void JudgeOne(const ImageMask& img, const ImageMask& gt,
                       double *prec, double* recall) {
    
    CHECK(img.image_id() == gt.image_id() &&
          img.image_mask().size() == gt.image_mask().size());

    size_t TP = 0, FP = 0, FN = 0;
    const string& img_content = img.image_mask();
    const string& gt_content = gt.image_mask();

    const size_t n = gt_content.length();
    for (size_t i = 0; i < n; i++) {
      if (img_content[i]) {
        if (gt_content[i]) { TP++; }
        else { FP++; }   
      } else {
        if (gt_content[i]) { FN++; }
      }
    }

    double prec_base = TP + FP ? TP + FP : 1.0;
    double recall_base = TP + FN? TP + FN : 1.0;

    *prec = TP / prec_base;
    *recall = TP / recall_base;
  }
  
  static void JudgeList(const ImageMaskList& test_list,
                        const ImageMaskList& gt_list,
                        double *prec, double *recall) {
    size_t n = test_list.masks().size();
    CHECK(n == gt_list.masks().size());

    if (gt_list.has_image_set_id()) {
      CHECK(test_list.image_set_id() == test_list.image_set_id());
    }

    ImageMaskList test_list_copy = test_list,
                  gt_list_copy = gt_list;

    RepeatedPtrField<ImageMask>* test_mask_ptr
        = test_list_copy.mutable_masks();
    RepeatedPtrField<ImageMask>* gt_mask_ptr = gt_list_copy.mutable_masks();

    sort(test_mask_ptr->begin(), test_mask_ptr->end(), ImageMaskComparator);
    sort(gt_mask_ptr->begin(), gt_mask_ptr->end(), ImageMaskComparator);

    double total_prec = 0.0, total_recall = 0.0;

    for (size_t i = 0; i < n; i++) {
      double local_prec, local_recall;
      JudgeOne(test_mask_ptr->Get(i), gt_mask_ptr->Get(i),
               &local_prec, &local_recall);
      total_prec += local_prec;
      total_recall += local_recall;
    }
    *prec = total_prec / n;
    *recall = total_recall / n;
  }
};

static bool ImageMaskComparator(const ImageMask& img1,
                                const ImageMask& img2) {
  return img1.image_id() < img2.image_id();
}

class GrabCutTestServiceImpl {
 public:
  TestResponse JudgeList(const ImageMaskList& candidate_list,
                         const ImageMaskList& gt_list) throw (){
    double precision, recall;
    TestResponse response;
    try
    {
      GrabcutMaskJudge::JudgeList(candidate_list, 
                                  gt_list, &precision, &recall);

      if (gt_list.has_image_set_id()) {
        response.set_image_set_id(gt_list.image_set_id());
      }
      response.set_code(GrabCut::TestResponse_ResponseCode_OK);
      response.set_precision(precision);
      response.set_recall(recall);      
    } catch (exception& e) {
      response.set_code(GrabCut::TestResponse_ResponseCode_ERROR);
      response.set_error_msg(e.what());
    } catch (...) {
      response.set_code(GrabCut::TestResponse_ResponseCode_ERROR);  
    }
    
    return response;
  }
};

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  RCF::RcfInitDeinit rcf_init;
  RCF::RcfServer server(RCF::TcpEndpoint(static_cast<int>(FLAGS_port)));

  RCF::ThreadPoolPtr poolptr(new RCF::ThreadPool(10));
  server.setThreadPool(poolptr);

  GrabCutTestServiceImpl gc_test;
  server.bind<I_GrabCutTest>(gc_test);
  server.start();

  while (true) sleep(10000000);
}
