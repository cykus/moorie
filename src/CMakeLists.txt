set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/")

find_package(Boost COMPONENTS regex program_options thread filesystem REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(mcrypt REQUIRED)
find_package(mhash REQUIRED)

add_subdirectory (libmoor)
add_subdirectory (moor_bin)
if(ENABLE_GUI)
	add_subdirectory (moor_gui)
endif(ENABLE_GUI)


