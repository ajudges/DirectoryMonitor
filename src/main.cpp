#include "ConfParser.h"
#include "DeletePolicy.h"
#include "DirControlPolicy.h"

#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
namespace fs = std::filesystem;

struct dirPolicy {
  std::unique_ptr<string> path;
  std::unique_ptr<DirControlPolicy> controlPolicy;
  std::unique_ptr<DeletePolicy> cleanUpPolicy;
  std::unique_ptr<vector<string>> excludeFiles;
};

struct inputConf {
  string path;
  controlPolicyType dirControlPolicy;
  double dirControlPolicyValue;
  deletePolicyType dirDeletepolicy;
  vector<string> dirExcludeSome;
};

// Read configuration file
// void ReadConf(string user_input, string &x) {
void ReadConf(vector<inputConf> &confs) {

  std::ifstream stream(ConfParser::confFile);
  if (stream.is_open()) {
    cout << "confs path exists" << std::endl;
    string key;
    string line;
    string delimiter;
    string ignore{"{[]"};
    string value;
    string pathValue;
    string controlPolicyType;
    double controlPolicyValue;
    string deletePolicyType;
    vector<string> deletePolicyValue;
    string buffer;
    inputConf *configuration = new inputConf;
    while (std::getline(stream, line)) {
      pathValue = ""; // prevent concatenation of multiple paths
      std::replace(line.begin(), line.end(), '"', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if (ignore.find(key) != std::string::npos) {
        cout << "Key is... " << key << std::endl;
        continue;
      }
      if (key == "}") {
        confs.emplace_back(std::move(*configuration));
        configuration = new inputConf;
      }
      if (key == ConfParser::pathKey) {
        while (linestream >> buffer) {
          if (pathValue.empty()) {
            pathValue = buffer;
          } else { // take care of empty space in folder names
            pathValue += " ";
            pathValue += buffer;
          }
        }
        configuration->path = pathValue;
        cout << "pathValue is: " << pathValue << std::endl;
      }
      if (key == ConfParser::controlPolicyKey) {
        linestream >> controlPolicyType >> controlPolicyValue;
        cout << "controlPolicyType is: " << controlPolicyType << std::endl;
        cout << "controlPolicyValue is: " << controlPolicyValue << std::endl;
        if (controlPolicyType == ConfParser::maxSizeInMb) {
          configuration->dirControlPolicy = controlPolicyType::max_size_in_mb;
        } else if (controlPolicyType == ConfParser::maxNumOfContent) {
          configuration->dirControlPolicy =
              controlPolicyType::max_num_of_content;
        }
        configuration->dirControlPolicyValue = controlPolicyValue;
      }
      if (key == ConfParser::deletePolicyKey) {
        linestream >> deletePolicyType;
        cout << "deletePolicyType is: " << deletePolicyType << std::endl;
        if (deletePolicyType == ConfParser::delAll) {
          configuration->dirDeletepolicy = deletePolicyType::ALL;
        } else if (deletePolicyType == ConfParser::excludeSome) {
          configuration->dirDeletepolicy = deletePolicyType::exclude;
          while (linestream >> buffer) {
            deletePolicyValue.emplace_back(buffer);
            cout << "buffer is: " << buffer << std::endl;
          }
          configuration->dirExcludeSome = deletePolicyValue;
        }
      }
    }
    delete configuration;
    configuration = nullptr;
  }
}

void GetInput(string user_input, string &x) {
  cout << "Enter " << user_input << ": " << std::endl;
  std::cin >> x;

  std::cout << "Accepted input value " << x << std::endl;
  return;
}

int main() {
  vector<inputConf> confs;
  ReadConf(confs);

  vector<dirPolicy> directories;
  for (auto &configuration : confs) {
    if (fs::is_directory(configuration.path)) {
      DirControlPolicy controlPolicy(configuration.dirControlPolicy,
                                     configuration.dirControlPolicyValue);
      DeletePolicy cleanUpPolicy =
          (configuration.dirDeletepolicy == deletePolicyType::exclude)
              ? DeletePolicy(configuration.dirDeletepolicy,
                             configuration.dirExcludeSome)
              : DeletePolicy(configuration.dirDeletepolicy);
      dirPolicy testEvery;
      testEvery.path = std::make_unique<string>(std::move(configuration.path));
      testEvery.controlPolicy =
          std::make_unique<DirControlPolicy>(std::move(controlPolicy));
      testEvery.cleanUpPolicy =
          std::make_unique<DeletePolicy>(std::move(cleanUpPolicy));
      directories.emplace_back(std::move(testEvery));
    } else {
      cout << "Path ,," << configuration.path << "´´ does not exists"
           << std::endl;
    }
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
