cmake_minimum_required(VERSION 3.8)
project(database)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage")

include_directories(include)
add_library(workers_static STATIC  
    src/consistent_search.c
    include/workers.h
    src/workers.c
    include/consistent_search.h)

add_library(workers_dynamic SHARED
    src/parallel_search.c
    include/workers.h
    src/workers.c
    include/parallel_search.h)
    
add_executable(stress_test tests/stress_test.c)
target_link_libraries(stress_test workers_static)
target_link_libraries(stress_test workers_dynamic ${CMAKE_DL_LIBS})

enable_testing()

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
message(${GTEST_LIBRARIES})
add_executable(test_static
    tests/gtests.cpp)
target_link_libraries(test_static workers_static)
target_link_libraries(test_static gtest ${CMAKE_THREAD_LIBS_INIT})

add_executable(test_dynamic
    tests/gtests.cpp)
target_link_libraries(test_dynamic workers_dynamic)
target_link_libraries(test_dynamic gtest ${CMAKE_THREAD_LIBS_INIT})
