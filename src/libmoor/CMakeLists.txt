add_library (moor SHARED Decoder.cpp
                         LibMoor.cpp
                         Log.cpp  
                         Mailbox.cpp
                         Mailboxes.cpp  
                         YgoowHashDecoder.cpp
                         Status.cpp 
                         EmailHeader.cpp
                         Tools.cpp
##                         MailBoxes/HotmailCom.cpp
                         MailBoxes/GMail.cpp
                         MailBoxes/RockCom.cpp
                         MailBoxes/Onet.cpp
                         MailBoxes/Volny.cpp
                         MailBoxes/MailRu.cpp
                         MailBoxes/OiComBr.cpp
                         MailBoxes/MyNet.cpp
                         MailBoxes/GoogleApps.cpp
                         MailBoxes/Volny.cpp
                         MailboxFactory.cpp
                         HashManager.cpp
                         HashUtils.cpp
                         MoorhuntHash.cpp
                         YgoowHash.cpp
                         Hash.cpp
                         StringUtils.cpp
                         BinaryStream.cpp)

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm") 
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm") 
    SET(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -lm")
    target_link_libraries (moor ${PROJECT_SOURCE_DIR}/src/libmoor/Libraries/libmoorhunt64.a)
    
elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i686" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i586")
    target_link_libraries (moor ${PROJECT_SOURCE_DIR}/src/libmoor/Libraries/libmoorhunt.a)
endif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
# 
set_property(TARGET moor PROPERTY LINK_INTERFACE_LIBRARIES "")
install (TARGETS moor DESTINATION lib${LIB_SUFFIX})
