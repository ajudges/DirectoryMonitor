#include "ConfParser.h"
#include "DeletePolicy.h"
#include "DirControlPolicy.h"

#include <fstream>
#include <future>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;

struct dirPolicy { // for initialized directory configurations
  std::unique_ptr<string> path;
  std::unique_ptr<DirControlPolicy> controlPolicy;
  std::unique_ptr<DeletePolicy> cleanUpPolicy;
  std::unique_ptr<vector<string>> excludeFiles;
};

struct inputConf { // to store configurations from file
  string path;
  controlPolicyType dirControlPolicy{controlPolicyType::undefined}; // define with undefined to predict behavior
  double dirControlPolicyValue{100}; // default of 100 mb in size or number of content
  deletePolicyType dirDeletepolicy{deletePolicyType::ALL}; // default of delete all files
  vector<string> dirExcludeSome;
};

// Read configuration file
void ReadConf(vector<inputConf> &confs) {

  std::ifstream stream(ConfParser::confFile); //
  if (stream.is_open()) {
    string key;
    string line;
    string delimiter;
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
      if (ConfParser::ignore.find(key) !=
          std::string::npos) { // skip lines that don't contain any keys
        continue;
      }

      // end of a configuration;
      // push old configuration to container and create a new one
      else if (key == ConfParser::endOfConf) {
        confs.emplace_back(std::move(*configuration));
        delete configuration;
        configuration = new inputConf;
        continue;
      }

      // Get directory to monitor
      else if (key == ConfParser::pathKey) {
        while (linestream >> buffer) {
          pathValue =
              (pathValue.empty())
                  ? buffer
                  : pathValue +=
                    " " + buffer; // take care of empty space in folder names
        }
        configuration->path = pathValue;
        cout << "pathValue is: " << pathValue << std::endl;
      }

      // Get directory control policy
      else if (key == ConfParser::controlPolicyKey) {
        linestream >> controlPolicyType >> controlPolicyValue;
        if (controlPolicyType == ConfParser::maxSizeInMb) {
          configuration->dirControlPolicy = controlPolicyType::max_size_in_mb;
        } else if (controlPolicyType == ConfParser::maxNumOfContent) {
          configuration->dirControlPolicy =
              controlPolicyType::max_num_of_content;
        }
        configuration->dirControlPolicyValue = controlPolicyValue;
      }

      // Get directory clean up/delete policy
      else if (key == ConfParser::deletePolicyKey) {
        linestream >> deletePolicyType;
        if (deletePolicyType == ConfParser::excludeSome) {
          configuration->dirDeletepolicy = deletePolicyType::exclude;
          while (linestream >> buffer) {
            deletePolicyValue.emplace_back(buffer);
            cout << "Excluding `" << buffer <<"` from being deleted" << std::endl;
          }
          configuration->dirExcludeSome = deletePolicyValue;
        }
      }
    }
    delete configuration; // prevent dangling pointers
    configuration = nullptr;
  }
}

int main() {
  vector<inputConf> confs; // container to hold configurations
  ReadConf(confs);

  vector<dirPolicy> directories; // container to hold initialized directory configurations 
  for (auto &configuration : confs) {
    if (fs::is_directory(configuration.path) && configuration.dirControlPolicy != undefined) {
      cout << "Path ,," << configuration.path << "´´ exists" << " and " << " control policy is properly defined"
           << std::endl;
      DirControlPolicy controlPolicy(configuration.dirControlPolicy,
                                     configuration.dirControlPolicyValue);
      DeletePolicy cleanUpPolicy =
          (configuration.dirDeletepolicy == deletePolicyType::exclude) // check if delete policy requires some files to exclude
              ? DeletePolicy(configuration.dirDeletepolicy,
                             configuration.dirExcludeSome)   // construct DeletePolicy with exclude
              : DeletePolicy(configuration.dirDeletepolicy); // construct DeletePolicy without exclude
      dirPolicy newDirPolicy;
      newDirPolicy.path = std::make_unique<string>(std::move(configuration.path));
      newDirPolicy.controlPolicy =
          std::make_unique<DirControlPolicy>(std::move(controlPolicy));
      newDirPolicy.cleanUpPolicy =
          std::make_unique<DeletePolicy>(std::move(cleanUpPolicy));
      directories.emplace_back(std::move(newDirPolicy));
    } else {
      cout << "Path ,," << configuration.path << "´´ does not exist" << " or " << " control policy not properly defined"
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

          // clean up in a parallel execution
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
