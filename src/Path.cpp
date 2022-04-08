#include "Path.h"

Path::Path(string path) : _path(path)
{
}

Path::~Path()
{
}

bool Path::IsDir() {
    return fs::is_directory(_path);
}
