cmake_minimum_required(VERSION 3.10)
project(IntegrationCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Web server source pipeline files
set(SOURCES
    "Server.cpp"
    "lexer/lexer.cpp"
    "Parser/Parser.cpp"
    "integrator/Integrator.cpp"
    "integrator/Printer.cpp"
    "integrator/ExplanationBuilder.cpp"
)

# 2. Build target binary executable
add_executable(Calculator ${SOURCES})

# 3. Locate multi-threading libraries required by Crow
find_package(Threads REQUIRED)

# CRITICAL FIX: Link the Windows Socket networking libraries to resolve the linker errors
if(WIN32)
    target_link_libraries(Calculator PRIVATE Threads::Threads ws2_32 mswsock)
else()
    target_link_libraries(Calculator PRIVATE Threads::Threads)
endif()

# 4. Expose internal subdirectories
target_include_directories(Calculator PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser
    ${CMAKE_CURRENT_SOURCE_DIR}/integrator
)

# 5. Root folder treat as SYSTEM inclusion path
target_include_directories(Calculator SYSTEM BEFORE PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
