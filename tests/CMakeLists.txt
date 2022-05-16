add_executable(UTestDirControlPolicy UTestDirectoryPolicy.cpp ../src/DirControlPolicy.cpp)
add_executable(UTestDeletePolicy UTestDeletePolicy.cpp ../src/DeletePolicy.cpp)
add_test(NAME TestDirControlPolicy COMMAND UTestDirControlPolicy)
add_test(NAME TestDeletePolicy COMMAND UTestDeletePolicy)