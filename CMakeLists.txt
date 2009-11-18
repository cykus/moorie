cmake_minimum_required(VERSION 2.6)

project(MOOR)

option(ENABLE_GUI "Kompilacja wraz z Qmoorie")

add_subdirectory(doc)
add_subdirectory(src)

message(STATUS)
message(STATUS "ENABLE_GUI = ${ENABLE_GUI}")
message(STATUS)

set(ENABLE_GUI ${ENABLE_GUI} CACHE BOOL
		"Set to ON to build with Qmoorie" FORCE)
