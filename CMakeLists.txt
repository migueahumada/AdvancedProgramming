# minimum cmake version
cmake_minimum_required(VERSION 3.31)

# name of the project, and language
project(AdvancedProgramming)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(HEADERS inc/Actor.h
            inc/App.h
            inc/Character.h
            inc/HelperMacros.h
            inc/Object.h
            inc/SceneObject.h
            inc/Prop.h
            inc/World.h
            inc/Event.h)

SET(SOURCE  src/main.cpp
            src/App.cpp
            src/Object.cpp
            src/SceneObject.cpp
            src/Actor.cpp
            src/Character.cpp
            src/Prop.cpp
            src/World.cpp
            src/Event.cpp)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})

# include files 
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)

# link static library
target_link_libraries(${PROJECT_NAME} PRIVATE   ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/lib/sfml-graphics.lib
                                                ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/lib/sfml-window.lib
                                                ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/lib/sfml-system.lib)


if(WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/bin/sfml-graphics-3.dll"
      "${CMAKE_CURRENT_BINARY_DIR}/Debug/sfml-graphics-3.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/bin/sfml-window-3.dll"
      "${CMAKE_CURRENT_BINARY_DIR}/Debug/sfml-window-3.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/bin/sfml-system-3.dll"
      "${CMAKE_CURRENT_BINARY_DIR}/Debug/sfml-system-3.dll"
    COMMAND_EXPAND_LISTS
    COMMENT "Copying DLLs to target dir")  
endif()

