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

    /* for HTML-File-Create path using */
    static const QString HTML_DIR;

    /* for HTML-File-Rename path using */
    static const QString HTML_RDIR;

public:
    AppSettings();

    static QString APP_VERSION();
};

#endif // APPSETTINGS_H
