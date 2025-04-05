#include "QtUsbStorageInfo.h"
#include <windows.h>
#include <winioctl.h>
#include <ntddstor.h>
#include <QDebug>

QString getWindowsSerial(const QStorageInfo &storage)
{
    QString devicePath = QString("\\\\.\\%1")
                        .arg(storage.device().replace(QRegExp("^\\\\.\\\\"), "").left(2));

    HANDLE hDevice = CreateFileW(
        devicePath.toStdWString().c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        qWarning() << "Failed to open device:" << GetLastError();
        return QString();
    }

    STORAGE_PROPERTY_QUERY query{};
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    STORAGE_DESCRIPTOR_HEADER header{};
    DWORD bytesReturned = 0;

    // First call to get required buffer size
    if (!DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        &header,
        sizeof(header),
        &bytesReturned,
        NULL
    ) && GetLastError() != ERROR_MORE_DATA) {
        qWarning() << "Initial DeviceIoControl failed:" << GetLastError();
        CloseHandle(hDevice);
        return QString();
    }

    // Allocate buffer for full descriptor
    const DWORD bufferSize = header.Size;
    BYTE* pBuffer = new BYTE[bufferSize];

    // Second call to get full descriptor
    if (!DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        pBuffer,
        bufferSize,
        &bytesReturned,
        NULL
    )) {
        qWarning() << "DeviceIoControl failed:" << GetLastError();
        delete[] pBuffer;
        CloseHandle(hDevice);
        return QString();
    }

    // Extract serial number
    STORAGE_DEVICE_DESCRIPTOR* pDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(pBuffer);
    QString serial;

    if (pDescriptor->SerialNumberOffset) {
        const char* serialPtr = reinterpret_cast<const char*>(pBuffer) + pDescriptor->SerialNumberOffset;
        serial = QString::fromLatin1(serialPtr).trimmed();
    }

    delete[] pBuffer;
    CloseHandle(hDevice);
    return serial;
}
