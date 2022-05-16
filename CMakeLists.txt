cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(DirectoryMonitor)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

include_directories(include)
# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

# link experimental::filesystem in linux
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    link_libraries(stdc++fs)  
endif(CMAKE_SYSTEM STREQUAL Linux)

# Add project executable
add_executable(DirectoryMonitor ${project_SRCS})

# check for memory leak
target_link_libraries(DirectoryMonitor -fsanitize=address)

# Add the testing executable
enable_testing()
add_subdirectory(tests)