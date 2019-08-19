cmake_minimum_required(VERSION 3.5)

add_subdirectory(plyometrics)

add_executable(packed_benchmarks packed_vector_benchmark.cpp)
target_link_libraries(packed_benchmarks plyometrics)
