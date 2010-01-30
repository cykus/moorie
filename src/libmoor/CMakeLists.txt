add_library (moor SHARED Decoder.cpp
                         LibMoor.cpp 
                         Log.cpp  
                         Mailbox.cpp
                         Mailboxes.cpp  
                         MoorhuntHashDecoder.cpp
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
install (TARGETS moor DESTINATION lib${LIB_SUFFIX})
