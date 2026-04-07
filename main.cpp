#include "class/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/pictures/assets/cfIcon.ico"));

    // 设置基础信息
    QCoreApplication::setOrganizationName("CF");
    QCoreApplication::setApplicationName("CFA");

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
