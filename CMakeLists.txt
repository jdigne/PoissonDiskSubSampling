project(pdss)


cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS_RELEASE "-O3")

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

SET(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS} -fopenmp”)
SET(CMAKE_C_FLAGS “${CMAKE_C_FLAGS} -fopenmp”)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)


SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-write-strings -ansi -Wfatal-errors")

ADD_EXECUTABLE(pdss     src/main.cpp
			src/Point.cpp
			src/FileIO.cpp
			src/Sample.cpp
			)
	

install(TARGETS pdss RUNTIME DESTINATION bin)
