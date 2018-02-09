# Enabling debug symbols
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

list (APPEND CMAKE_CXX_FLAGS "-fPIC -Wall -DGL_GLEXT_PROTOTYPES -Werror=return-type -g -pedantic-errors -std=c++11${CMAKE_CXX_FLAGS}")

file(GLOB SRCS *.cpp *.h)

add_executable(SRPG ${SRCS})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
find_package(SFML 2.4.0 REQUIRED system window graphics network audio)
find_package(Jsoncpp REQUIRED)

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(SRPG ${SFML_LIBRARIES})
endif()

if(Jsoncpp_FOUND)
	message("Found jsoncpp in " ${Jsoncpp_INCLUDE_DIR})
	include_directories(${Jsoncpp_INCLUDE_DIR})
	target_link_libraries(SRPG ${Jsoncpp_LIBRARY})
else()
	message("-- Jsoncpp not found")
	message("  -- jsoncpp_INCLUDE_DIR: " ${Jsoncpp_INCLUDE_DIR})
	message("  -- jsoncpp_LIBRARY: " ${Jsoncpp_LIBRARIES})
endif()
