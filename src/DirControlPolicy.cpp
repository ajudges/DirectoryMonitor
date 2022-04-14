#include "DirControlPolicy.h"
#include <filesystem>
#include <iostream>
#include <numeric>

using std::accumulate;
using std::cout;

DirControlPolicy::DirControlPolicy(controlPolicyType policy, double value)
    : _type(policy), _value(value) {}

DirControlPolicy::~DirControlPolicy() {}

// implement check if directory is in agreement with policy
bool DirControlPolicy::isAgreement(string &path) const {
  if (_type == controlPolicyType::max_num_of_content) {
    // check if max content is reached
    int count(0);
    auto fileCount =
        accumulate(fs::recursive_directory_iterator(path),
                   fs::recursive_directory_iterator(), 0,
                   [&count](auto &sz, auto &entry) { return ++count; });
    return fileCount <= _value;
  }

  if (_type == controlPolicyType::max_size_in_mb) {
    // check if max size is reached
    double dirSize = accumulate(
        fs::recursive_directory_iterator(path),
        fs::recursive_directory_iterator(), 0, [](auto sz, auto entry) {
          return is_directory(entry) ? sz : sz + file_size(entry);
        });
    dirSize /= (1024. * 1024.); // convert to MB
    dirSize = std::ceil(dirSize * 10.) / 10.;
    return dirSize <= _value;
  }

  return true;
}
