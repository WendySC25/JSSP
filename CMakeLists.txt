cmake_minimum_required(VERSION 3.14)
project(jssp-mbo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(jssp_core
    src/jssp/InstanceJSSP.cpp
    src/jssp/CostFunction.cpp
    src/jssp/SolutionJSSP.cpp
    src/mbo/MBO.cpp
)

target_include_directories(jssp_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(jssp_runner src/main.cpp)
target_link_libraries(jssp_runner PRIVATE jssp_core)

add_executable(jssp_test 
    test/testInstanceJSSP.cpp
    test/testCostFunction.cpp
)
target_link_libraries(jssp_test PRIVATE jssp_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(jssp_test)



