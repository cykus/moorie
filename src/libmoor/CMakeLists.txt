add_library (moor SHARED	Decoder.cpp
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
													MailBoxes/Yahoo.cpp 
													MailboxFactory.cpp
													HashManager.cpp
													HashUtils.cpp
													Utils.cpp)
