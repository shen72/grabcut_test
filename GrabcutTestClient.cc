#include "GrabcutTestClient.h"

using namespace std;

GrabCutTestClient::GrabCutTestClient(int port) {
  new (this)GrabCutTestClient("127.0.0.1", port);
}

GrabCutTestClient::GrabCutTestClient(string ip, int port) {
  client_.reset(new RcfClient<I_GrabCutTest>(RCF::TcpEndpoint(ip, port)));
}

string GrabCutTestClient::JudgeList(const string& mask_proto,
                                    const string& bench_mask_proto) {
  ImageMaskList mask_list, bench_mask_list;
  mask_list.ParseFromString(mask_proto);
  bench_mask_list.ParseFromString(bench_mask_proto);

  const TestResponse& response = client_->JudgeList(
      mask_list, bench_mask_list);

  return response.SerializeAsString();
}

string GrabCutTestClient::JudgeOne(const string& mask_proto,
                                   const string& bench_mask_proto) {
  ImageMask mask, bench_mask;
  CHECK(mask.ParseFromString(mask_proto));
  CHECK(bench_mask.ParseFromString(bench_mask_proto));

  CHECK(client_.get());
  const TestResponse& response = client_->JudgeOne(mask, bench_mask);

  return response.SerializeAsString();
}

