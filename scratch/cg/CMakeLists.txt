# CMakeLists.txt
#
# To build the application, run the following commands:
# 
#   mkdir cbuild
#   cd cbuild
#   cmake ../
#   make
#
# Here are some reference links:
#
#   http://wiki.qtcentre.org/index.php?title=Compiling_Qt4_apps_with_CMake
#   http://qtnode.net/wiki/Qt_with_cmake#cmake_for_Qt_4
#

PROJECT(cg)


# ----------------------------------------------------------------------------
# Require CMake (>= 2.4.0)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

# Verbose - on/off
SET(CMAKE_VERBOSE_MAKEFILE ON)

# Disallow in-source build
STRING(COMPARE EQUAL "${cg_SOURCE_DIR}" "${cg_BINARY_DIR}" INSOURCE)
IF(INSOURCE)
    MESSAGE(FATAL_ERROR "cg requires an out of source build. Please create a separate binary directory and run cmake there.")
ENDIF(INSOURCE)


# ----------------------------------------------------------------------------
# Input files for cg project
SET(cg_UIS
    src/MainWindow.ui
)
SET(cg_SOURCES
    src/main.cpp
    src/MainWindow.cpp
)
SET(cg_MOC_HEADERS
    src/MainWindow.h
)

# ----------------------------------------------------------------------------
# Configure Cigma
INCLUDE(${cg_SOURCE_DIR}/CMake/FindCigma.cmake)


# ----------------------------------------------------------------------------
# Configure HDF5
INCLUDE(${cg_SOURCE_DIR}/CMake/FindHDF5.cmake)
INCLUDE(${cg_SOURCE_DIR}/CMake/FindZLIB.cmake)


# ----------------------------------------------------------------------------
# Configure VTK
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})


# ----------------------------------------------------------------------------
# Configure Qt4
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})


# ----------------------------------------------------------------------------
# Definitions
ADD_DEFINITIONS(
    -Wall
    ${QT_DEFINITIONS}
)

# ----------------------------------------------------------------------------
# Include directories
INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES}
    src
)

# ----------------------------------------------------------------------------
# Qt4 definitions
QT4_WRAP_UI(cg_UIS_H ${cg_UIS})
QT4_WRAP_CPP(cg_MOC_SOURCES ${cg_MOC_HEADERS})
QT4_AUTOMOC(${cg_SOURCES})

# The main build target
ADD_EXECUTABLE(cigma-gui
    ${cg_SOURCES}
    ${cg_UIS_H}
    ${cg_MOC_SOURCES}
    ${cg_UIS}
)

# Libraries to be linked to the project
TARGET_LINK_LIBRARIES(cigma-gui
    ${QT_LIBRARIES}
    QVTK
    vtkCommon
    vtkIO
    vtkRendering
    vtkGraphics
    vtkWidgets
)


# ----------------------------------------------------------------------------
# Build instructions for test application
#SET(cgt_UIS
#    src/TestWindow.ui
#)
#SET(cgt_SOURCES
#    src/testapp.cpp
#    src/TestWindow.cpp
#)
#SET(cgt_MOC_HEADERS
#    src/TestWindow.h
#)
#QT4_WRAP_UI(cgt_UIS_H ${cgt_UIS})
#QT4_WRAP_CPP(cgt_MOC_SOURCES ${cgt_MOC_HEADERS})
#QT4_AUTOMOC(${cgt_SOURCES})
#ADD_EXECUTABLE(cgt
#    ${cgt_SOURCES}
#    ${cgt_UIS_H}
#    ${cgt_MOC_SOURCES}
#    ${cgt_UIS}
#)
#TARGET_LINK_LIBRARIES(cgt
#    ${QT_LIBRARIES}
#    QVTK
#    vtkCommon
#    vtkIO
#    vtkRendering
#    vtkGraphics
#    vtkWidgets
#)


# ----------------------------------------------------------------------------
#EOF
