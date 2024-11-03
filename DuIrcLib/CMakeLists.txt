cmake_minimum_required(VERSION 3.28)
project("DuIrcLib")
set(CMAKE_CXX_STANDARD 20)
find_package(Boost 1.82 REQUIRED COMPONENTS asio system)


add_library(DuIrcLib STATIC
 "src/IrcConnection.cpp" "src/IrcClient.cpp" "src/IrcProtocol.cpp")

 set_target_properties(DuIrcLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib  
 )

target_link_libraries(DuIrcLib PUBLIC Boost::asio Boost::system)
target_include_directories(DuIrcLib PUBLIC include ${Boost_INCLUDE_DIRS})
