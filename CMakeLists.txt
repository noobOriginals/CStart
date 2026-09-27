cmake_minimum_required(VERSION 3.31)
project(CStart VERSION 3.0.0 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
add_executable(cstart "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_compile_features(cstart PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(cstart PRIVATE
        $<$<CONFIG:Debug>:/W4>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(cstart PRIVATE
        $<$<CONFIG:Debug>:-Wextra -Wunused -Wsign-conversion -Wpedantic>
        $<$<CONFIG:Release>:-O3>
    )
endif()
