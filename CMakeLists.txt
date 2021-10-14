cmake_minimum_required(VERSION 3.8)
project(schedule)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-pedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "-pedantic -fprofile-arcs -ftest-coverage")

set(LIB_SOURCE_FILES src/schedule.c)
add_Library(study_std_lib STATIC ${LIB_SOURCE_FILES})
include_directories(include)
include_directories(src)

add_executable(prog
    include/schedule.h  
    src/main.c)
target_link_libraries(prog study_std_lib)


enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
message(${GTEST_LIBRARIES})
add_executable(tests src/tests.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests study_std_lib)
