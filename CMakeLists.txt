cmake_minimum_required(VERSION 3.5)
project(QtUsbStorageInfo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

# Platform-specific sources
if(WIN32)
    set(SOURCES src/QtUsbStorageInfo_win.cpp)
elseif(APPLE)
    if(IOS)
        set(SOURCES src/QtUsbStorageInfo_ios.mm)
    else()
        set(SOURCES src/QtUsbStorageInfo_mac.cpp)
    endif()
elseif(ANDROID)
    set(SOURCES src/QtUsbStorageInfo_android.cpp)
elseif(UNIX AND NOT ANDROID)
    set(SOURCES src/QtUsbStorageInfo_linux.cpp)
endif()

add_library(QtUsbStorageInfo STATIC
    src/QtUsbStorageInfo.cpp
    ${SOURCES}
)

target_include_directories(QtUsbStorageInfo PUBLIC include)
target_link_libraries(QtUsbStorageInfo PRIVATE Qt5::Core)

if(WIN32)
    target_link_libraries(QtUsbStorageInfo PRIVATE Setupapi)
endif()

if(APPLE)
    target_link_libraries(QtUsbStorageInfo PRIVATE
        "-framework IOKit"
        "-framework DiskArbitration"
        "-framework CoreFoundation"
    )
endif()

if(ANDROID)
    find_package(Qt5 COMPONENTS AndroidExtras REQUIRED)
    target_link_libraries(QtUsbStorageInfo PRIVATE Qt5::AndroidExtras)
endif()
