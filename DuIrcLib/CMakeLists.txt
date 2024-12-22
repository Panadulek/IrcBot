cmake_minimum_required(VERSION 3.28)
project("DuIrcLib")
set(CMAKE_CXX_STANDARD 20)



add_library(DuIrcLib STATIC
 "src/IrcConnection.cpp" "src/IrcClient.cpp" "src/IrcProtocol.cpp"  "include/DuSession.h" "include/DuServer.h"  "src/DuServer.cpp" "include/DuCallbacks.hpp" "src/DuCallbacks.cpp"  "include/Pinger.h")



 

 set_target_properties(DuIrcLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib  
 )
 message("BOOST_INCLUDE_DIR = "${Boost_INCLUDE_DIRS})
 target_include_directories(DuIrcLib PUBLIC ${BOOST_INCLUDEDIR})
target_link_directories(DuIrcLib PUBLIC ${BOOST_LIBRARYDIR})
target_link_libraries(DuIrcLib PUBLIC Boost::asio Boost::system Boost::uuid)

