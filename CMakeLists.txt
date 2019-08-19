cmake_minimum_required(VERSION 3.5)

add_subdirectory(plyometrics)
add_subdirectory(catch2)

add_executable(packed_benchmarks packed_vector_benchmark.cpp)
target_link_libraries(packed_benchmarks plyometrics)

add_executable(packed_tests packed_vector_tests.cpp)
target_link_libraries(packed_tests Catch2::Catch2)
