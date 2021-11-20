cmake_minimum_required(VERSION 3.20)
project(mario)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR /usr/local/include/SFML)
find_package(SFML REQUIRED COMPONENTS audio network graphics window system)
if (NOT SFML_FOUND)
    message(FATAL_ERROR "SFML NOT FOUND!")
else()
    message("SFML FOUND!")
endif()
include_directories(${SFML_DIR})
file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/Headers/*.hpp"
        "${PROJECT_SOURCE_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/Headers/*.mm"
    )
add_executable(mario ${SRC_FILES})
# copy Resources after
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Resources"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/Resources"
)

target_link_libraries(mario sfml-audio sfml-network sfml-graphics sfml-window sfml-system)
