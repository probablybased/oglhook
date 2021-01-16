cmake_minimum_required(VERSION 3.17)
project(oglhook)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
    )
endif()

link_directories(PolyHook_2_0/_install/lib/)

add_library(oglhook SHARED main.cpp)
target_link_libraries(oglhook PRIVATE PolyHook_2 capstone opengl32)
target_include_directories(oglhook PRIVATE "PolyHook_2_0/_install/include")