# Enabling debug symbols
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(../game)
add_library(InputManager STATIC ../game/InputManager.cpp)

list (APPEND CMAKE_CXX_FLAGS "-w -g -pedantic-errors -std=c++11${CMAKE_CXX_FLAGS}")

file(GLOB SRCS *.cpp *.h)

add_executable(DialogueGrapher ${SRCS})
target_link_libraries(DialogueGrapher InputManager)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
find_package(SFML 2.2.0 REQUIRED system window graphics network audio)
find_package(OpenGL)
find_package(Jsoncpp)

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(DialogueGrapher ${SFML_LIBRARIES})
endif()

if(OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
  target_link_libraries(DialogueGrapher ${OPENGL_LIBRARIES})
endif()

if(Jsoncpp_FOUND)
#	message(Found jsoncpp in ${JSONCPP_INCLUDE_DIR})
#	include_directories(${JSONCPP_INCLUDE_DIR})
#	target_link_libraries(DialogueGrapher ${JSONCPP_LIBRARIES})
	message("Found jsoncpp in " ${Jsoncpp_INCLUDE_DIR})
	include_directories(${Jsoncpp_INCLUDE_DIR})
	target_link_libraries(DialogueGrapher ${Jsoncpp_LIBRARY})
else()
	message("-- Jsoncpp not found")
	message("  -- jsoncpp_INCLUDE_DIR: " ${Jsoncpp_INCLUDE_DIR})
	message("  -- jsoncpp_LIBRARY: " ${Jsoncpp_LIBRARIES})
endif()
