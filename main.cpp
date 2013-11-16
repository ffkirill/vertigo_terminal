#include <QApplication>
#include "html5applicationviewer.h"
#include <QSettings>
#include <QDir>
#include <QUrl>

QString adjustPath(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if (!QDir::isAbsolutePath(path))
        return QCoreApplication::applicationDirPath()
                + QLatin1String("/../Resources/") + path;
#else
    const QString pathInInstallDir = QCoreApplication::applicationDirPath()
        + QLatin1String("/../") + path;
    if (pathInInstallDir.contains(QLatin1String("opt"))
            && pathInInstallDir.contains(QLatin1String("bin"))
            && QFileInfo(pathInInstallDir).exists()) {
        return pathInInstallDir;
    }
#endif
#endif
    return QFileInfo(path).absoluteFilePath();
}

Q_DECLARE_METATYPE(QSettings*)
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSettings settings(adjustPath("settings/settings.ini"), QSettings::IniFormat);
    Html5ApplicationViewer viewer;
    viewer.showExpanded();
    viewer.loadUrl(QUrl(settings.value("General/url",
                                       "http://192.168.1.2/tokens/token_reader").toString()));

    return app.exec();
}
