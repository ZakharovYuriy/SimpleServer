cmake_minimum_required(VERSION 3.11)

project(HTTPserver CXX)
set(CMAKE_CXX_STANDARD 17) 

set(SOURCE_DIR Server)

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../${SOURCE_DIR})

file(GLOB sources
  ${SOURCE_DIR}/*.cpp
  ${SOURCE_DIR}/*.h
)

add_executable(server ${sources}) 

