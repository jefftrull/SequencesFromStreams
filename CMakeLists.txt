# Build control file for stream sequence examples
# Usage: mkdir build;cd build;cmake -DRANGE_V3=<path> -DBOOST_ROOT=<path> ..;make

cmake_minimum_required( VERSION 2.8.9 )

# dependencies
find_package( Boost 1.58 REQUIRED )
if (NOT DEFINED RANGE_V3)
  message(FATAL_ERROR "please supply a path to the range V3 library with -DRANGE_V3=<some-path>")
endif()

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} ${RANGE_V3}/include )

if (CMAKE_COMPILER_IS_GNUCC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  add_definitions(-Wall -Wextra -Werror)  # be conservative about checks
  add_definitions(-std=c++0x)             # enable C++11
else()
  # actually not known to work on any other compilers (MSVC!)
  message(FATAL_ERROR "please use gcc or clang to compile these examples")
endif()

# define targets
foreach( EXAMPLE
         plain_istream_iterator plain_istream_range )
  add_executable( ${EXAMPLE} ${EXAMPLE}.cpp )
endforeach()

add_executable( custom_stream_operator custom_stream_operator.cpp nmea.cpp )

add_definitions( -DFUSION_MAX_VECTOR_SIZE=15 )

add_executable( spirit_stream_operator spirit_stream_operator.cpp )
