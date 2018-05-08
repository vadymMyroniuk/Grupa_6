cmake_minimum_required(VERSION 2.8)
project(server)

#includowanie z RootProject
include_directories(${server_SOURCE_DIR}/includes)

#okreslenie polozenia plikow source
set(PROJECT_SOURCE_DIR ${server_SOURCE_DIR}/sources)

#zrobienie listy source'ow
set(MAIN_SRCS
${PROJECT_SOURCE_DIR}/main.cpp
)

#skladanie executable
add_executable(server ${MAIN_SRCS})

#linkowanie z biblioteka dlLib
target_link_libraries(server ${CMAKE_DL_LIBS}) #dlLib

#wlaczanie C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -O0 -g -std=c++11")

#instalowanie w folderze /exec/
install(TARGETS server 
ARCHIVE DESTINATION ${server_SOURCE_DIR}/exec/
LIBRARY DESTINATION ${server_SOURCE_DIR}/exec/
RUNTIME DESTINATION ${server_SOURCE_DIR}/exec/)

