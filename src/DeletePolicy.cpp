#include "DeletePolicy.h"
#include <filesystem>
#include <iostream>
#include <numeric>

using std::accumulate;
using std::cout;
namespace fs = std::filesystem;

DeletePolicy::DeletePolicy(deletePolicyType policy, vector<string> exclude)
    : _policy(policy) {
  if (_policy == deletePolicyType::exclude)
    _exclude = exclude;
}

DeletePolicy::DeletePolicy(deletePolicyType policy) : _policy(policy) {}

DeletePolicy::~DeletePolicy() {}

void DeletePolicy::CleanUp(string &dir) {
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
