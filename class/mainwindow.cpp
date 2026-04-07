#include "handledialog.h"
#include "mainwindow.h"
#include "profiles.h"
#include <QFile>
#include <QDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // app 名称设置
    this->setWindowTitle("Codeforces Arena");

    // 搭建主界面
    mainTags = new QTabWidget;
    mainTags->addTab(new Profiles(), tr("PROFILES"));
    this->setCentralWidget(mainTags);
}

MainWindow::~MainWindow() = default;

void MainWindow::showEvent(QShowEvent *event) {
    // 调用基类，确保窗口绘制完毕
    QMainWindow::showEvent(event);

    // 为了使主窗口先出现，将这个事件延迟 handle 检测（第一次运行）
    QTimer::singleShot(0, this, &MainWindow::handleCheck);
}

void MainWindow::handleCheck() {
    HandleDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getHandle();
        qDebug() << name << "!";
        this->setWindowTitle(tr("Codeforces Arena [%1] - Loading...").arg(name));
    } else {
        qDebug() << "Canceled!";
    }
}

