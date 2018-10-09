cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project (FifoTest)
find_package (Threads)
add_executable(FifoTest
    Src/fifoTests.cpp)
target_include_directories(FifoTest PUBLIC Catch2)

add_executable(FifoThreadTest
    Src/main.cpp)
target_include_directories(FifoThreadTest PUBLIC Catch2)
target_link_libraries (FifoThreadTest ${CMAKE_THREAD_LIBS_INIT})
