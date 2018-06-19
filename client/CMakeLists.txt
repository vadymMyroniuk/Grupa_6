cmake_minimum_required(VERSION 2.8)
project(client)

#includowanie z RootProject
include_directories(${client_SOURCE_DIR}/includes)

#okreslenie polozenia plikow source
set(PROJECT_SOURCE_DIR ${client_SOURCE_DIR}/sources)

#zrobienie listy source'ow
set(MAIN_SRCS
${PROJECT_SOURCE_DIR}/client.cpp
)

#skladanie executable
add_executable(client ${MAIN_SRCS})

#linkowanie z biblioteka dlLib
target_link_libraries(client ${CMAKE_DL_LIBS}) #dlLib

#wlaczanie C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -O0 -g -std=c++11")

#instalowanie w folderze /exec/
install(TARGETS client 
ARCHIVE DESTINATION ${client_SOURCE_DIR}/exec/
LIBRARY DESTINATION ${client_SOURCE_DIR}/exec/
RUNTIME DESTINATION ${client_SOURCE_DIR}/exec/)

