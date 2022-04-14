#include "../include/DirControlPolicy.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <cassert>

using std::accumulate;
namespace fs = std::experimental::filesystem;
using std::cerr;

void test_max_num_of_content() {
  string dirName{"sandbox"};
  const fs::path sandbox{dirName};
  fs::create_directory(sandbox);

  std::ofstream("sandbox/file1.txt");
  std::ofstream("sandbox/file2.txt");
  std::ofstream("sandbox/file3.txt");
  DirControlPolicy dirPolicy(controlPolicyType::max_num_of_content, 2);

  assert(dirPolicy.isAgreement(dirName) == false);
  fs::remove("sandbox/file3.txt");

  assert(dirPolicy.isAgreement(dirName) == true);
  std::cout << "max num of content passed" << std::endl;
  if (fs::remove_all("sandbox") < 0)
    std::cout << "Could not remove all files in sandbox" << std::endl;
}

void test_max_size() {
  string dirName{"sandbox"};
  const fs::path sandbox{dirName};
  fs::create_directory(sandbox);
  std::ofstream file(sandbox/ "text_file.txt");
  std::ofstream (sandbox/ "text_file2.txt");

  std::mt19937 gen{ std::random_device()() };
  std::uniform_int_distribution<> dis(0, 255);
  int size = 10000000; //~10MB
  std::generate_n(std::ostream_iterator<char>(file, ""), size, [&]{ return dis(gen); });

  DirControlPolicy dirPolicy(controlPolicyType::max_size_in_mb, 2);
  assert(dirPolicy.isAgreement(dirName) == false);
  
  fs::remove(sandbox/ "text_file.txt");
  
  assert(dirPolicy.isAgreement(dirName) == true);
  std::cout << "max size test passed" << std::endl;
  if (fs::remove_all(sandbox) < 0)
    std::cout << "clean up failed" << std::endl;
}

int main () {
  test_max_size();
  test_max_num_of_content();
}