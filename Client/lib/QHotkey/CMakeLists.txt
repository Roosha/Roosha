cmake_minimum_required(VERSION 3.5.1)

project(lib.qhotkey)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH $ENV{HOME}/.Qt/5.8/gcc_64)
set(QT5_USED_COMPONENTS Gui)

set(HEADERS qhotkey.h qhotkey_p.h)
set(SOURCES qhotkey.cpp)

if (WIN32)
    # TODO: LIBS += -luser32
    set(SOURCES ${SOURCES} qhotkey_win.cpp)
elseif (APPLE)
    # TODO: LIBS += -framework Carbon
    set(SOURCES ${SOURCES} qhotkey_mac.cpp)
elseif (UNIX)
    set(SOURCES ${SOURCES} qhotkey_x11.cpp)

    set(QT5_USED_COMPONENTS ${QT5_USED_COMPONENTS} X11Extras)

    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    set(LIBS ${X11_LIBRARIES})
else ()
    message(FATAL_ERROR "Unknown platform. Can not build. CMake will exit")
endif ()

find_package(Qt5 COMPONENTS ${QT5_USED_COMPONENTS} REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
qt5_use_modules(${PROJECT_NAME} ${QT5_USED_COMPONENTS})