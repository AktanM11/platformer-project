cmake_minimum_required(VERSION 3.27)
project(platformer)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -fsanitize=undefined")
endif()

add_executable(platformer platformer.cpp
                            globals.h
                            graphics.h
                            level.h
                            player_manager.h
                            enemy.h
                            assets.h
                            utilities.h
                            enemies_manager.h
                            enemies_manager.cpp
                            level_manager.h
                            level_manager.cpp
                            player.cpp
                            player_manager.cpp

)
target_link_libraries(platformer PRIVATE raylib)
