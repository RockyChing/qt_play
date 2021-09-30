#ifndef PICINFO_H
#define PICINFO_H
#include <QObject>

typedef enum {
    PIC_FMT_JPG = 0,
    PIC_FMT_PNG,
    PIC_FMT_BMP,
    PIC_FMT_GIF
} PicFormat;


typedef struct _PicInfo {
    PicFormat format;
    quint32 size;
} PicInfo;


#endif // PICINFO_H
