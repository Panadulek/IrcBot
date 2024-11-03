# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("irc_bot")

include(ExternalProject)
set(3RD_PARTY "${CMAKE_SOURCE_DIR}/3rdparty")
ExternalProject_Add(
    boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.82.0  
    CMAKE_ARGS 
        -DBOOST_INCLUDE_LIBRARIES=asio;system 
        -DBOOST_ENABLE_CMAKE=ON
        -DCMAKE_INSTALL_PREFIX=${3RD_PARTY}/boost  
    UPDATE_COMMAND "" 
    INSTALL_COMMAND "${CMAKE_COMMAND}" --install . --prefix "${3RD_PARTY}/boost" 
)


set(BOOST_ROOT ${3RD_PARTY}/boost)

set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib)      


# Uwzględnij podprojekty.
add_subdirectory ("DuIrcLib")
add_subdirectory("IrcExample")