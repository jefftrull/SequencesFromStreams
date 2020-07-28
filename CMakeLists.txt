# Build control file for stream sequence examples
# Usage: mkdir build;cd build;cmake -DRANGE_V3=<path> -DBOOST_ROOT=<path> ..;make

cmake_minimum_required( VERSION 3.13 )

project( SequencesFromStreams )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_CXX_STANDARD 17 )

# dependencies
find_package( Boost 1.63 REQUIRED COMPONENTS context )

if (NOT DEFINED RANGE_V3)
  message(FATAL_ERROR "please supply a path to the range V3 library with -DRANGE_V3=<some-path>")
endif()
add_library( range-v3 INTERFACE )
target_include_directories( range-v3 INTERFACE ${RANGE_V3}/include )

if (CMAKE_COMPILER_IS_GNUCC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  add_definitions(-Wall -Wextra -Werror)  # be conservative about checks
  set( CMAKE_CXX_STANDARD 14 )
else()
  # actually not known to work on any other compilers (MSVC!)
  message(FATAL_ERROR "please use gcc or clang to compile these examples")
endif()

# define targets
foreach( EXAMPLE
         plain_istream_iterator plain_istream_range )
  add_executable( ${EXAMPLE} ${EXAMPLE}.cpp )
endforeach()
target_link_libraries( plain_istream_range range-v3 )

add_executable( custom_stream_operator custom_stream_operator.cpp nmea.cpp )
target_link_libraries( custom_stream_operator Boost::boost range-v3 )

foreach( EXAMPLE
    spirit_stream_operator spirit_range_facade spirit_coroutine )
  add_executable( ${EXAMPLE} ${EXAMPLE}.cpp )
endforeach()

target_link_libraries( spirit_range_facade Boost::boost range-v3 )
target_link_libraries( spirit_stream_operator Boost::boost range-v3 )

target_link_libraries( spirit_coroutine Boost::context )
