cmake_minimum_required(VERSION 3.10)
project(IntegrationCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Dynamically find all source files in the current root folder
file(GLOB SOURCES "*.cpp")

# 2. Build target binary executable
add_executable(Calculator ${SOURCES})

# 3. Locate multi-threading libraries required by Crow
find_package(Threads REQUIRED)

# Link networking libraries
if(WIN32)
    target_link_libraries(Calculator PRIVATE Threads::Threads ws2_32 mswsock)
else()
    target_link_libraries(Calculator PRIVATE Threads::Threads)
endif()

# 4. Include the base root directory for header searching
target_include_directories(Calculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
