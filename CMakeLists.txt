cmake_minimum_required(VERSION 3.22.1)
project(SimpleRayTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -fsanitize=address")


find_package(TBB REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(SimpleRayTracer main.cpp)

target_link_libraries(SimpleRayTracer TBB::tbb)