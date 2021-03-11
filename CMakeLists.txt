cmake_minimum_required(VERSION 3.17.0)
project(vm)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.1
)
FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

target_include_directories(vm_tests PRIVATE SYSTEM
        "include")
target_link_libraries(vm_tests PRIVATE Catch2::Catch2)

target_include_directories(vm PRIVATE SYSTEM
        "include")