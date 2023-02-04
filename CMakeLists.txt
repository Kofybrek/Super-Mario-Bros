cmake_minimum_required(VERSION 3.16)
project(supermario)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SFML_STATIC_LIBRARIES TRUE)

file(GLOB SOURCES "Source/*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/libraries/SFML-2.5.1/include)
link_directories(${CMAKE_SOURCE_DIR}/libraries/SFML-2.5.1/lib)

add_executable(supermario ${SOURCES})

# target_link_libraries(supermario PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries(supermario PRIVATE sfml-graphics-d sfml-window-d sfml-system-d)