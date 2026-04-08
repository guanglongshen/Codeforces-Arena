#include "handledialog.h"
#include "mainwindow.h"
#include "profiles.h"
#include <QFile>
#include <QDialog>
#include <QTimer>
#include <QSettings>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // app 名称设置
    this->setWindowTitle("Codeforces Arena");

    QSettings settings;
    handleSet = settings.value("handle", false).toBool() || settings.value("alwaysAskHandle", false).toBool();
    if (!handleSet) {
        // 为了使主窗口先出现，将这个事件延迟 handle 检测（第一次运行）
        QTimer::singleShot(0, this, &MainWindow::handleCheck);
    } else {
        // 之前已经设置了正确的 Handle，提取即可
        username = settings.value("username").toString();
        this->setWindowTitle(tr("Codeforces Arena [ %1 ] - Logined" ).arg(username));
    }

    menuInit();

    // 搭建主界面
    mainTags = new QTabWidget;
    mainTags->addTab(new Profiles(), tr("PROFILES"));
    this->setCentralWidget(mainTags);
}

MainWindow::~MainWindow() = default;

void MainWindow::handleCheck() {
    HandleDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        QSettings settings;
        settings.setValue("alwaysAskHandle", dialog.getAskHandle());

        QString name = dialog.getHandle();
        qDebug() << name << "!";
        this->setWindowTitle(tr("Codeforces Arena [%1] - Loading...").arg(name));
    } else {
        qDebug() << "Canceled!";
    }
}

void MainWindow::menuInit() {
    mBar = this->menuBar();
    QMenu *platformMenu = mBar->addMenu(tr("Codeforces Arena(&C)"));

    QAction *changeDefaultHandleAct = platformMenu->addAction(tr("Change Default Handle"));
    platformMenu->addSeparator();
    QAction *exitAct = platformMenu->addAction(tr("Exits"));

    connect(exitAct, &QAction::triggered, this, &QMainWindow::close);
    connect(changeDefaultHandleAct, &QAction::triggered, this, &MainWindow::handleCheck);
}


