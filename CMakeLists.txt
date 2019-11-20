cmake_minimum_required(VERSION 3.15)


# ========= GLOBAL CONFIGURATIONS =========
# -----------------------------------------

# set the project name
project(main VERSION 1.0)

set(CMAKE_FIND_DEBUG_MODE 1)
set(CMAKE_VERBOSE_MAKEFILE ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find the needed packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)

# ========= LOCAL CONFIGURATIONS ==========
# -----------------------------------------
set(LIB lib)

# ========= TARGETS CONFIGURATIONS ========
# -----------------------------------------
# add the executable
set(SOURCES main.cpp
			${LIB}/math3d.h
			${LIB}/game_of_life.h
	)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE GLUT::GLUT)
target_link_libraries(main PRIVATE GL)
target_link_libraries(main PRIVATE GLEW)
target_link_libraries(main PRIVATE ${OpenCV_LIBS})

# add config file
configure_file(config.h.in config.h)

target_include_directories(main PUBLIC
								lib
								)

