INCLUDEPATH += $$PWD/include
HEADERS += $$PWD/include/QtUsbStorageInfo.h
SOURCES += $$PWD/src/QtUsbStorageInfo.cpp

win32 {
  SOURCES += $$PWD/src/QtUsbStorageInfo_win.cpp
  LIBS += -lSetupapi
}

macx {
  SOURCES += $$PWD/src/QtUsbStorageInfo_mac.cpp
  LIBS += -framework IOKit -framework DiskArbitration -framework CoreFoundation
}

linux:!android: SOURCES += $$PWD/src/QtUsbStorageInfo_linux.cpp

android: SOURCES += $$PWD/src/QtUsbStorageInfo_android.cpp

ios {
    QT += core
    OBJECTIVE_SOURCES += QtUsbStorageInfo_ios.mm
    LIBS += -framework Foundation -framework UIKit
}
