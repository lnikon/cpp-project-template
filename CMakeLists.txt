cmake_minimum_required(VERSION 3.28)
project(cpp-project-template C CXX)

# Use GCC for --ffast-math
set(CMAKE_C_COMPILER=/usr/bin/gcc)
set(CMAKE_CXX_COMPILER=/usr/bin/g++)

# Defaulted to 'ON' to always generate compile_commands.json
# Useful for LSPs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(BENCHMARK_ENABLE_LIBPFM ON CACHE BOOL "" FORCE)
endif()

# Project wide compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Commonly used packages
# Uncomment to use
# Uncomment the appropriate require directive in conanfile.txt
find_package(Boost REQUIRED)
#find_package(Catch2 REQUIRED)
#find_package(gRPC REQUIRED)
#find_package(spdlog REQUIRED)
#find_package(fmt REQUIRED)
#find_package(ZLIB REQUIRED)
#find_package(Protobuf REQUIRED)
find_package(benchmark REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)

set(TBB_TEST OFF CACHE BOOL "Do not build TBB tests" FORCE)
find_package(TBB REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Fast by default :)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Less warnings
set(CMAKE_CXX_FLAGS_RELEASE
  "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-but-set-variable -g")

# Stable results between recompilations
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -falign-functions=32")

# An example target
# main
set(
  sources
  "src/main.cpp"
)

add_executable(main ${sources})
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(main pthread benchmark OpenMP::OpenMP onetbb::onetbb)

# eytz_algorithmica
set(
  sources_eytz_algorithmica
  "src/eytz_algorithmica.cpp"
)

add_executable(eytz_algorithmica ${sources_eytz_algorithmica})
target_include_directories(eytz_algorithmica PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(eytz_algorithmica pthread benchmark OpenMP::OpenMP onetbb::onetbb)

# boost_eytz
set(
  sources_boost_eytz
  "src/boost_eytz.cpp"
)

add_executable(boost_eytz ${sources_boost_eytz})
target_include_directories(boost_eytz PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(boost_eytz Boost::boost pthread benchmark OpenMP::OpenMP onetbb::onetbb)
