#pragma once
#include <QString>
#include <QStorageInfo>

class QtUsbStorageInfo {
public:
    static QString getSerialNumber(const QStorageInfo &storage);
};
