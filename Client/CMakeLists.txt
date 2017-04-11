cmake_minimum_required(VERSION 3.5.1)

project(roosha.client)

set(CMAKE_CXX_STANDARD 14)

#------------------------ QHotkey
add_subdirectory(lib/QHotkey)
include_directories(lib/QHotkey)
set(LIBS_TO_LINK ${LIBS_TO_LINK} lib.qhotkey )
#------------------------- /QHotkey

#-----------------------Qt5
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#TODO: come up with more convenient way to set qt installation path
set(CMAKE_PREFIX_PATH $ENV{HOME}/.Qt/5.8/gcc_64)

set(QT5_USED_MODULES Qml Quick Widgets Core QuickWidgets QuickControls2 Concurrent Test)
find_package(Qt5 COMPONENTS ${QT5_USED_MODULES} REQUIRED)
#----------------------/Qt5


#----------------GRPC
find_package(Protobuf REQUIRED)
link_libraries(grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY})

set(PROTO_SOURCES Network/Proto/changes.pb.cc
        Network/Proto/commons.pb.cc
        Network/Proto/roosha_service.grpc.pb.cc
        Network/Proto/roosha_service.pb.cc
        Network/Proto/learning.pb.cc
        )
#----------------/GRPC

#--------------------Project sources

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
        GUI/QmlWidget.cpp
        Helpers/StateHolder.cpp
        GUI/Learning/LearningWindowController.cpp
        GUI/Learning/LearningStrategy.cpp
        GUI/Learning/CardLearningViewModels.cpp
        Core/QLSeq.cpp
        GUI/Learning/UserInputModels.cpp 
        GUI/Learning/UserInputModels.h
        Core/Scrutiny.cpp Core/Scrutiny.cpp
        GUI/Learning/CardDifficulty.cpp
        GUI/Learning/LearningManager.cpp
        )

set(ROOSHA_RESOURCES QML/qml.qrc
        Img/img.qrc
        )
#------------------------------/Project sources

set(HEADERS ${QHK_HEADERS} ${ROOSHA_HEADERS} ${PROTO_HEADERS})
set(SOURCES ${QHK_SOURCES} ${ROOSHA_SOURCES} ${PROTO_SOURCES})
qt5_add_resources(RESOURCES ${ROOSHA_RESOURCES})

add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS_TO_LINK})
qt5_use_modules(${PROJECT_NAME} ${QT5_USED_MODULES})


#-----------------------------Testing

enable_testing()

find_package(Qt5Test REQUIRED)

set(TESTED_SOURCES Core/QLSeq.cpp)

set(TESTS_SOURCES Test/Core/TestLSequence.cpp)

add_executable(TestApp Test/runTests.cpp ${TESTED_SOURCES} ${TESTS_SOURCES})
add_test(TestApp TestApp)

target_link_libraries(TestApp Qt5::Test ${LIBS_TO_LINK})
qt5_use_modules(TestApp ${QT5_USED_MODULES})
