cmake_minimum_required (VERSION 2.6) 
project (MOOR) 

find_package(Boost COMPONENTS regex thread filesystem program_options REQUIRED)
find_package (CURL REQUIRED)
find_package (OpenSSL REQUIRED)

add_library (moor SHARED libmoor/Decoder.cpp
												 libmoor/LibMoor.cpp 
												 libmoor/Log.cpp  
												 libmoor/Mailbox.cpp  
												 libmoor/Mailboxes.cpp  
												 libmoor/MoorhuntHashDecoder.cpp 
												 libmoor/YgoowHashDecoder.cpp 
												 libmoor/Status.cpp 
												 libmoor/EmailHeader.cpp 
												 libmoor/Tools.cpp
												 libmoor/MailBoxes/MailRu.cpp 
												 libmoor/MailBoxes/GMail.cpp 
												 libmoor/MailBoxes/Yahoo.cpp 
												 libmoor/MailboxFactory.cpp
												 libmoor/HashManager.cpp
												 libmoor/HashUtils.cpp
												 libmoor/StringUtils.cpp
												 libmoor/Utils.cpp)
add_subdirectory (libmoor)
add_subdirectory (moor_bin)

option(ENABLE_GUI "Kompilacja wraz z Qmoorie" OFF)
if(ENABLE_GUI)
add_subdirectory (moor_gui)
endif(ENABLE_GUI)

include_directories (${MOOR_SOURCE_DIR}/moor_bin) 
include_directories (${MOOR_SOURCE_DIR}/libmoor) 

link_directories (${MOOR_BINARY_DIR}/moor_bin)
link_directories (${MOOR_BINARY_DIR}/libmoor)


add_executable (moorie moor_bin/main.cpp)

target_link_libraries (moorie "mcrypt" "mhash")
target_link_libraries (moorie moor ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})
