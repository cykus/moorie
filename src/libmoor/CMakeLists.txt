add_library (moor SHARED Decoder.cpp
                         LibMoor.cpp 
                         Log.cpp  
                         Mailbox.cpp
                         Mailboxes.cpp  
                         YgoowHashDecoder.cpp
                         Status.cpp 
                         EmailHeader.cpp
                         Tools.cpp
                         MailBoxes/MailRu.cpp
                         MailBoxes/GMail.cpp
                         MailBoxes/RockCom.cpp
                         MailBoxes/Onet.cpp
                         MailBoxes/Volny.cpp
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
target_link_libraries (moor ${PROJECT_SOURCE_DIR}/src/libmoor/libmoorhunt.a)
set_property(TARGET moor PROPERTY LINK_INTERFACE_LIBRARIES "")
install (TARGETS moor DESTINATION lib${LIB_SUFFIX})
