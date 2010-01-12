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
													MailBoxes/RockCom.cpp
													MailBoxes/Onet.cpp
													MailBoxes/OiComBr.cpp
													MailBoxes/MyNet.cpp
                                                                                                        MailBoxes/GoogleApps.cpp
													MailboxFactory.cpp
													HashManager.cpp
													HashUtils.cpp
													Utils.cpp
													MoorhuntHash.cpp
													YgoowHash.cpp
													Hash.cpp)
install (TARGETS moor DESTINATION lib${LIB_SUFFIX})
