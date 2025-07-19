cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("LoggerLib")

add_library(LoggerLibImpl STATIC
    src/Logger.cpp
)

target_include_directories(LoggerLibImpl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)

target_link_libraries(LoggerLibImpl PUBLIC Threads::Threads)

target_compile_options(LoggerLibImpl PRIVATE -Wall)


find_package(Threads REQUIRED)

target_link_libraries(LoggerLibImpl INTERFACE Threads::Threads)
target_compile_options(LoggerLibImpl INTERFACE -Wall)

if(MSVC)
    add_compile_options(/Qspectre-)
endif()

# --- TESTING ---

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(LoggerTests
    tests/test_logger.cpp
)

target_link_libraries(LoggerTests PRIVATE LoggerLibImpl gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(LoggerTests)