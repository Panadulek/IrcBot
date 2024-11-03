cmake_minimum_required(VERSION 3.28)
project("DuIrcExample")
set(CMAKE_CXX_STANDARD 20)

add_executable(DuIrcExample src/main.cpp "include/SimpleWriter.h" "include/SimpleReader.h")

set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)
set(DU_IRC_PATH_LIB ${CMAKE_SOURCE_DIR}/DuIrcLib/include)


target_link_libraries(DuIrcExample PUBLIC DuIrcLib)


