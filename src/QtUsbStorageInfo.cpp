#include "QtUsbStorageInfo.h"
#include <QDebug>

QString QtUsbStorageInfo::getSerialNumber(const QStorageInfo &storage) {
#if defined(Q_OS_WIN)
    return getWindowsSerial(storage);
#elif defined(Q_OS_MACOS)
    return getMacSerial(storage);
#elif defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    return getLinuxSerial(storage);
#elif defined(Q_OS_ANDROID)
    return getAndroidSerial(storage);
#elif defined(Q_OS_IOS)
    return getIosSerial(storage);
#else
    qWarning() << "Unsupported platform";
    return QString();
#endif
}
