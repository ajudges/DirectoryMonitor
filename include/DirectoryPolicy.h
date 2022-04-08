#pragma once
#include <string>

using std::string;

enum directoryPolicyType { max_size, max_num_of_content };

class DirectoryPolicy {
private:
  directoryPolicyType _type;
  double _value;

public:
  DirectoryPolicy(directoryPolicyType policy, double value);
  bool isAgreement(string &path) const;
  ~DirectoryPolicy();
};
