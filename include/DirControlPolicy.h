#pragma once
#include <string>

using std::string;

enum controlPolicyType { max_size_in_mb, max_num_of_content };

class DirControlPolicy {
private:
  controlPolicyType _type;
  double _value;

public:
  DirControlPolicy(controlPolicyType policy, double value);
  bool isAgreement(string &path) const;
  ~DirControlPolicy();
};
