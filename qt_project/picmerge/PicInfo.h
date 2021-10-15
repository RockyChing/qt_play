#ifndef PICINFO_H
#define PICINFO_H
#include <QObject>


#define PIC_INFO_VERSION (0x01)

typedef enum {
    PIC_FMT_JPG = 0,
    PIC_FMT_PNG,
    PIC_FMT_BMP,
    PIC_FMT_GIF,
    PIC_FMT_WEBP,
    PIC_FMT_MAX
} PicFormat;

typedef struct _MergeFileInfo {
    quint16 picInfoVer;
    quint16 picInfoCnt;
} MergeFileInfo;

typedef struct _PicInfo {
    PicFormat format;
    quint32 size;
} PicInfo;


#endif // PICINFO_H
