#ifndef GRABCUTTESTCLIENT_H
#define GRABCUTTESTCLIENT_H 

#include <RCF/RCF.hpp>

#include <string>
#include <boost/scoped_ptr.hpp>
#include "grabcut_test.pb.h"
#include "macros.h"

using namespace GrabCut;
using boost::scoped_ptr;

#include "grabcut_test_service.rcf"

class GrabCutTestClient {
 public:
  explicit GrabCutTestClient(int port);
  explicit GrabCutTestClient(std::string ip, int port);

  std::string JudgeList(const std::string& mask_proto,
                        const std::string& bench_mask_proto);

  std::string JudgeOne(const std::string& mask_proto,
                       const std::string& bench_mask_proto);

 private:
  scoped_ptr<RcfClient<I_GrabCutTest> > client_;
  RCF::RcfInitDeinit rcf_init_;

  DISALLOW_COPY_AND_ASSIGN(GrabCutTestClient);
};

#endif /* GRABCUTTESTCLIENT_H */
