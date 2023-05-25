#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include <QString>
#include <QSettings>

class AppSettings
{
public:
    static const QString ORG_NAME;
    static const QString APP_NAME;
    static const QString APP_SETTINGS_FILE;

    static const QString QT_PLAY_MAJOR;
    static const QString QT_PLAY_MINOR;
    static const QString QT_PLAY_MICRO;

    /* for newline object using */
    static const QString NL_PATH;

    /* for account file using */
    static const QString ACCOUNT_PATH;

    /* for POG path using */
    static const QString POG_PATH;

    /* for File-Create path using */
    static const QString FILE_CDIR;

    /* for File-Rename path using */
    static const QString FILE_RDIR;

    /* for Sync-Src path using */
    static const QString SYNC_SDIR;

    /* for Sync-Dst path using */
    static const QString SYNC_DDIR;

    /* for Kindle note file path using */
    static const QString KNOTE_SDIR;
    /* for Kindle note save path using */
    static const QString KNOTE_DDIR;

    /*for Reminder config using */
    static const QString R_TIMEROUT;
    static const QString R_REPEAT;

public:
    AppSettings();

    static QString APP_VERSION();
};

#endif // APPSETTINGS_H
