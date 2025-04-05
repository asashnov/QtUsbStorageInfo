#include "QtUsbStorageInfo.h"
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOStorageProtocolCharacteristics.h>
#include <DiskArbitration/DiskArbitration.h>
#include <QDebug>
#include <QStorageInfo>

// Helper function to convert CFString to QString
static QString CFStringToQString(CFStringRef str) {
    if (!str) return QString();
    CFIndex length = CFStringGetLength(str);
    CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
    char* buffer = new char[maxSize];
    if (CFStringGetCString(str, buffer, maxSize, kCFStringEncodingUTF8)) {
        QString result = QString::fromUtf8(buffer);
        delete[] buffer;
        return result;
    }
    delete[] buffer;
    return QString();
}

QString getMacSerial(const QStorageInfo &storage)
{
    QString bsdName = QFileInfo(storage.device()).fileName();
    if (!bsdName.startsWith("disk")) {
        qWarning() << "Not a disk device:" << bsdName;
        return QString();
    }

    DASessionRef session = DASessionCreate(kCFAllocatorDefault);
    if (!session) {
        qWarning() << "Failed to create DiskArbitration session";
        return QString();
    }

    QString serialNumber;
    DADiskRef disk = DADiskCreateFromBSDName(kCFAllocatorDefault, session, bsdName.toUtf8().constData());
    if (disk) {
        CFDictionaryRef desc = DADiskCopyDescription(disk);
        if (desc) {
            CFStringRef volumeKind = (CFStringRef)CFDictionaryGetValue(desc, kDADiskDescriptionVolumeKindKey);
            if (volumeKind && CFStringCompare(volumeKind, CFSTR("msdos"), 0) != kCFCompareEqualTo) {
                // For non-FAT volumes, try to get the native serial
                CFStringRef serial = (CFStringRef)CFDictionaryGetValue(desc, kDADiskDescriptionDeviceSerialNumberKey);
                if (serial) {
                    serialNumber = CFStringToQString(serial);
                }
            }
            
            if (serialNumber.isEmpty()) {
                // Fallback to IOMedia properties
                io_service_t media = DADiskCopyIOMedia(disk);
                if (media) {
                    CFTypeRef serial = IORegistryEntryCreateCFProperty(media, CFSTR(kIOPropertyProductSerialNumberKey), kCFAllocatorDefault, 0);
                    if (serial) {
                        serialNumber = CFStringToQString((CFStringRef)serial);
                        CFRelease(serial);
                    }
                    IOObjectRelease(media);
                }
            }
            CFRelease(desc);
        }
        CFRelease(disk);
    }
    CFRelease(session);

    if (serialNumber.isEmpty()) {
        qWarning() << "Could not retrieve serial number for" << bsdName;
    }

    return serialNumber;
}
