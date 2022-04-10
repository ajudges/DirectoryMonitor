#include "DirCleanUpPolicy.h"
#include <filesystem>
#include <iostream>
#include <numeric>

using std::accumulate;
using std::cout;
namespace fs = std::filesystem;

DirCleanUpPolicy::DirCleanUpPolicy(cleanUpPolicyType policy, vector<string> exclude)
    : _policy(policy) {
  if (_policy == cleanUpPolicyType::exclude)
    _exclude = exclude;
}

DirCleanUpPolicy::DirCleanUpPolicy(cleanUpPolicyType policy) : _policy(policy) {}

DirCleanUpPolicy::~DirCleanUpPolicy() {}

void DirCleanUpPolicy::CleanUp(string &dir) {
  int count(0);
  auto exclude = std::make_shared<vector<string>>(_exclude);
  auto delCount = accumulate(
      fs::recursive_directory_iterator(dir), fs::recursive_directory_iterator(),
      0, [&count, dir, exclude](auto &sz, auto &entry) {
        string filename = entry.path().filename();
        if (std::find(exclude->begin(), exclude->end(), filename) ==
            exclude->end()) {
          if (fs::remove(fs::path(dir) / filename) == true) {
            ++count;
            std::cout << "Deleted  " << filename << std::endl;
          }
        }
        return count;
      });
  exclude = nullptr;
  std::cout << "Deleted " << delCount << " files" << std::endl;
}
