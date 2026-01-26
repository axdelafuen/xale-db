cmake_minimum_required(VERSION 3.10)
project(XaleDb VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SRV_DIR ${CMAKE_SOURCE_DIR}/apps/server)
set(CLI_DIR ${CMAKE_SOURCE_DIR}/apps/cli)

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE SERVER_EXE "${SRV_DIR}/*.cpp")
file(GLOB_RECURSE CLI_EXE "${CLI_DIR}/*.cpp")

FetchContent_Declare(xale-logger
    GIT_REPOSITORY https://github.com/axdelafuen/xale-logger.git
    GIT_TAG v1.1.1)
FetchContent_MakeAvailable(xale-logger)

add_executable(xale-db-debug ${SOURCES})
add_executable(xale-db-server ${SERVER_EXE})
add_executable(xale-db-cli ${CLI_EXE})

target_link_libraries(xale-db-debug PRIVATE xale-logger)
target_include_directories(xale-db-debug PRIVATE ${INCLUDE_DIR})

if(UNIX)
    target_compile_options(xale-db-debug PRIVATE -Wall -Wextra -pthread)
endif()

add_custom_command(
    TARGET xale-db-debug
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/appconfig.json
            $<TARGET_FILE_DIR:xale-db-debug>/appconfig.json
)
