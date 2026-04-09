#include "handledialog.h"
#include "mainwindow.h"
#include "profiles.h"
#include <QFile>
#include <QDialog>
#include <QTimer>
#include <QSettings>
#include <QJsonDocument>
#include <QMenuBar>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // API 线程开启
    apiWorkerThread();

    // 菜单界面搭建
    menuInit();

    QSettings settings;
    bool handleSet = settings.value("firstRun", true).toBool();
    bool alwaysAsk = settings.value("alwaysAskHandle", false).toBool();
    username = settings.value("username").toString();

    if (handleSet || alwaysAsk) {
        if (!username.isEmpty()) {
            this->setWindowTitle(tr("Codeforces Arena [ %1 ] - logined").arg(username));
        }

        // 为了使主窗口先出现，将这个事件延迟 handle 检测（第一次运行）
        QTimer::singleShot(0, this, &MainWindow::handleCheck);
    } else {
        // 之前已经设置了正确的 Handle，提取即可
        this->setWindowTitle(tr("Codeforces Arena [ %1 ] - Logined" ).arg(username));
    }


    // 搭建主界面
    mainTags = new QTabWidget;
    mainTags->addTab(new Profiles(), tr("PROFILES"));
    this->setCentralWidget(mainTags);
}

MainWindow::~MainWindow() {
    // 析构函数需要处理好子线程生存
    if (apiThread && apiThread->isRunning()) {
        apiThread->quit();

        if (!apiThread->wait(1000)) {
            apiThread->terminate();
        }
    }
};

void MainWindow::handleCheck() {
    HandleDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        QSettings settings;
        settings.setValue("alwaysAskHandle", dialog.getAskHandle());
        settings.setValue("firstRun", false);

        QString Handlename = dialog.getHandle();
        settings.setValue("username", Handlename);

        emit sig_requestUserInfo(Handlename);

        this->setWindowTitle(tr("Codeforces Arena [ %1 ] - Loading...").arg(Handlename));
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

void MainWindow::apiWorkerThread() {
    apiThread = new QThread(this);
    worker = new CFApiWorker();

    worker->moveToThread(apiThread);

    // 将 MainWindow 各种 API 请求信号连接给 worker
    // 用户信息 UserInfo
    connect(this, &MainWindow::sig_requestUserInfo, worker, &CFApiWorker::requestUserInfo);

    // 接收 worker 各种 API GET 回的数据到 MainWindow 上
    // 用户信息
    connect(worker, &CFApiWorker::sig_GetUserInfo, this, &MainWindow::onUserInfoReceived);

    // 错误情况
    connect(worker, &CFApiWorker::sig_ErrorOccurred, this, [this](const QByteArray &data, const QString &message) {
        qDebug() << message;

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        qDebug() << obj["status"].toString() << "\n" << obj["comment"];
    });

    // worker 最后需要在子线程结束后进行销毁
    connect(apiThread, &QThread::finished, worker, &QObject::deleteLater);

    // 开启线程
    apiThread->start();
}

void MainWindow::onUserInfoReceived(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (obj["status"].toString() == "OK") {
        qDebug() << "Get Data";

        QJsonArray resultArray = obj["result"].toArray();
        if (!resultArray.isEmpty()) {
            QJsonObject userObj = resultArray.first().toObject();

            // 字段
            qDebug() << userObj["rank"] << " " << userObj["rating"];
            qDebug() << userObj["maxRank"] << " " << userObj["maxRating"];
            qDebug() << userObj["avatar"];

            this->setWindowTitle(tr("Codeforces Arena [ %1 - %2 ]").arg(userObj["handle"].toString()).arg(userObj["rating"].toInt()));
        }

    } else {
        qDebug() << "API ERROR: " << obj["comment"].toString();
    }
}


