cmake_minimum_required(VERSION 3.5)
project(sdl_fps_rabbit CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

#TODO: USE this for production
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -std=c++14")

#TODO: START enable this for debug
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#set(CMAKE_BUILD_TYPE Debug)
#TODO: END enable this for debug

find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)
include_directories( ${SDL_INCLUDE_DIR} )

file(COPY freesansbold.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(sdl_fps_rabbit main.cpp FpsCounter.cpp)
target_link_libraries(sdl_fps_rabbit ${SDL_LIBRARY} ${SDL_TTF_LIBRARY} ${SDL_IMAGE_LIBRARY} -lm)