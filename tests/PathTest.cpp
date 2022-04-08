#include "../include/Path.h"
#include <fstream>

namespace fs = std::filesystem;

void test_is_dir() {
    const fs::path sandbox{"sandbox"};
    fs::create_directory(sandbox);
    auto my_path = Path("sandbox");

    assert(my_path.IsDir() == true);
    fs::remove("sandbox");
    std::cout << "isPath test passed" << std::endl;
}

int main () {
    test_is_dir();   
}