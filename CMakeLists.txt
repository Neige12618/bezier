cmake_minimum_required(VERSION 3.25)
project(curve-and-surface)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH $ENV{QT6_DIR})


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED)

qt_add_resources(qrc_FILES res.qrc)

add_executable(curve-and-surface
        main.cpp
        ${qrc_FILES}
        MainOpenGLWidget.cpp MainOpenGLWidget.h
        MainWindow.cpp MainWindow.h
        Controller.cpp Controller.h
        Mode.h
        Camera.cpp Camera.h
        GeometryEngine.cpp GeometryEngine.h)

target_link_libraries(curve-and-surface
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
        glu32
        opengl32
        )
