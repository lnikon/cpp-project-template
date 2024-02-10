cmake_minimum_required(VERSION 3.28)
project(cpp-project-template C CXX)

# Defaulted to 'ON' to always generate compile_commands.json
# Useful for LSPs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project wide compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Commonly used packages
# Uncomment to use
# Uncomment the appropriate require directive in conanfile.txt
find_package(Boost REQUIRED)
find_package(Catch2 REQUIRED)
find_package(gRPC REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Protobuf REQUIRED)

# An example target
add_compile_options(-Wall -Wextra -pedantic)
set(
  sources
  "src/main.cpp"
  "src/ExampleSource.cpp"
)

add_executable(main ${sources})
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(main ${CONAN_LIBS})
