cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("irc_bot")


set(3RD_PARTY "${CMAKE_SOURCE_DIR}/3rdparty")
message("3rd = ${3RD_PARTY}")
include(FetchContent)
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.83.0 # Wybierz odpowiednią wersję Boost
)
#
set(FETCHCONTENT_QUIET OFF)
set(BOOST_ENABLE_CMAKE ON CACHE BOOL "Enable CMake support for Boost")
set(BOOST_WITH_SYSTEM ON CACHE BOOL "Enable Boost.System")
set(BOOST_WITH_UUID ON CACHE BOOL "Enable Boost.UUID")
set(BOOST_BUILD_TESTS OFF CACHE BOOL "Disable Boost tests")
FetchContent_MakeAvailable(Boost)


# Informowanie CMake o lokalizacji Boost
set(BOOST_ROOT ${Boost_SOURCE_DIR})
set(BOOST_INCLUDEDIR ${Boost_SOURCE_DIR})
set(BOOST_LIBRARYDIR ${Boost_BINARY_DIR}/stage/lib)
set(DULIB_INCLUDEDIR "DuIrcLib/include/")
message("BOOST_ROOT =" ${BOOST_ROOT} )
message("BOOST_LIBRARY_DIR = " ${BOOST_LIBRARYDIR})

add_subdirectory ("DuIrcLib")
add_subdirectory("IrcExample")
add_subdirectory("IrcServer")

