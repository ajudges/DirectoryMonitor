#pragma once
#include "CleanUpPolicy.h"
#include "DirectoryPolicy.h"

#include <string>
#include <filesystem>
#include <iostream>

using std::string;
using std::cout;
namespace fs = std::filesystem;

class Path
{
private:
    /* data */
    string _path;
public:
    Path(string path);
    ~Path();
    bool IsDir();
};
