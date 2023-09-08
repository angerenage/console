cmake_minimum_required(VERSION 3.12)

project(retroConsole)

set(SRCS
	src/retroConsole.cpp
	src/Logic.cpp
	src/core/CPU/ALU.cpp
	src/core/CPU/Core.cpp
	src/core/CPU/CPU.cpp
	)
	
set(HEADERS
	src/Logic.h
	src/core/CPU/ALU.h
	src/core/CPU/Core.h
	src/core/CPU/CPU.h
	)

add_executable(retroConsole ${SRCS} ${HEADERS})