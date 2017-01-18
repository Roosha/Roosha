cmake_minimum_required(VERSION 3.5.1)

project(RooshaClient)

set(CMAKE_CXX_STANDARD 11)

#------------------Prepare Qt related variables
#TODO: come up with more convenient way to set qt installation path
set(CMAKE_PREFIX_PATH  $ENV{HOME}/.Qt/5.7/gcc_64)
set(QT5_USED_MODULES Qml Quick Widgets Core QuickWidgets QuickControls2 Concurrent)
#-----------------/Prepare Qt related variables

#------------------------ QHotkey
#    TODO: use QHotkey as subdirectory with it's own CMakeLists.txt
set(QHK_HEADERS lib/QHotkey/qhotkey_p.h lib/QHotkey/qhotkey.h)

set(QHK_SOURCES lib/QHotkey/qhotkey.cpp)
if (WIN32)
    #    TODO: LIBS += -lUser32
    set(QHK_SOURCES ${QHK_SOURCES} lib/QHotkey/qhotkey_win.cpp)
elseif(APPLE)
    #    TODO: LIBS += -framework Carbon
    set(QHK_SOURCES ${QHK_SOURCES} lib/QHotkey/qhotkey_mac.cpp)
elseif(UNIX)
    set(QHK_SOURCES ${QHK_SOURCES} lib/QHotkey/qhotkey_x11.cpp)

    set(QT5_USED_MODULES ${QT5_USED_MODULES} X11Extras)

    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    link_libraries(${X11_LIBRARIES})
else()
    message(FATAL_ERROR "Unknown platform. Can not build. CMake will exit")
endif()

include_directories(lib/QHotkey)
#------------------------- /QHotkey

#-----------------------Qt5
find_package(Qt5 COMPONENTS ${QT5_USED_MODULES} REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#----------------------/Qt5


#----------------GRPC
find_package(Protobuf REQUIRED)
link_libraries(grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY})

set(PROTO_HEADERS Network/Proto/changes.pb.h
        Network/Proto/commons.pb.h
        Network/Proto/roosha_service.grpc.pb.h
        Network/Proto/roosha_service.pb.h)

set(PROTO_SOURCES Network/Proto/changes.pb.cc
        Network/Proto/commons.pb.cc
        Network/Proto/roosha_service.grpc.pb.cc
        Network/Proto/roosha_service.pb.cc)
#----------------/GRPC

#--------------------Project sources
set(ROOSHA_HEADERS Core/centralcontroller.h
        Core/hotkeylistener.h
        Core/card.h
        Core/dbcard.h
        GUI/guimanager.h
        Helpers/bootstrap.h
        Helpers/configuremanager.h
        Core/translation.h
        GUI/translationcontroller.h
        Helpers/qmlconvertation.h
        Core/ichange.h
        Core/changes.h
        Network/server_response.h
        Network/authentication_manager.h
        Network/roosha_service_connector.h
        Network/network_manager.h
        Test/Network/translations_test_slot_holder.h
        Core/world.h
        Test/Core/changes/worldtest.h
        GUI/cardlistcontroller.h
        Helpers/protobuf_converter.h
        GUI/authentication_controller.h
        GUI/system_tray.h
        GUI/cardeditioncontroller.h
        GUI/QmlWidget.h)

set(ROOSHA_SOURCES main.cpp
        Core/centralcontroller.cpp
        Core/hotkeylistener.cpp
        Core/card.cpp
        Core/dbcard.cpp
        GUI/guimanager.cpp
        Helpers/bootstrap.cpp
        Helpers/configuremanager.cpp
        Core/translation.cpp
        GUI/translationcontroller.cpp
        Helpers/qmlconvertation.cpp
        Test/Network/translations_test_slot_holder.cpp
        Core/changes.cpp
        Network/server_response.cpp
        Network/authentication_manager.cpp
        Network/roosha_service_connector.cpp
        Network/network_manager.cpp
        Helpers/protobuf_converter.cpp
        Core/world.cpp
        GUI/system_tray.cpp
        Test/Core/changes/worldtest.cpp
        GUI/cardlistcontroller.cpp
        GUI/authentication_controller.cpp
        GUI/cardeditioncontroller.cpp
        GUI/QmlWidget.cpp )

set(ROOSHA_RESOURCES QML/qml.qrc
        Img/img.qrc)
#------------------------------/Project sources

set(HEADERS ${QHK_HEADERS} ${ROOSHA_HEADERS} ${PROTO_HEADERS})
set(SOURCES ${QHK_SOURCES} ${ROOSHA_SOURCES} ${PROTO_SOURCES})
qt5_add_resources(RESOURCES ${ROOSHA_RESOURCES})

add_executable(roosha ${SOURCES} ${RESOURCES})

qt5_use_modules(roosha ${QT5_USED_MODULES})