#include "QtUsbStorageInfo.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

QString getLinuxSerial(const QStorageInfo &storage)
{
    // Get the base device name (e.g., "sda" from "/dev/sda1")
    QString devicePath = storage.device();
    QFileInfo deviceInfo(devicePath);
    QString baseDevice = deviceInfo.baseName();  // "sda1" -> "sda"
    
    // Handle NVMe devices (e.g., "/dev/nvme0n1p1" -> "nvme0n1")
    if (baseDevice.startsWith("nvme")) {
        baseDevice = baseDevice.left(baseDevice.indexOf('p'));
    }

    // Path to the serial number file
    QString serialPath = QString("/sys/block/%1/device/serial").arg(baseDevice);

    // Alternative paths to try (different controllers/drivers)
    const QStringList altPaths = {
        QString("/sys/block/%1/device/vendor").arg(baseDevice),  // Some devices put serial here
        QString("/sys/block/%1/serial").arg(baseDevice),         // Alternative location
        QString("/sys/class/block/%1/device/serial").arg(baseDevice)
    };

    // Try primary path first
    QFile serialFile(serialPath);
    if (serialFile.open(QIODevice::ReadOnly)) {
        QString serial = QString::fromUtf8(serialFile.readAll()).trimmed();
        serialFile.close();
        if (!serial.isEmpty()) {
            return serial;
        }
    }

    // Fallback to alternative paths
    for (const QString &path : altPaths) {
        QFile altFile(path);
        if (altFile.open(QIODevice::ReadOnly)) {
            QString serial = QString::fromUtf8(altFile.readAll()).trimmed();
            altFile.close();
            if (!serial.isEmpty()) {
                return serial;
            }
        }
    }

    // Final fallback: Use device ID if no serial found
    QFile idFile(QString("/sys/block/%1/device/id").arg(baseDevice));
    if (idFile.open(QIODevice::ReadOnly)) {
        return QString::fromUtf8(idFile.readAll()).trimmed();
    }

    qWarning() << "Could not find serial number for device" << devicePath;
    return QString();
}
