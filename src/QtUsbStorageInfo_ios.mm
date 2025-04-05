#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <QString>

QString getIosSerial(const QStorageInfo &storage) {
    NSArray *paths = [[NSFileManager defaultManager] mountedVolumeURLsIncludingResourceValuesForKeys:nil options:0];
    for (NSURL *url in paths) {
        if (QString::fromNSString(url.path) == storage.rootPath()) {
            NSDictionary *attrs = [[NSFileManager defaultManager] attributesOfFileSystemForPath:url.path error:nil];
            return QString::fromNSString(attrs[NSFileSystemNumber]);
        }
    }
    return QString();
}
