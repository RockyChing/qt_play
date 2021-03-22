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

    /* for newline object using */
    static const QString NL_PATH;
public:
    AppSettings();
};

#endif // APPSETTINGS_H
