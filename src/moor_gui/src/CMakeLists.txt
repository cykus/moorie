set( QMOORIE_SRCS
    qmoorie.cpp
    main.cpp
    addDownload.cpp
    about.cpp
    tabledelegate.cpp
    infoDialog.cpp
    myTableWidget.cpp
    singleton.cpp
    configdialog.cpp
    configurationpage.cpp
    threadinstance.cpp
)
set( QMOORIE_MOC_HDRS
    qmoorie.h
    addDownload.h
    about.h
    tabledelegate.h
    infoDialog.h
    myTableWidget.h
    singleton.h
    configdialog.h
    configurationpage.h
    threadinstance.h
)
set( QMOORIE_UIS
    qmoorie.ui
    about.ui
)
set( QMOORIE_RCS
     application.qrc
)

find_package(Boost COMPONENTS regex thread filesystem program_options REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include( ${QT_USE_FILE} )

qt4_wrap_ui( QMOORIE_UI_HDRS ${QMOORIE_UIS} )
qt4_wrap_cpp( QMOORIE_MOC_SRCS ${QMOORIE_MOC_HDRS} )
qt4_add_resources( QMOORIE_RC_SRCS ${QMOORIE_RCS} )

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
  /usr/local/lib
)

target_link_libraries(qmoorie
  ${QT_LIBRARIES}
  moor
  mcrypt 
  mhash 
  ${CURL_LIBRARIES} 
  ${OPENSSL_LIBRARIES} 
  ${Boost_THREAD_LIBRARY} 
  ${Boost_REGEX_LIBRARY} 
  ${Boost_FILESYSTEM_LIBRARY} 
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
install (TARGETS qmoorie DESTINATION bin)

# install desktop file

install(FILES ../qmoorie.desktop DESTINATION share/applications)
install(FILES ../qmoorie.menu DESTINATION share/menu)

# install icons
install (FILES images/hi16-app-qmoorie.png DESTINATION "share/icons/hicolor/16x16/apps" RENAME qmoorie.png)
install (FILES images/hi24-app-qmoorie.png DESTINATION "share/icons/hicolor/24x24/apps" RENAME qmoorie.png)
install (FILES images/hi32-app-qmoorie.png DESTINATION "share/icons/hicolor/32x32/apps" RENAME qmoorie.png)
install (FILES images/hi48-app-qmoorie.png DESTINATION "share/icons/hicolor/48x48/apps" RENAME qmoorie.png)
install (FILES images/hi64-app-qmoorie.png DESTINATION "share/icons/hicolor/64x64/apps" RENAME qmoorie.png)
install (FILES images/hisvg-app-qmoorie.svgz DESTINATION "share/icons/hicolor/scalable/apps" RENAME qmoorie.svgz)
install (FILES images/hixpm-app-qmoorie.xpm DESTINATION "share/pixmaps" RENAME qmoorie.xpm)