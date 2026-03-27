cmake_minimum_required(VERSION 3.10)
project(XaleDb VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_program(OPENSSL_EXECUTABLE openssl REQUIRED)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(DBG_DIR ${CMAKE_SOURCE_DIR}/apps/debug)
set(SRV_DIR ${CMAKE_SOURCE_DIR}/apps/server)
set(CLI_DIR ${CMAKE_SOURCE_DIR}/apps/cli)
set(TST_DIR ${CMAKE_SOURCE_DIR}/tests)

set(CERT_FILE "${CMAKE_CURRENT_BINARY_DIR}/server_cert.pem")
set(KEY_FILE "${CMAKE_CURRENT_BINARY_DIR}/server_key.pem")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE SERVER "${SRV_DIR}/*.cpp")
file(GLOB_RECURSE CLI "${CLI_DIR}/*.cpp")
file(GLOB_RECURSE TEST "${TST_DIR}/*.cpp")
file(GLOB_RECURSE DEBUG "${DBG_DIR}/*.cpp")

FetchContent_Declare(xale-logger
    GIT_REPOSITORY https://github.com/axdelafuen/xale-logger.git
    GIT_TAG v1.1.1)
FetchContent_MakeAvailable(xale-logger)

# Core
add_library(xale-db-core ${SOURCES})
target_include_directories(xale-db-core PUBLIC ${INCLUDE_DIR})
target_link_libraries(xale-db-core PUBLIC xale-logger)
if(WIN32)
    target_link_libraries(xale-db-core PUBLIC ws2_32)
endif()
add_custom_target(GenerateSSLCerts DEPENDS ${CERT_FILE} ${KEY_FILE})
add_dependencies(xale-db-core GenerateSSLCerts)
target_link_libraries(xale-db-core PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Debug
add_executable(xale-db-debug ${DEBUG})
target_include_directories(xale-db-debug PRIVATE ${INCLUDE_DIR})
target_link_libraries(xale-db-debug PRIVATE xale-db-core)

# Apps
add_executable(xale-db-server ${SERVER})
target_include_directories(xale-db-server PRIVATE ${INCLUDE_DIR})
target_link_libraries(xale-db-server PRIVATE xale-db-core)
add_executable(xale-db-cli ${CLI})
target_include_directories(xale-db-cli PRIVATE ${INCLUDE_DIR})
target_link_libraries(xale-db-cli PRIVATE xale-db-core)

# Tests
add_executable(xale-db-tests ${TEST})
target_include_directories(xale-db-tests PRIVATE ${INCLUDE_DIR} ${TST_DIR})
target_link_libraries(xale-db-tests PRIVATE xale-db-core)

if(UNIX)
    target_compile_options(xale-db-debug PRIVATE -Wall -Wextra -pthread)
endif()

add_custom_command(
    TARGET xale-db-core
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/appconfig.json
            $<TARGET_FILE_DIR:xale-db-core>/appconfig.json
)

add_custom_command(
    OUTPUT ${CERT_FILE} ${KEY_FILE}
    COMMAND ${OPENSSL_EXECUTABLE} req -x509 -newkey rsa:4096 
            -keyout ${KEY_FILE} -out ${CERT_FILE} 
            -days 365 -nodes -subj "/C=FR/ST=Local/L=Local/O=Dev/CN=localhost"
    COMMENT "Generating debug SSL certificates..."
    VERBATIM
)