#include "class/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/pictures/assets/cfIcon.ico"));

    // 设置本地基础信息以及配置
    QCoreApplication::setOrganizationName("CF");
    QCoreApplication::setApplicationName("Codeforces Arena");
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath());
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CodeforcesArena_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.showMaximized();
    return QCoreApplication::exec();
}
