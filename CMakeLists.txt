cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(DirectoryMonitor)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")


include_directories(include)
# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

# Add project executable
add_executable(DirectoryMonitor ${project_SRCS})
target_link_libraries(DirectoryMonitor -fsanitize=address)

# Add the testing executable
add_executable(test_DirectoryMonitor tests/PathTest.cpp src/Path.cpp)
add_executable(test_DirectoryPolicy tests/DirectoryPolicyTest.cpp src/DirectoryPolicy.cpp)
add_executable(test_CleanUpPolicy tests/CleanUpPolicyTest.cpp src/CleanUpPolicy.cpp)
