#include "CleanUpPolicy.h"
#include "DirectoryPolicy.h"
#include "Path.h"
#include <thread>

void GetInput(string user_input, string &x) {
  std::cout << "Enter " << user_input << ": " << std::endl;
  std::cin >> x;

  std::cout << "Accepted input value " << x << std::endl;
  return;
}

int main() {
  string dir_path;
  GetInput("path to monitor", dir_path);

  // Create path object
  Path my_path(dir_path);
  DirectoryPolicy myDirPolicy(directoryPolicyType::max_num_of_content, 2);
  CleanUpPolicy myCleanUpPolicy(cleanUpPolicyType::ALL);
  if (my_path.IsDir()) {
    cout << "Path exists " << std::endl;
    while (true) {
      // check if path is in agreement with monitoring policy
      if (!myDirPolicy.isAgreement(dir_path))
      {
          std::cout << "Directory policy is breached, cleaning up..." << std::endl;
          myCleanUpPolicy.CleanUp(dir_path);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
  }
  else {
    std::cout << "Path does not exist" << std::endl;
  }
}
