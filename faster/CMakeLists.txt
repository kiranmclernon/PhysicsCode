cmake_minimum_required(VERSION 3.13.4)
project(test)
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} main.cpp
            src/button.cpp)
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
