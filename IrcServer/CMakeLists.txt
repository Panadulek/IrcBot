project("DuServer")

add_executable(DuServer "src/main.cpp")

set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)
set(DU_IRC_PATH_LIB ${CMAKE_SOURCE_DIR}/DuIrcLib/include)

target_include_directories(DuServer PUBLIC ${BOOST_INCLUDEDIR} ${DU_IRC_PATH_LIB})
target_link_directories(DuServer PUBLIC ${BOOST_LIBRARYDIR})
target_link_libraries(DuServer PUBLIC Boost::asio Boost::system Boost::uuid DuIrcLib) 