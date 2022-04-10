#include "DirCleanUpPolicy.h"
#include "DirControlPolicy.h"

#include <filesystem>
#include <future>
#include <iostream>
#include <thread>

using std::cin;
using std::cout;
namespace fs = std::filesystem;

struct dirPolicy {
  std::unique_ptr<string> path;
  std::unique_ptr<DirControlPolicy> controlPolicy;
  std::unique_ptr<DirCleanUpPolicy> cleanUpPolicy;
};

void GetInput(string user_input, string &x) {
  cout << "Enter " << user_input << ": " << std::endl;
  std::cin >> x;

  std::cout << "Accepted input value " << x << std::endl;
  return;
}

int main() {
  string dir_path;
  GetInput("path to monitor", dir_path);

  DirControlPolicy controlPolicy(controlPolicyType::max_num_of_content, 2);
  DirCleanUpPolicy cleanUpPolicy(cleanUpPolicyType::ALL);

  vector<dirPolicy> directories;
  if (fs::is_directory(dir_path)) {
    dirPolicy testEvery;
    testEvery.path = std::make_unique<string>(std::move(dir_path));
    testEvery.controlPolicy =
        std::make_unique<DirControlPolicy>(std::move(controlPolicy));
    testEvery.cleanUpPolicy =
        std::make_unique<DirCleanUpPolicy>(std::move(cleanUpPolicy));
    directories.emplace_back(std::move(testEvery));
  }
  else
  {
    cout << "Path ,," << dir_path << "´´ does not exists" << std::endl;
  }
  std::vector<std::future<void>> futures;
  if (!directories.empty()) {
    while (true) {
      for (auto &directory : directories) {
        // check if path is in agreement with monitoring policy
        if (!directory.controlPolicy->isAgreement(*directory.path)) {
          std::cout << "Directory ,," << (*directory.path)
                    << "´´ policy is breached, cleaning up... :-)" << std::endl;

          // clean up in separate thread 
          futures.emplace_back(std::async(std::launch::async, [&directory]() {
            directory.cleanUpPolicy->CleanUp(*directory.path);
          }));
        }
      }
      // wait for tasks to complete
      for (const std::future<void> &ftr : futures)
          ftr.wait();
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
  }
}
