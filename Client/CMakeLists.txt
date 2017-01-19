cmake_minimum_required(VERSION 3.5.1)

project(roosha.client)

set(CMAKE_CXX_STANDARD 11)

#------------------------ QHotkey
add_subdirectory(lib/QHotkey)
include_directories(lib/QHotkey)
set(LIBS_TO_LINK ${LIBS_TO_LINK} lib.qhotkey)
#------------------------- /QHotkey

#-----------------------Qt5
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#TODO: come up with more convenient way to set qt installation path
set(CMAKE_PREFIX_PATH $ENV{HOME}/.Qt/5.7/gcc_64)

set(QT5_USED_MODULES Qml Quick Widgets Core QuickWidgets QuickControls2 Concurrent)
find_package(Qt5 COMPONENTS ${QT5_USED_MODULES} REQUIRED)
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
        Network/preferences.h
        Network/error_status.h)

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
        GUI/cardeditioncontroller.cpp)

set(ROOSHA_RESOURCES QML/qml.qrc
        Img/img.qrc)
#------------------------------/Project sources

set(HEADERS ${QHK_HEADERS} ${ROOSHA_HEADERS} ${PROTO_HEADERS})
set(SOURCES ${QHK_SOURCES} ${ROOSHA_SOURCES} ${PROTO_SOURCES})
qt5_add_resources(RESOURCES ${ROOSHA_RESOURCES})

add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS_TO_LINK})
qt5_use_modules(${PROJECT_NAME} ${QT5_USED_MODULES})