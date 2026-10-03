cmake_minimum_required(VERSION 3.10)
project(IntegrationCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Automatically grab ALL C++ source files in the root folder dynamically
file(GLOB SOURCES "*.cpp")

# 2. Build target binary executable using the dynamically gathered list
add_executable(Calculator ${SOURCES})

# 3. Locate multi-threading libraries required by Crow
find_package(Threads REQUIRED)

# Link networking libraries
if(WIN32)
    target_link_libraries(Calculator PRIVATE Threads::Threads ws2_32 mswsock)
else()
    target_link_libraries(Calculator PRIVATE Threads::Threads)
endif()

# 4. Include base directory path configurations
target_include_directories(Calculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Calculator SYSTEM BEFORE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
