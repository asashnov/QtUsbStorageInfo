# QtUsbStorageInfo

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A cross-platform Qt library to retrieve USB storage device serial numbers on **Windows, macOS, Linux, Android, and iOS**.

## Features
- **Single-header API** for easy integration  
- Supports **Qt5/Qt6** with qmake or CMake  
- Works on:
  - Windows (`SetupAPI`)
  - macOS (`IOKit`)
  - Linux (`/sys` filesystem)
  - Android (Java/JNI)
  - iOS (Objective-C++)  
- MIT Licensed (free for commercial use)

## Installation

### As a Git Submodule

```bash
git submodule add https://github.com/your-repo/QtUsbStorageInfo.git
```

### Via qmake
Include the .pri file in your project:
```qmake
include($$PWD/QtUsbStorageInfo/QtUsbStorageInfo.pri)
```

### Via CMake
```cmake
add_subdirectory(QtUsbStorageInfo)
target_link_libraries(YourApp PRIVATE QtUsbStorageInfo)
```

## Usage

```c
#include "QtUsbStorageInfo.h"

// Get serial number for a mounted volume
QStorageInfo storage("/Volumes/USB_DRIVE");  // Or "E:" (Windows), "/mnt/usb" (Linux)
if (storage.isValid()) {
    QString serial = QtUsbStorageInfo::getSerialNumber(storage);
    qDebug() << "USB Serial:" << serial;
}
```

## Platform Requirements
| Platform    | Dependencies               | Notes                          |
|:------------|:---------------------------|:-------------------------------|
| **Windows** | `SetupAPI.h` (Win32 SDK)   | No extra libs needed           |
| **macOS**   | `IOKit.framework`          | Built into macOS               |
| **Linux**   | `/sys` filesystem access   | Runs without root              |
| **Android** | `USB Host Mode` permissions| Requires `android.permission.USB` |
| **iOS**     | Filesystem APIs            | Sandboxed access only          |

## Build from Source
```bash
# CMake
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt
cmake --build .

# qmake
qmake && make
```

## License

MIT - See LICENSE for details.  
Copyright (c) 2025 Alexander Sashnov

## Contributing

1. Fork the repository
1. Create a feature branch (git checkout -b feature/xyz)
1. Submit a Pull Request


## FAQ

**Q: Why not use QStorageInfo directly?**  
A: Qt doesn't expose USB serial numbers natively—this library bridges the gap with platform-specific APIs.

**Q: Does it work with USB-C/NVMe drives?**  
A: Yes, as long as the OS detects them as removable storage.

**Q: Android permission requirements?**  
A: Add this to your `AndroidManifest.xml`:
```xml
<uses-permission android:name="android.permission.USB" />
```

## Example Output
```
USB Serial: "WD1234567890"  (Windows)
USB Serial: "12345ABC"      (macOS)
USB Serial: "SAMSUNG_XYZ"   (Linux)
```

## Support

Open an issue or contact asashnov@rambler.ru

## Credits

Developed with ❤️ using Qt.
