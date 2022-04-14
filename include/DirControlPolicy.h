#pragma once
#include <string>
#include <experimental/filesystem>
#include <cmath>

using std::string;
namespace fs = std::experimental::filesystem;

enum controlPolicyType { max_size_in_mb, max_num_of_content, undefined }; // policies to check on directory

class DirControlPolicy {
private:
  controlPolicyType _type;
  double _value;

public:
  DirControlPolicy(controlPolicyType policy, double value);
  bool
  isAgreement(string &path) const; // is directory in agreement with policy?
  ~DirControlPolicy();
};
