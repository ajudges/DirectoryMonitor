#include <fstream>
#include <iostream>
#include <numeric>
#include "../include/CleanUpPolicy.h"

using std::accumulate;
namespace fs = std::filesystem;

void test_clean_all() {
    string dirName{"sandbox"};
    const fs::path sandbox{dirName};
    fs::create_directory(sandbox);

    std::ofstream("sandbox/file1.txt");
    std::ofstream("sandbox/file2.txt");
    std::ofstream("sandbox/file3.txt");
    CleanUpPolicy cleanUpPolicy(cleanUpPolicyType::ALL);

    cleanUpPolicy.CleanUp(dirName);
    int count(0);
    auto fileCount =
        accumulate(fs::recursive_directory_iterator(dirName),
                   fs::recursive_directory_iterator(), 0,
                   [&count](auto &sz, auto &entry) { return ++count; });
    assert(fileCount == 0);
    fs::remove(dirName);
}

void test_exclude() {
  string dirName{"sandbox"};
  const fs::path sandbox{dirName};
  fs::create_directory(sandbox);

  std::ofstream("sandbox/file1.txt");
  std::ofstream("sandbox/file2.txt");
  std::ofstream("sandbox/file3.txt");
  std::ofstream("sandbox/file4.txt");
  vector<string> excludeTwoFiles{"file1.txt", "file2.txt"};
  CleanUpPolicy cleanUpPolicyExcludeTwo(cleanUpPolicyType::exclude, excludeTwoFiles);

  cleanUpPolicyExcludeTwo.CleanUp(dirName);
  int count(0);
    auto fileCount =
        accumulate(fs::recursive_directory_iterator(dirName),
                   fs::recursive_directory_iterator(), 0,
                   [&count](auto &sz, auto &entry) { return ++count; });
    assert(fileCount == 2);

    vector<string> excludeNone;
    CleanUpPolicy cleanUpPolicyExcludeNone(cleanUpPolicyType::exclude, excludeNone);
    cleanUpPolicyExcludeNone.CleanUp(dirName);
    count = 0;
    fileCount =
        accumulate(fs::recursive_directory_iterator(dirName),
                   fs::recursive_directory_iterator(), 0,
                   [&count](auto &sz, auto &entry) { return ++count; });
    assert(fileCount == 0);
    fs::remove_all(dirName);
}

int main () {
    test_clean_all();
    test_exclude();

}