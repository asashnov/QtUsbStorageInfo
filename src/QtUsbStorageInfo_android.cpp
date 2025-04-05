#include <QAndroidJniObject>
#include <QtAndroid>

QString getAndroidSerial(const QStorageInfo &storage) {
    QAndroidJniObject usbManager = QtAndroid::androidContext().callObjectMethod(
        "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;",
        QAndroidJniObject::fromString("usb").object()
    );

    QAndroidJniObject devices = usbManager.callObjectMethod(
        "getDeviceList",
        "()Ljava/util/HashMap;"
    );

    // Iterate devices and match by storage path
    // (Requires Android USB Host Mode permissions)
}
