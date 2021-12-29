cmake_minimum_required(VERSION 2.8.9)
project(SuperMario)

file(GLOB SOURCES "Source/*.cpp")

set(CMAKE_CXX_STANDARD 17)


#look for SFML and add it
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/SFML/cmake-modules/")
find_package(SFML REQUIRED system window graphics network audio)

include_directories(${SFML_INCLUDE_DIR})

add_executable(SuperMario ${SOURCES})

# target_link_libraries(SuperMario ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_link_libraries(SuperMario sfml-graphics sfml-audio)
