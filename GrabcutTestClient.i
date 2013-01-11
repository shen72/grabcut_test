%module GrabcutTestClient
%include "std_string.i"

%{
#include "GrabcutTestClient.h"
%}

class GrabCutTestClient {
public:
  explicit GrabCutTestClient(int port);
  explicit GrabCutTestClient(std::string ip, int port);

  std::string JudgeList(const std::string& mask_proto,
                        const std::string& bench_mask_proto);
  std::string JudgeOne(const std::string& mask_proto,
                       const std::string& bench_mask_proto);
};
