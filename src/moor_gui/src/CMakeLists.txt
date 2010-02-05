set( QMOORIE_SRCS
    main.cpp
    qmoorie.cpp
    aboutdialog.cpp
    configdialog.cpp
    newdownloaddialog.cpp
    newuploaddialog.cpp
    infodialog.cpp
    tabledelegate.cpp
    mytablewidget.cpp
    mysystemtrayicon.cpp
    configurationpage.cpp
    uploadpage.cpp
    singleton.cpp
    downloadinstance.cpp
    uploadinstance.cpp
    statusesthread.cpp
    tools.cpp
)
set( QMOORIE_MOC_HDRS
    qmoorie.h
    newdownloaddialog.h
    newuploaddialog.h
    aboutdialog.h
    configdialog.h
    infodialog.h
    tabledelegate.h
    mytablewidget.h
    mysystemtrayicon.h
    configurationpage.h
    uploadpage.h
    statusesthread.h
)
set( QMOORIE_UIS
    qmoorie.ui
    aboutdialog.ui
)
set( QMOORIE_RCS
     application.qrc
)
 
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTDBUS TRUE)

include( ${QT_USE_FILE} )

qt4_wrap_ui( QMOORIE_UI_HDRS ${QMOORIE_UIS} )
qt4_wrap_cpp( QMOORIE_MOC_SRCS ${QMOORIE_MOC_HDRS} )
qt4_add_resources( QMOORIE_RC_SRCS ${QMOORIE_RCS} )
qt4_automoc(${QMOORIE_MOC_HDRS})

add_definitions( -O2 -w -D_REENTRANT -DQT_NO_KEYWORDS $(QT_DEFINITIONS) )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR}
    ${QMOORIE_BINARY_DIR}
    ${QMOORIE_SOURCE_DIR}
    ${MOOR_SOURCE_DIR}/src/libmoor
)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR} )

add_executable( qmoorie
    ${QMOORIE_SRCS}
    ${QMOORIE_MOC_SRCS}
    ${QMOORIE_UI_HDRS}
    ${QMOORIE_RC_SRCS}
)
link_directories(
  ${MOOR_BINARY_DIR}/src/libmoor
)

target_link_libraries(qmoorie
  ${QT_LIBRARIES}
  moor
  ${MCRYPT_LIBRARIES}
  ${MHASH_LIBRARIES}
  ${CURL_LIBRARIES} 
  ${OPENSSL_LIBRARIES} 
  ${Boost_THREAD_LIBRARY} 
  ${Boost_REGEX_LIBRARY} 
  ${Boost_FILESYSTEM_LIBRARY} 
)
install (TARGETS qmoorie DESTINATION bin)

# install desktop file

install(FILES ../qmoorie.desktop DESTINATION share/applications)
install(FILES ../qmoorie.menu DESTINATION share/menu)

# install icons
install (FILES images/hi16-app-qmoorie.png DESTINATION "share/icons/hicolor/16x16/apps" RENAME qmoorie.png)
install (FILES images/hi22-app-qmoorie.png DESTINATION "share/icons/hicolor/22x22/apps" RENAME qmoorie.png)
install (FILES images/hi32-app-qmoorie.png DESTINATION "share/icons/hicolor/32x32/apps" RENAME qmoorie.png)
install (FILES images/hi48-app-qmoorie.png DESTINATION "share/icons/hicolor/48x48/apps" RENAME qmoorie.png)
install (FILES images/hi64-app-qmoorie.png DESTINATION "share/icons/hicolor/64x64/apps" RENAME qmoorie.png)
install (FILES images/hisvg-app-qmoorie.svgz DESTINATION "share/icons/hicolor/scalable/apps" RENAME qmoorie.svgz)
install (FILES images/hixpm-app-qmoorie.xpm DESTINATION "share/pixmaps" RENAME qmoorie.xpm)
