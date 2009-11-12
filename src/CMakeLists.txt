# The name of our project is "HELLO". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and 
# to the root binary directory of the project as ${HELLO_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.6) 
project (MOOR) 

find_package(Boost COMPONENTS regex thread filesystem program_options REQUIRED)
# Recurse into the "Hello" and "Demo" subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_library (moor SHARED libmoor/Decoder.cpp  libmoor/LibMoor.cpp libmoor/Log.cpp  libmoor/Mailbox.cpp  libmoor/Mailboxes.cpp  libmoor/MoorhuntHash.cpp libmoor/YgoowHash.cpp libmoor/EmailHeader.cpp libmoor/Tools.cpp
libmoor/MailBoxes/MailRu.cpp)
add_subdirectory (libmoor)
add_subdirectory (moor_bin)

include_directories (${MOOR_SOURCE_DIR}/moor_bin) 
include_directories (${MOOR_SOURCE_DIR}/libmoor) 

link_directories (${MOOR_BINARY_DIR}/moor_bin)
link_directories (${MOOR_BINARY_DIR}/libmoor)


add_executable (moorie moor_bin/main.cpp)

target_link_libraries (moorie "ssl" "mcrypt" "mhash" "curl")
target_link_libraries (moorie moor ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})
