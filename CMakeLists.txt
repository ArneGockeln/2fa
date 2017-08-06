# minimum cmake version
cmake_minimum_required(VERSION 3.8)

# project name
project(2FA)

# build type
set(CMAKE_BUILD_TYPE release)

# set version
SET(PROJECT_VERSION_MAJOR 1)
SET(PROJECT_VERSION_MINOR 0)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# basic information about the author
set(COMPANY "Arne Gockeln")
set(COPYRIGHT "Copyright(c) 2017 Arne Gockeln. All rights reserved.")
set(IDENTIFIER "com.arnegockeln.2FA")

# set c++ standard to c++14
set(CMAKE_CXX_STANDARD 14)

# set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/arnegockeln/Sourcecode/SDKs/Qt/5.9.1/clang_64")
set(SOURCE_FILES main.cpp MainWindow.cpp MainWindow.h TableModel.cpp TableModel.h AddressWidget.cpp AddressWidget.h AddDialog.cpp AddDialog.h Storage.cpp Storage.h Presets.h ServiceItem.h Auth.cpp Auth.h)
set(RESOURCE_FOLDER resources)

# app icon
set(ICON_NAME "AppIconLock")
set(ICON_FILE ${RESOURCE_FOLDER}/${ICON_NAME}.icns)

# Resources
set(RESOURCE_FILES ${RESOURCE_FOLDER}/resources.qrc)

# Include files
set(INCLUDE_FILES include/libcppotp/sha1.h include/libcppotp/sha1.cpp include/libcppotp/otp.h include/libcppotp/otp.cpp include/libcppotp/bytes.h include/libcppotp/bytes.cpp)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# macOS related
set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

#if(APPLE)
set(OS_BUNDLE MACOSX_BUNDLE)
set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
set(MACOSX_BUNDLE_INFO_STRING "An app for two factor authentication.")
set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT})
set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER})
set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
#endif()

# find qt packages
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)

# add executable
add_executable(2FA ${OS_BUNDLE} ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES} ${ICON_FILE})

# link libs
target_link_libraries(2FA Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml)

# add custom plist for macos settings
set_target_properties(2FA PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/MacOSXBundleInfo.plist.in
    MACOSX_BUNDLE_ICON_FILE ${ICON_NAME}
    MACOSX_FRAMEWORK_ICON_FILE ${ICON_NAME}
)
