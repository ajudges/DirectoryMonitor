#include "DeletePolicy.h"
#include <filesystem>
#include <iostream>
#include <numeric>
#include <experimental/filesystem>

using std::accumulate;
using std::cout;
namespace fs = std::experimental::filesystem;

DeletePolicy::DeletePolicy(deletePolicyType policy, vector<string> exclude)
    : _policy(policy) {
  _mutex = new std::mutex();
  if (_policy == deletePolicyType::exclude)
    _exclude = exclude;
}

DeletePolicy::DeletePolicy(deletePolicyType policy) : _policy(policy) {
  _mutex = new std::mutex();
}

DeletePolicy::~DeletePolicy() {
  if (_mutex != NULL)
  {
    delete _mutex;
    _mutex = NULL;
  }
  
}

// implement directory clean up
void DeletePolicy::CleanUp(string &dir) {
  int count(0);
  auto exclude = std::make_shared<vector<string>>(_exclude);
  // perform directory cleanup under lock
  std::lock_guard<std::mutex> uLock(*_mutex);
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

// copy constructor
DeletePolicy::DeletePolicy(const DeletePolicy &source)
{
  _mutex = new std::mutex();
  _policy = source._policy;
  _exclude = source._exclude;
}

// copy assignment operator
DeletePolicy &DeletePolicy::operator=(const DeletePolicy &source)
{
  if (this == &source) {
    return *this;
  }
  delete _mutex;
  _mutex = new std::mutex();
  _mutex = source._mutex;
  _policy = source._policy;
  _exclude = source._exclude;
  return *this;
}

// move constructor
DeletePolicy::DeletePolicy(DeletePolicy &&source) {
  _mutex = source._mutex;
  _policy = source._policy;
  _exclude = source._exclude;
  source._mutex = NULL;
  source._policy = deletePolicyType();
  source._exclude = vector<string>();
}

// assignment move constructor
DeletePolicy &DeletePolicy::operator=(DeletePolicy &&source) {
  if (this == &source) {
    return *this;
  }

  _mutex = source._mutex;
  _policy = source._policy;
  _exclude = source._exclude;
  source._mutex = NULL;
  source._policy = deletePolicyType();
  source._exclude = vector<string>();
  return *this;
}